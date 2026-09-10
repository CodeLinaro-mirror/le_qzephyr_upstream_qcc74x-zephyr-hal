# Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries
# SPDX-License-Identifier: BSD-3-Clause

import csv
import importlib.util
import tempfile
import unittest
from pathlib import Path

from csi_capture import (
    CaptureConfig,
    build_start_commands,
    build_stop_commands,
    command_error,
    extract_csi_payload,
    parse_csi_payload,
    save_plot,
    write_csv,
)


class CSIParserTest(unittest.TestCase):
    def test_extracts_payload_from_colored_shell_output(self):
        line = "\x1b[1;32mqcc74x:~$\x1b[mCSI: 100,0,AA:BB:CC:DD:EE:FF,-20"

        self.assertEqual(
            extract_csi_payload(line),
            "100,0,AA:BB:CC:DD:EE:FF,-20",
        )

    def test_parses_header_and_iq_matrix(self):
        payload = (
            "100,0,8C:DE:F9:BB:26:E0,-20,-90,11,81,0,11,0,0,0,0,2,"
            "10,-5,20,-15,"
        )

        report = parse_csi_payload(payload)

        self.assertEqual(report.timestamp, 100)
        self.assertEqual(report.mac_addr, "8C:DE:F9:BB:26:E0")
        self.assertEqual(report.rssi_dbm, -20)
        self.assertEqual(report.nsts, 0)
        self.assertEqual(report.nsc, 2)
        self.assertEqual(report.csi_matrix, ((complex(10, -5), complex(20, -15)),))

    def test_rejects_truncated_iq_matrix(self):
        payload = (
            "100,0,8C:DE:F9:BB:26:E0,-20,-90,11,81,0,11,0,0,0,0,2,"
            "10,-5,"
        )

        with self.assertRaisesRegex(ValueError, "expected 18 fields, got 16"):
            parse_csi_payload(payload)


class ZephyrCommandTest(unittest.TestCase):
    def test_uses_default_sta_interface_without_hard_coded_index(self):
        config = CaptureConfig(
            port="/dev/ttyACM0",
            baudrate=115200,
            mac="8C:DE:F9:BB:26:E0",
            channel=1,
            buffer=0,
            interval_ms=500,
            ready_after_isr=1,
        )

        self.assertEqual(
            build_start_commands(config),
            (
                "wifi channel -c1 -b2",
                "csidma_start -a 8C:DE:F9:BB:26:E0 -b 0 -i 500 -r 1",
                "wifi mode -m",
            ),
        )
        self.assertEqual(
            build_stop_commands(config),
            ("csidma_stop 0", "wifi mode -s"),
        )

    def test_supports_an_explicit_sta_interface(self):
        config = CaptureConfig(
            port="/dev/ttyACM0",
            baudrate=115200,
            mac="8C:DE:F9:BB:26:E0",
            channel=1,
            buffer=2,
            interval_ms=250,
            ready_after_isr=0,
            interface=2,
        )

        self.assertEqual(build_start_commands(config)[0], "wifi channel -i2 -c1 -b2")
        self.assertEqual(build_start_commands(config)[2], "wifi mode -i2 -m")
        self.assertEqual(build_stop_commands(config)[1], "wifi mode -i2 -s")

    def test_detects_zephyr_and_csidma_command_failures(self):
        self.assertEqual(
            command_error("channel set operation failed with reason -134"),
            "channel set operation failed with reason -134",
        )
        self.assertEqual(
            command_error("CSI DMA failed to start"),
            "CSI DMA failed to start",
        )
        self.assertIsNone(command_error("Wi-Fi mode set to 2"))


class CSIOutputTest(unittest.TestCase):
    def setUp(self):
        self.report = parse_csi_payload(
            "100,0,8C:DE:F9:BB:26:E0,-20,-90,11,81,0,11,0,0,0,0,2,"
            "10,-5,20,-15,"
        )

    def test_writes_fixed_width_csv_with_iq_headers(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            output = Path(temp_dir) / "capture.csv"

            write_csv([self.report], output)

            with output.open(newline="", encoding="utf-8") as csv_file:
                rows = list(csv.reader(csv_file))
            self.assertEqual(
                rows[0][-4:],
                ["s0_sc0_i", "s0_sc0_q", "s0_sc1_i", "s0_sc1_q"],
            )
            self.assertEqual(rows[1][-4:], ["10", "-5", "20", "-15"])
            self.assertEqual(len(rows[0]), len(rows[1]))

    @unittest.skipUnless(
        importlib.util.find_spec("matplotlib")
        and importlib.util.find_spec("numpy"),
        "plot test requires numpy and matplotlib",
    )
    def test_saves_line_and_matrix_plots(self):
        with tempfile.TemporaryDirectory() as temp_dir:
            for mode in ("line", "matrix"):
                output = Path(temp_dir) / f"{mode}.png"

                save_plot([self.report, self.report], mode, output)

                self.assertGreater(output.stat().st_size, 1000)


if __name__ == "__main__":
    unittest.main()
