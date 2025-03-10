# GLCKIo2-Research
GLCKIo2.sys Research (PoC)
# Usage
- sc.exe create GLCKIo2 binPath=[Path to]\GLCKIO2.sys type=kernel
- sc.exe start GLKIo2
  - Run compiled main.cpp from command line cmd.exe (Non-admin)
# GLCKIo2 Driver IOCTL Fuzzer

## Overview
This project is a proof-of-concept (PoC) for fuzzing and testing IOCTL commands on the GLCKIo2 driver, specifically targeting potential vulnerabilities that allow arbitrary memory writes from user-mode to kernel-mode.

## Purpose
The primary objective of this fuzzer is to interact with the GLCKIo2 driver using its exposed IOCTL interfaces. It aims to validate whether the driver properly handles IOCTL requests, and it tests for potential security issues, such as improper input validation or the ability to execute arbitrary writes to memory.

## IOCTL Codes and Functions
Below is a list of the discovered IOCTL codes for the GLCKIo2 driver and their respective functions:

| IOCTL Code    | Decimal Value | Purpose/Function |
|---------------|---------------|------------------|
| `0x80102040` | `-2146426816` | Memory copy using `memmove`, followed by a call to `sub_1400017AC`, possibly related to physical memory mapping (`PHYSICAL_ADDRESS`). |
| `0x80102044` | `-2146426812` | Similar to the above but calls `sub_140001ACC` with a handle (`HANDLE`), possibly related to handle-based operations. |
| `0x80102050` | `-2146426800` | Handles port I/O operations (`__inbyte`, `__inword`, `__indword`) and memory read operations, with the behavior controlled by the `v15` value (`1`, `2`, or `4` for byte, word, and dword operations respectively). |
| `0x80102054` | `-2146426796` | Handles port I/O write operations (`__outbyte`, `__outword`, `__outdword`), also influenced by the `v15` value to determine data size. |
| `0x80102058` | `-2146426792` | Calls `__readmsr`, which reads from a Model Specific Register (MSR). This is typically related to low-level CPU control and debugging. |
| `0x8010205C` | `-2146426788` | Calls `sub_140001E0C`, which might involve a custom operation or a more complex interaction with kernel resources. |
| `0x80102060` | `-2146426784` | Calls `sub_140001998`, another custom function, potentially for advanced memory or hardware operations. |

## How to Use
1. Compile the fuzzer using a C++ compiler (e.g., Visual Studio).
2. Run the fuzzer on a test machine with the GLCKIo2 driver installed.
3. Monitor the output for any crashes, unexpected behavior, or potential vulnerabilities.

## Warning
This tool is intended for educational and research purposes only. Running it on a production system or without proper authorization could lead to system instability, crashes, or data loss.

## License
MIT License

