# FereOS

[![Built with C and Assembly](https://img.shields.io/badge/Built%20with-C%20&%20Assembly-blue.svg)](https://github.com/your-username/FereOS)
[![OS: 16-bit](https://img.shields.io/badge/OS-16--bit-important.svg)](https://en.wikipedia.org/wiki/16-bit)
[![Emulator: QEMU](https://img.shields.io/badge/Emulator-QEMU-red.svg)](https://www.qemu.org/)

Welcome to **FereOS**, a simple 16-bit operating system crafted as a learning project by a 15-year-old developer.


## ✨ Features

*   **Bootloader:** Written in x86 Assembly
*   **Kernel:** The core of the OS, developed in C.
*   **Core Drivers:**
    *   **VGA Text Mode Driver:**
    *   **Keyboard Input Driver:**
*   **Memory Management:** Implements a simple stack-based allocator.
*   **Simple File System:** which support folowing commands:
    *   `touch` - Create file.
    *   `ls` - Show a list of all files.
    *   `write` - Write some data in the file.
    *   `cat` - Read the content from file.
    *   `rm` - Delete the file.
    *   etc.
*   **Simple shell** supports different commands. you can see him enter `help` in the OS.

## 🛠️ Building & Running

To build and run FereOS, you will need a cross-compiler for `i686-elf` target, NASM, and QEMU.

### Available Make Commands:

*   `make clean` - Clean up all build artifacts.
*   `make` - Build the entire OS, producing the final bootable disk image.
*   `make run` - Launch the OS in the QEMU emulator.
*   `make disasm` - Generate a disassembly of the kernel. 

### Quick Start:

1.  Clone this repository:
    ```bash
    git clone https://github.com/Andrey2809-sys/FereOS.git
    cd FereOS
    ```
2.  Run it in QEMU:
    ```bash
    make run
    ```


## 🚀 What's Next?

This is just the beginning! Future plans include:
*   Executing user programs
*   Support kernel modules
*   Enhanced memory management (e.g., heap allocation).

---

**Star this repository if you find this project interesting or inspiring! ⭐**
