# Custom 8-Bit Discrete Logic CPU & Pipeline

## Overview
This repository contains the complete hardware design, schematics, and software interface pipeline for a custom 8-bit computing architecture built from the ground up using discrete logic chips on breadboards. Rather than relying solely on high-level abstractions, this project explores the bare-metal realities of computer architecture, instruction set microprogramming, and physical hardware debugging.

## Repository Structure & Architecture
The project is modularized into four distinct domains, separating the physical routing from the logic generation and the hardware programming interfaces:

* **`Hardware Description/`**: Contains the logical models and simulation files used to verify the CPU's datapath, control unit, and register behavior before and alongside physical construction.
* **`Schematics/`**: KiCad files mapping the complete physical routing of the discrete components, including the ALU, registers, memory modules, and clock circuitry.
* **`Microcode/`**: Custom scripts and source files that define the Instruction Set Architecture (ISA). This generates the precise control word sequences required to execute instructions over multiple clock cycles.
* **`Software-Hardware Interface/`**: The C++/Python firmware and desktop pipeline required to bridge the gap between a modern host machine and the physical 8-bit hardware, specifically for writing microcode and programs to the system's EEPROMs.

## Engineering Tradeoffs & Physical Design
Moving from logical design to physical breadboard implementation required navigating several strict engineering tradeoffs and hardware realities:

* **Clock Cycles & Bus Contention:** Achieving system stability required a precise positive-edge-then-negative-edge sequential read/write cycle. The system powers on with the clock initially low to prevent race conditions. To prevent bus contention during switching, the shared bus is driven for only half a clock cycle. 
* **RAM Write Timing:** Driving the bus for only half a cycle meant the write signal was too wide for the window when the data was actually stable. Resolving this required adding physical propagation delay and utilizing hardware logic to decrease the write signal width, ensuring accurate writes to RAM.
* **Instruction Set & Data Width:** To maximize the utility of the limited architecture, the ISA implements a 2-line fetch sequence for absolute and immediate values. This allows a full 8 bits to be passed directly as data while reserving the entirety of the primary 8-bit control word strictly for opcodes and instructions.
* **Signal Translation & Debugging:** Troubleshooting physical logic gates often introduces abstraction leaks. Diagnosing active-low vs. active-high signal behavior required careful visual translation on the breadboards, such as reversing LED polarities to accurately reflect the true electrical state of the RAM write sequences without masking the underlying logic.

## Sociotechnical Context
As modern computing scales into heavily abstracted, cloud-based environments, the foundational understanding of how electrons translate into logic is often lost. This project serves as a practical exploration of computing at its most fundamental level. Understanding the physical constraints of hardware—power draw, signal propagation delay, and physical footprint—is essential for designing the next generation of efficient, sustainable computing architectures.

## Build & Run Instructions
1. The firmware source files within the `Software-Hardware Interface/` directory must be imported and organized within a **PlatformIO** environment stack to compile the EEPROM programmer correctly.
2. Run the `Microcode/` generator scripts to compile the target `.bin` or `.hex` ROM images.
3. Use the desktop pipeline interface to flash the generated microcode and programs to the physical hardware.
