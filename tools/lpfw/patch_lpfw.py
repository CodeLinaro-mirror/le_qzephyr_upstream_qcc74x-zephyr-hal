# Copyright (c) 2026 Qualcomm Technologies, Inc. and/or its subsidiaries
# SPDX-License-Identifier: BSD-3-Clause

#!/usr/bin/env python3

import argparse
import hashlib
import struct
import sys
from pathlib import Path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Append a QCC74x LPFW blob to an application binary."
    )
    parser.add_argument("app_bin", help="Path to the application binary")
    parser.add_argument("lpfw_bin", help="Path to the LPFW binary blob")
    return parser.parse_args()


def main() -> int:
    args = parse_args()

    app_path = Path(args.app_bin)
    lpfw_path = Path(args.lpfw_bin)

    app_data = bytearray(app_path.read_bytes())
    lpfw_data = lpfw_path.read_bytes()

    if len(app_data) < 64:
        print(f"{app_path} is too small to contain an LPFW placeholder", file=sys.stderr)
        return 1

    if app_data[-32:-28] != b"LPFW":
        print(f"{app_path} missing LPFW placeholder, clean and rebuild first", file=sys.stderr)
        return 1

    digest = hashlib.sha256(lpfw_data).digest()
    app_data[-64:-32] = digest
    app_data[-32:] = b"LPFW" + struct.pack("<I", len(lpfw_data)) + bytes(24)
    app_data.extend(lpfw_data)

    self_table_offset = app_data.find(b"SELF")
    if self_table_offset < 0:
        print(f"{app_path} missing SELF image table", file=sys.stderr)
        return 1

    app_data[self_table_offset + 12 : self_table_offset + 16] = struct.pack(
        "<I", len(app_data)
    )
    app_path.write_bytes(app_data)

    print(f"patched {app_path} with {lpfw_path.name} ({len(lpfw_data)} bytes)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
