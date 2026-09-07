# Bus Architecture

## What is a Bus?

A bus is a communication pathway used to transfer data, addresses, and control signals between different components of a microcontroller.

A bus allows components such as the CPU, memory, DMA, and peripherals to communicate with each other.

---

## AHB (Advanced High-performance Bus)

AHB (Advanced High-performance Bus) is a high-performance bus defined as part of ARM's AMBA specification.

It is designed to connect high-performance components such as:

- CPU
- DMA
- Flash memory
- SRAM

AHB provides higher performance and more complex communication features compared to APB.

---

## APB (Advanced Peripheral Bus)

APB (Advanced Peripheral Bus) is a bus defined as part of ARM's AMBA specification.

Unlike AHB, APB is designed for simpler and lower-speed peripherals. It also requires less hardware complexity and consumes less power.

Peripherals such as UART, SPI, I2C, and timers can be connected to APB.

---

## AHB Bus Matrix

The AHB bus matrix is an interconnect that connects multiple bus masters to different bus slaves.

It acts like a crossbar between masters and slaves and manages access between them.

This architecture allows multiple transfers to progress in parallel when different resources are available.

### Bus Masters

A bus master is a component that initiates a bus transaction and requests access to a slave.

Examples of bus masters in the STM32F401RE include:

- Cortex-M4 I-Bus
- Cortex-M4 D-Bus
- Cortex-M4 S-Bus
- DMA1 Memory Bus
- DMA2 Memory Bus
- DMA2 Peripheral Bus

### Bus Slaves

A bus slave is a component that responds when it is addressed by a master and performs the requested operation.

Examples of bus slaves include:

- Flash memory
- SRAM
- AHB1 peripherals
- AHB2 peripherals

---

## Cortex-M4 Bus Interfaces

The Cortex-M4 core has different bus interfaces for different types of accesses.

### I-Bus

The I-Bus is mainly used for instruction fetches.

For example, when the CPU needs to execute an instruction stored in Flash memory:

```text
CPU
 │
 └── I-Bus
       ↓
   AHB Bus Matrix
       ↓
      Flash
       ↓
   Instruction
       ↓
      CPU