# Memory Map

A memory map is a representation of the processor's address space. It defines where different hardware resources, such as Flash memory, SRAM, and peripherals, are located within the address space, allowing the CPU to access them through specific memory addresses.

The ARM Cortex-M4 processor provides a **32-bit address space**, allowing access to a total of **4 GB** of memory. This address space is divided into several regions, each reserved for a specific purpose.

---

# STM32F401RE Internal Memory

| Memory | Start Address | Size |
|---------|---------------|------|
| Flash | 0x08000000 | 512 KB |
| SRAM | 0x20000000 | 96 KB |

---

# ARM Cortex-M4 Address Space (4 GB)

```text
ARM Cortex-M4 Address Space (4 GB)
│
├── Code Region
├── SRAM Region
├── Peripheral Region
│   ├── APB1
│   ├── Reserved
│   ├── APB2
│   ├── Reserved
│   ├── AHB1
│   ├── Reserved
│   ├── AHB2
│   └── Reserved
├── Reserved
├── Not Used
└── Cortex-M4 Internal Peripherals
```

---

## Address Regions

| Region | Address Range |
|---------|---------------|
| Code Region | 0x00000000 – 0x1FFFFFFF |
| SRAM Region | 0x20000000 – 0x3FFFFFFF |
| Peripheral Region | 0x40000000 – 0x5FFFFFFF |
| Reserved | 0x60000000 – 0xBFFFFFFF |
| Not Used | 0xC0000000 – 0xDFFFFFFF |
| Cortex-M4 Internal Peripherals | 0xE0000000 – 0xFFFFFFFF |

---

# Peripheral Region

The peripheral region contains all on-chip peripherals of the STM32F401RE. These peripherals are grouped according to the bus they are connected to.

| Bus | Address Range |
|-----|---------------|
| APB1 | 0x40000000 – 0x400073FF |
| Reserved | 0x40007400 – 0x4000FFFF |
| APB2 | 0x40010000 – 0x40014BFF |
| Reserved | 0x40014C00 – 0x4001FFFF |
| AHB1 | 0x40020000 – 0x400267FF |
| Reserved | 0x40026800 – 0x4FFFFFFF |
| AHB2 | 0x50000000 – 0x5003FFFF |
| Reserved | 0x50040000 – 0x5FFFFFFF |

---

# Notes

- The Code Region contains Flash memory, System Memory, and Option Bytes.
- SRAM is mapped into a dedicated address region beginning at **0x20000000**.
- Peripheral registers are memory-mapped, allowing the CPU to access them using normal load and store instructions.
- Cortex-M4 internal peripherals (NVIC, SysTick, SCB, MPU, etc.) are located in the **0xE0000000** address region.