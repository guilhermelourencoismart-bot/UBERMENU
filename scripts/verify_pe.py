"""Reject a missing, truncated, non-AMD64 or non-DLL build artifact."""
import argparse
from pathlib import Path
import struct


def verify(data):
    if len(data) < 64 or data[:2] != b'MZ':
        raise ValueError('Not a DOS/PE image')
    offset = struct.unpack_from('<I', data, 60)[0]
    if offset < 64 or offset + 26 > len(data) or data[offset:offset + 4] != b'PE\0\0':
        raise ValueError('Missing or truncated PE header')
    machine, sections = struct.unpack_from('<HH', data, offset + 4)
    optional_size, flags = struct.unpack_from('<HH', data, offset + 20)
    if machine != 0x8664:
        raise ValueError('Artifact is not AMD64/x64')
    if not flags & 0x2000:
        raise ValueError('Artifact is not marked as a DLL')
    if not sections or optional_size < 112 or offset + 24 + optional_size + sections * 40 > len(data):
        raise ValueError('Truncated optional header or section table')
    if struct.unpack_from('<H', data, offset + 24)[0] != 0x20B:
        raise ValueError('Artifact is not PE32+')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('dll', type=Path)
    args = parser.parse_args()
    try:
        verify(args.dll.read_bytes())
    except (OSError, ValueError) as error:
        parser.exit(1, f'ERROR: {error}\n')
    print(f'Confirmed AMD64 PE32+ DLL: {args.dll}')
