#!/usr/bin/env python3
# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
# SPDX-License-Identifier: BSD-3-Clause

"""Capture QCC74x CSI reports from the Zephyr Wi-Fi shell."""

from __future__ import annotations

import argparse
import csv
import re
import sys
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Optional, Sequence, Tuple


ANSI_ESCAPE_RE = re.compile(r"\x1b(?:[@-Z\\-_]|\[[0-?]*[ -/]*[@-~])")
MAC_ADDRESS_RE = re.compile(r"^(?:[0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}$")
CSI_MARKER = "CSI:"
CSI_HEADER = (
    "timestamp",
    "csi_port",
    "mac_addr",
    "rssi_dbm",
    "noise_dbm",
    "cfo_ppm",
    "agc",
    "ppdu_format",
    "rate",
    "mcs",
    "beamformed",
    "smoothing",
    "nsts",
    "nsc",
)


class CaptureError(RuntimeError):
    """Raised when the device cannot enter or complete CSI capture."""


@dataclass(frozen=True)
class CaptureConfig:
    port: str
    baudrate: int
    mac: str
    channel: int
    buffer: int
    interval_ms: int
    ready_after_isr: int
    interface: Optional[int] = None


@dataclass(frozen=True)
class CSIReport:
    timestamp: int
    csi_port: int
    mac_addr: str
    rssi_dbm: int
    noise_dbm: int
    cfo_ppm: int
    agc: int
    ppdu_format: int
    rate: int
    mcs: int
    beamformed: int
    smoothing: int
    nsts: int
    nsc: int
    csi_matrix: Tuple[Tuple[complex, ...], ...]
    raw_payload: str

    def header_values(self) -> Tuple[object, ...]:
        return (
            self.timestamp,
            self.csi_port,
            self.mac_addr,
            self.rssi_dbm,
            self.noise_dbm,
            self.cfo_ppm,
            self.agc,
            self.ppdu_format,
            self.rate,
            self.mcs,
            self.beamformed,
            self.smoothing,
            self.nsts,
            self.nsc,
        )


def _interface_arg(interface: Optional[int]) -> str:
    return "" if interface is None else f" -i{interface}"


def build_start_commands(config: CaptureConfig) -> Tuple[str, str, str]:
    interface = _interface_arg(config.interface)
    return (
        f"wifi channel{interface} -c{config.channel} -b2",
        f"csidma_start -a {config.mac} -b {config.buffer} "
        f"-i {config.interval_ms} -r {config.ready_after_isr}",
        f"wifi mode{interface} -m",
    )


def build_stop_commands(config: CaptureConfig) -> Tuple[str, str]:
    interface = _interface_arg(config.interface)
    return (f"csidma_stop {config.buffer}", f"wifi mode{interface} -s")


def extract_csi_payload(line: str) -> Optional[str]:
    clean_line = ANSI_ESCAPE_RE.sub("", line)
    marker_pos = clean_line.find(CSI_MARKER)
    if marker_pos < 0:
        return None
    return clean_line[marker_pos + len(CSI_MARKER) :].strip()


def _parse_timestamp(value: str) -> int:
    try:
        return int(value, 10)
    except ValueError:
        return int(value, 16)


def parse_csi_payload(payload: str) -> CSIReport:
    fields = [field.strip() for field in payload.split(",") if field.strip()]
    if len(fields) < len(CSI_HEADER):
        raise ValueError(
            f"expected at least {len(CSI_HEADER)} fields, got {len(fields)}"
        )

    nsts = int(fields[12])
    nsc = int(fields[13])
    if nsts < 0 or nsc <= 0:
        raise ValueError(f"invalid CSI dimensions: nsts={nsts}, nsc={nsc}")

    stream_count = nsts + 1
    expected_fields = len(CSI_HEADER) + stream_count * nsc * 2
    if len(fields) != expected_fields:
        raise ValueError(f"expected {expected_fields} fields, got {len(fields)}")

    matrix = []
    offset = len(CSI_HEADER)
    for _stream in range(stream_count):
        stream = []
        for _subcarrier in range(nsc):
            stream.append(complex(int(fields[offset]), int(fields[offset + 1])))
            offset += 2
        matrix.append(tuple(stream))

    return CSIReport(
        timestamp=_parse_timestamp(fields[0]),
        csi_port=int(fields[1]),
        mac_addr=fields[2].upper(),
        rssi_dbm=int(fields[3]),
        noise_dbm=int(fields[4]),
        cfo_ppm=int(fields[5]),
        agc=int(fields[6]),
        ppdu_format=int(fields[7]),
        rate=int(fields[8]),
        mcs=int(fields[9]),
        beamformed=int(fields[10]),
        smoothing=int(fields[11]),
        nsts=nsts,
        nsc=nsc,
        csi_matrix=tuple(matrix),
        raw_payload=payload,
    )


