from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent.parent

input_path = PROJECT_ROOT / "easy_program.hex"
output_path = PROJECT_ROOT / "include" / "ProgramImage.hpp"

bytes_out = []

for line_number, line in enumerate(input_path.read_text().splitlines(), start=1):
    line = line.strip()

    if not line or line.startswith("#"):
        continue

    value = int(line, 16)

    if value < 0 or value > 0xFF:
        raise ValueError(f"Invalid byte on line {line_number}: {line}")

    bytes_out.append(value)

array_text = ",\n    ".join(f"0x{b:02X}" for b in bytes_out)

output_path.parent.mkdir(parents=True, exist_ok=True)

output_path.write_text(f"""
                       
#pragma once
#include <Arduino.h>

constexpr uint16_t PROGRAM_START_ADDRESS = 0x00;

constexpr uint8_t PROGRAM_IMAGE[] = {{
    {array_text},
}};

constexpr uint16_t PROGRAM_SIZE = sizeof(PROGRAM_IMAGE) / sizeof(PROGRAM_IMAGE[0]);

""")

print(f"Generated {output_path}")
print(f"Wrote {len(bytes_out)} bytes")