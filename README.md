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

* **Clock Cycle Sequencing:** Achieving system stability required designing a precise positive-edge-then-negative-edge sequential read/write cycle. Ensuring the system powers on correctly with the clock initially low was critical to preventing race conditions during state transitions.
* **Signal Translation & Debugging:** Troubleshooting physical logic gates often introduces abstraction leaks. For instance, diagnosing active-low vs. active-high signal behavior required careful visual translation on the breadboards, such as reversing LED polarities to accurately reflect the true electrical state of the RAM write sequences without adding latency-inducing inverter chips.
* **Microcode Density vs. Flexibility:** With a limited 8-bit control word space, the ISA was heavily optimized. Decisions had to be made regarding which control signals were strictly necessary to implement a Turing-complete set of operations versus those that would require expanding the physical EEPROM footprint.

## Sociotechnical Context
As modern computing scales into heavily abstracted, cloud-based environments, the foundational understanding of how electrons translate into logic is often lost. This project serves as a practical exploration of computing at its most fundamental level. Understanding the physical constraints of hardware—power draw, signal propagation delay, and physical footprint—is essential for designing the next generation of efficient, sustainable computing architectures.

## Build & Run Instructions
1. Navigate to the `Software-Hardware Interface/` directory to compile the EEPROM programmer.
2. Run the `Microcode/` generator scripts to compile the `.bin` or `.hex` ROM images.
3. Use the pipeline interface to flash the generated images to the physical hardware.