def command_error(output: str) -> Optional[str]:
    for raw_line in output.splitlines():
        line = ANSI_ESCAPE_RE.sub("", raw_line).strip()
        lower_line = line.lower()
        if "operation failed" in lower_line or "csi dma failed" in lower_line:
            return line
        if lower_line.startswith("invalid ") or lower_line.startswith("cannot "):
            return line
    return None


def _subcarrier_indexes(nsc: int) -> Sequence[int]:
    if nsc == 52:
        return tuple(range(-26, 0)) + tuple(range(1, 27))
    return tuple(range(nsc))


def write_csv(reports: Sequence[CSIReport], filename: Path) -> None:
    if not reports:
        raise CaptureError("no CSI reports were captured")

    stream_count = reports[0].nsts + 1
    nsc = reports[0].nsc
    subcarriers = _subcarrier_indexes(nsc)
    iq_header = []
    for stream in range(stream_count):
        for subcarrier in subcarriers:
            iq_header.extend((f"s{stream}_sc{subcarrier}_i", f"s{stream}_sc{subcarrier}_q"))

    filename.parent.mkdir(parents=True, exist_ok=True)
    with filename.open("w", newline="", encoding="utf-8") as csv_file:
        writer = csv.writer(csv_file)
        writer.writerow((*CSI_HEADER, *iq_header))
        for report in reports:
            if report.nsts + 1 != stream_count or report.nsc != nsc:
                raise CaptureError(
                    "CSI dimensions changed during capture; cannot write one fixed CSV schema"
                )
            iq_values = []
            for stream in report.csi_matrix:
                for value in stream:
                    iq_values.extend((int(value.real), int(value.imag)))
            writer.writerow((*report.header_values(), *iq_values))


def save_plot(reports: Sequence[CSIReport], mode: str, filename: Path) -> None:
    if not reports:
        raise CaptureError("no CSI reports were captured")

    try:
        import matplotlib

        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
        import numpy as np
    except ImportError as exc:
        raise CaptureError(
            "plotting requires numpy and matplotlib; install requirements.txt from the script directory"
        ) from exc

    filename.parent.mkdir(parents=True, exist_ok=True)
    subcarriers = np.asarray(_subcarrier_indexes(reports[-1].nsc))

    if mode == "line":
        vector = np.asarray(reports[-1].csi_matrix[0])
        magnitude = 20.0 * np.log10(np.abs(vector) + 1e-12)
        phase = np.unwrap(np.angle(vector)) * 180.0 / np.pi

        figure, (magnitude_axis, phase_axis) = plt.subplots(2, 1, figsize=(10, 7))
        magnitude_axis.plot(subcarriers, magnitude, marker=".")
        magnitude_axis.set_ylabel("Magnitude (dB)")
        magnitude_axis.grid(True)
        phase_axis.plot(subcarriers, phase, marker=".")
        phase_axis.set_xlabel("Subcarrier")
        phase_axis.set_ylabel("Unwrapped phase (deg)")
        phase_axis.grid(True)
        figure.suptitle(
            f"CSI {reports[-1].mac_addr}, RSSI {reports[-1].rssi_dbm} dBm"
        )
    elif mode == "matrix":
        vectors = np.asarray([report.csi_matrix[0] for report in reports[-200:]])
        magnitude = 20.0 * np.log10(np.abs(vectors) + 1e-12)
        phase = np.angle(vectors) * 180.0 / np.pi

        figure, (magnitude_axis, phase_axis) = plt.subplots(2, 1, figsize=(11, 8))
        magnitude_image = magnitude_axis.imshow(
            magnitude, aspect="auto", origin="lower", interpolation="nearest"
        )
        phase_image = phase_axis.imshow(
            phase,
            aspect="auto",
            origin="lower",
            interpolation="nearest",
            vmin=-180,
            vmax=180,
        )
        magnitude_axis.set_ylabel("Capture")
        magnitude_axis.set_title("Magnitude (dB)")
        phase_axis.set_xlabel("Subcarrier index")
        phase_axis.set_ylabel("Capture")
        phase_axis.set_title("Phase (deg)")
        figure.colorbar(magnitude_image, ax=magnitude_axis)
        figure.colorbar(phase_image, ax=phase_axis)
    else:
        raise ValueError(f"unsupported plot mode: {mode}")

    figure.tight_layout()
    figure.savefig(filename, dpi=150)
    plt.close(figure)


