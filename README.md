# FereOS

[![Built with C and Assembly](https://img.shields.io/badge/Built%20with-C%20&%20Assembly-blue.svg)](https://github.com/your-username/FereOS)
[![OS: 16-bit](https://img.shields.io/badge/OS-16--bit-important.svg)](https://en.wikipedia.org/wiki/16-bit)
[![Emulator: QEMU](https://img.shields.io/badge/Emulator-QEMU-red.svg)](https://www.qemu.org/)

Welcome to **FereOS**, a 16-bit operating system crafted as a learning project by a 15-year-old developer.


## ✨ Features

*   **Bootloader:** Written in x86 Assembly, responsible for the initial startup and loading the kernel into memory.
*   **Kernel:** The core of the OS, developed in C.
*   **Core Drivers:**
    *   **VGA Text Mode Driver:** Handles all text output to the screen.
    *   **Keyboard Input Driver:** Utilizes BIOS interrupts to read user input from the keyboard.
*   **Memory Management:** Implements a simple and effective stack-based allocator.
*   **Interactive Shell:** A custom-built shell that supports the following commands:
    *   `info` - Display system information.
    *   `help` - Show a list of available commands.
    *   `cls` - Clear the terminal screen.
    *   `reboot` - Restart the system.
    *   `shutdown` - Power down the machine (in QEMU).

## 🛠️ Building & Running

To build and run FereOS, you will need a cross-compiler for `i686-elf` target, NASM, and QEMU.

### Available Make Commands:

*   `make clean` - Clean up all build artifacts.
*   `make` - Build the entire OS, producing the final bootable disk image.
*   `make run` - Launch the OS in the QEMU emulator.
*   `make disasm` - Generate a disassembly of the kernel. (Very useful for debugging and learning!)

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

## 🧠 A Note from the Developer

> Hey! I'm a 15-year-old developer who tried everything in programming - from web to apps. But when I discovered low-level programming and OS development, it just clicked. This is where the real magic happens!FereOS is my first operating system. I built it after just 1-2 weeks of learning. It's not perfect, but it's MINE. Every line of code taught me something new about how computers actually work.If you're a young developer thinking about trying OS dev - JUST DO IT. Don't let anyone tell you it's too hard. Start small, keep learning, and build cool shit!


## 🚀 What's Next?

This is just the beginning! Future plans for FereOS might include:
*   A simple File System.
*   Support for more complex commands and programs.
*   Enhanced memory management (e.g., heap allocation).

---

**Star this repository if you find this project interesting or inspiring! ⭐**

---
*FereOS - A journey into the heart of computing.*