class ZephyrSerialShell:
    def __init__(self, serial_port, command_timeout: float, verbose: bool):
        self.serial_port = serial_port
        self.command_timeout = command_timeout
        self.verbose = verbose

    def _print_device_line(self, line: str) -> None:
        if self.verbose:
            print(f"[device] {ANSI_ESCAPE_RE.sub('', line).rstrip()}")

    def send_command(self, command: str, expected: Optional[str] = None) -> str:
        print(f">> {command}")
        self.serial_port.write((command + "\r\n").encode("utf-8"))
        self.serial_port.flush()

        output = []
        deadline = time.monotonic() + self.command_timeout
        while time.monotonic() < deadline:
            raw_line = self.serial_port.readline()
            if not raw_line:
                continue
            line = raw_line.decode("utf-8", errors="replace")
            output.append(line)
            self._print_device_line(line)
            joined_output = "".join(output)
            error = command_error(joined_output)
            if error:
                raise CaptureError(f"command '{command}' failed: {error}")
            if expected and expected in ANSI_ESCAPE_RE.sub("", joined_output):
                return joined_output

        if expected:
            raise CaptureError(
                f"command '{command}' timed out waiting for response '{expected}'"
            )
        return "".join(output)


def capture_reports(
    config: CaptureConfig,
    duration: float,
    count: int,
    command_timeout: float,
    verbose: bool,
) -> Sequence[CSIReport]:
    try:
        import serial
    except ImportError as exc:
        raise CaptureError(
            "serial capture requires pyserial; install requirements.txt from the script directory"
        ) from exc

    reports = []
    invalid_reports = 0
    capture_started = False
    monitor_enabled = False

    print(f"Opening {config.port} at {config.baudrate} baud")
    try:
        serial_port = serial.Serial(
            config.port, config.baudrate, timeout=0.2, write_timeout=2
        )
    except serial.SerialException as exc:
        raise CaptureError(f"cannot open {config.port}: {exc}") from exc

    shell = ZephyrSerialShell(serial_port, command_timeout, verbose)
    try:
        serial_port.reset_input_buffer()
        start_commands = build_start_commands(config)
        shell.send_command(start_commands[0], "Wi-Fi channel set to")
        shell.send_command(start_commands[1], "CSI DMA started")
        capture_started = True
        shell.send_command(start_commands[2], "Wi-Fi mode set to")
        monitor_enabled = True

        print(
            f"Capturing CSI for up to {duration:g} seconds"
            + (f" or {count} reports" if count else "")
        )
        deadline = time.monotonic() + duration
        while time.monotonic() < deadline and (count == 0 or len(reports) < count):
            raw_line = serial_port.readline()
            if not raw_line:
                continue
            line = raw_line.decode("utf-8", errors="replace")
            payload = extract_csi_payload(line)
            if payload is None:
                shell._print_device_line(line)
                continue
            try:
                report = parse_csi_payload(payload)
            except (TypeError, ValueError) as exc:
                invalid_reports += 1
                if verbose:
                    print(f"[warning] invalid CSI report: {exc}", file=sys.stderr)
                continue
            if report.mac_addr != config.mac.upper():
                if verbose:
                    print(
                        f"[warning] ignored CSI report from {report.mac_addr}",
                        file=sys.stderr,
                    )
                continue
            reports.append(report)
            if len(reports) == 1 or len(reports) % 25 == 0:
                print(
                    f"Captured {len(reports)} reports "
                    f"(RSSI {report.rssi_dbm} dBm, {report.nsc} subcarriers)"
                )
    except KeyboardInterrupt:
        print("Capture interrupted")
    finally:
        stop_commands = build_stop_commands(config)
        if capture_started:
            try:
                shell.send_command(stop_commands[0], "stopped csi buf")
            except CaptureError as exc:
                print(f"Warning: {exc}", file=sys.stderr)
        if monitor_enabled:
            try:
                shell.send_command(stop_commands[1], "Wi-Fi mode set to")
            except CaptureError as exc:
                print(f"Warning: {exc}", file=sys.stderr)
        serial_port.close()

    if invalid_reports:
        print(f"Skipped {invalid_reports} invalid CSI reports", file=sys.stderr)
    return reports


def _mac_address(value: str) -> str:
    if not MAC_ADDRESS_RE.fullmatch(value):
        raise argparse.ArgumentTypeError("expected MAC address AA:BB:CC:DD:EE:FF")
    return value.upper()


def _bounded_integer(name: str, minimum: int, maximum: int):
    def parse(value: str) -> int:
        integer = int(value)
        if integer < minimum or integer > maximum:
            raise argparse.ArgumentTypeError(
                f"{name} must be in the range {minimum}..{maximum}"
            )
        return integer

    return parse


def parse_args(argv: Optional[Sequence[str]] = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Capture, save, and plot QCC74x CSI from the Zephyr Wi-Fi shell."
    )
    parser.add_argument("--port", required=True, help="Zephyr console serial device")
    parser.add_argument("--baudrate", type=int, default=115200)
    parser.add_argument("--mac", required=True, type=_mac_address, help="transmitter MAC")
    parser.add_argument(
        "--channel",
        required=True,
        type=_bounded_integer("channel", 1, 14),
        help="2.4 GHz channel",
    )
    parser.add_argument("--interface", type=_bounded_integer("interface", 1, 255))
    parser.add_argument("--buffer", type=_bounded_integer("buffer", 0, 5), default=0)
    parser.add_argument("--interval", type=_bounded_integer("interval", 0, 0xFFFFFFFF), default=500)
    parser.add_argument("--ready", type=_bounded_integer("ready", 0, 1), default=1)
    parser.add_argument("--duration", type=float, default=30.0)
    parser.add_argument("--count", type=int, default=0, help="stop after this many reports")
    parser.add_argument("--csv", type=Path, default=Path("csi_output.csv"))
    parser.add_argument("--plot", choices=("off", "line", "matrix"), default="line")
    parser.add_argument("--plot-file", type=Path, default=Path("csi_plot.png"))
    parser.add_argument("--command-timeout", type=float, default=5.0)
    parser.add_argument("--verbose", action="store_true")
    args = parser.parse_args(argv)
    if args.duration <= 0:
        parser.error("--duration must be greater than zero")
    if args.count < 0:
        parser.error("--count must not be negative")
    if args.command_timeout <= 0:
        parser.error("--command-timeout must be greater than zero")
    return args


def main(argv: Optional[Sequence[str]] = None) -> int:
    args = parse_args(argv)
    config = CaptureConfig(
        port=args.port,
        baudrate=args.baudrate,
        mac=args.mac,
        channel=args.channel,
        buffer=args.buffer,
        interval_ms=args.interval,
        ready_after_isr=args.ready,
        interface=args.interface,
    )

    try:
        reports = capture_reports(
            config,
            duration=args.duration,
            count=args.count,
            command_timeout=args.command_timeout,
            verbose=args.verbose,
        )
        if not reports:
            raise CaptureError(
                "no CSI reports received; verify the channel, transmitter MAC, and traffic"
            )
        write_csv(reports, args.csv)
        print(f"Wrote {len(reports)} reports to {args.csv}")
        if args.plot != "off":
            save_plot(reports, args.plot, args.plot_file)
            print(f"Wrote {args.plot} plot to {args.plot_file}")
    except CaptureError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
