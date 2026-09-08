# GPIO

GPIO (General-Purpose Input/Output) is a peripheral that allows the
microcontroller to interact with external digital signals.

A GPIO pin can generally be configured as:

- Input
- Output
- Alternate Function
- Analog

Each GPIO port can control up to 16 I/O pins.

---

## 1. GPIO and the Bus Architecture

In the STM32F401RE, GPIO peripherals are connected to the AHB1 bus.

A simplified access path is:

    Cortex-M4
        |
      S-Bus
        |
        v
    AHB Bus Matrix
        |
        v
       AHB1
        |
        v
      GPIOA
        |
        v
      Register
        |
        v
       Pin

For example:

    CPU → S-Bus → AHB Bus Matrix → AHB1 → GPIOA → ODR

The CPU does not access GPIOA using the name "GPIOA".

GPIO is memory-mapped, meaning that its registers are assigned
specific memory addresses.

---

## 2. GPIO Base Address

Each GPIO port has a base address.

For example:

    GPIOA Base Address = 0x40020000

Other GPIO ports have different base addresses.

The base address represents the beginning of the register block
belonging to that GPIO peripheral.

Registers are then located at offsets from the base address.

For example:

    GPIOA Base Address
          |
          v
      0x40020000
          |
          +---- MODER
          +---- OTYPER
          +---- OSPEEDR
          +---- PUPDR
          +---- IDR
          +---- ODR
          +---- BSRR
          +---- LCKR
          +---- AFRL
          +---- AFRH

---

## 3. GPIO Register Map

The GPIO peripheral contains several memory-mapped registers.

Important registers include:

| Offset | Register | Purpose |
|---:|---|---|
| 0x00 | MODER | GPIO mode selection |
| 0x04 | OTYPER | Output type selection |
| 0x08 | OSPEEDR | Output speed selection |
| 0x0C | PUPDR | Pull-up / pull-down selection |
| 0x10 | IDR | Input data |
| 0x14 | ODR | Output data |
| 0x18 | BSRR | Bit set/reset |
| 0x1C | LCKR | Configuration lock |
| 0x20 | AFRL | Alternate function selection for pins 0-7 |
| 0x24 | AFRH | Alternate function selection for pins 8-15 |

Each GPIO register is memory-mapped.

For example:

    GPIOA Base Address = 0x40020000

    ODR Offset = 0x14

Therefore:

    GPIOA ODR Address
    = 0x40020000 + 0x14
    = 0x40020014

The CPU ultimately accesses the address:

    0x40020014

rather than accessing a symbolic name such as "GPIOA->ODR".

---

## 4. GPIO MODER Register

The MODER register determines the operating mode of each GPIO pin.

Each GPIO pin uses two bits in MODER.

The possible configurations are:

    00 → Input mode
    01 → General-purpose output mode
    10 → Alternate function mode
    11 → Analog mode

For example, the configuration of PA5 is controlled by:

    MODER5[1:0]

The two bits are:

    MODER[11:10]

because each pin uses two bits.

The general pattern is:

    Pin n → MODER[2n+1 : 2n]

For example:

    Pin 0 → bits [1:0]
    Pin 1 → bits [3:2]
    Pin 2 → bits [5:4]
    Pin 5 → bits [11:10]

---

## 5. Configuring PA5 as Output

Suppose we want PA5 to operate as a general-purpose output.

PA5 uses bits 11 and 10 of MODER.

The required value is:

    01

because:

    00 → Input
    01 → Output
    10 → Alternate Function
    11 → Analog

Therefore:

    MODER[11:10] = 01

Conceptually:

    GPIOA
      |
     MODER
      |
      +---- bits 11:10
               |
               +---- PA5 mode
                      |
                      01
                      |
                      v
                    OUTPUT

---

## 6. OTYPER Register

OTYPER determines the output type of a GPIO configured as an output.

The main configurations are:

    0 → Push-pull
    1 → Open-drain

Each GPIO pin uses one bit.

For example:

    OTYPER[5]

controls the output type of PA5.

Therefore:

    OTYPER[5] = 0

selects push-pull.

While:

    OTYPER[5] = 1

selects open-drain.

---

## 7. OSPEEDR Register

OSPEEDR controls the output speed configuration of each GPIO pin.

Each pin uses two bits.

The exact electrical behavior should be understood as the GPIO output
driver's speed/slew configuration rather than simply interpreting the
value as the CPU clock frequency.

For example:

    OSPEEDR[11:10]

controls the output speed configuration of PA5.

This does NOT mean:

    "PA5 runs at 84 MHz"

The GPIO output speed setting and the AHB1 bus clock are different concepts.

---

## 8. PUPDR Register

PUPDR controls the pull-up and pull-down configuration of each GPIO pin.

Each pin uses two bits.

The possible configurations are:

    00 → No pull-up / pull-down
    01 → Pull-up
    10 → Pull-down
    11 → Reserved

For example:

    PUPDR[11:10]

controls the pull-up / pull-down configuration of PA5.

---

## 9. IDR Register

IDR means:

    Input Data Register

It contains the digital input state of the GPIO pins.

For example:

    IDR[5]

represents the input state associated with PA5.

Conceptually:

    External Signal
          |
          v
        PA5
          |
          v
        IDR[5]
          |
          v
         CPU

IDR is read-only from the software perspective.

---

## 10. ODR Register

ODR means:

    Output Data Register

It stores the output data for GPIO pins configured as outputs.

For example:

    ODR[5]

controls the output data associated with PA5.

Conceptually:

    CPU
     |
     v
    ODR[5]
     |
     v
    GPIO Output Driver
     |
     v
    PA5 Pin

A value of:

    0 → Low output

A value of:

    1 → High output

for a GPIO configured as a normal output.

---

## 11. Read-Modify-Write

A common GPIO operation is changing one bit while preserving
the other bits in the register.

For example:

    GPIOA->ODR |= (1 << 5);

Conceptually, this performs:

    1. Read ODR
    2. Set bit 5
    3. Write the modified value back to ODR

Simplified:

        ODR
         |
         v
      Read value
         |
         v
      Set bit 5
         |
         v
      Write value
         |
         v
        ODR

This is called a read-modify-write operation.

---

## 12. BSRR Register

GPIO also provides the BSRR register.

BSRR means:

    Bit Set/Reset Register

It provides a way to set or reset individual GPIO output bits
without performing the same type of read-modify-write operation
on ODR.

Conceptually:

    BSRR
      |
      +---- Set bits
      |
      +---- Reset bits

For example, a bit can be written through the set portion of BSRR
to set the corresponding GPIO output.

The reset portion can be used to clear the corresponding output.

This is useful when atomic bit manipulation is desired.

---

## 13. Alternate Function

GPIO pins are not limited to simple digital input/output.

A pin can also be connected to an internal peripheral through
the Alternate Function mechanism.

For example:

    GPIO Pin
       |
       +---- GPIO
       |
       +---- USART
       |
       +---- SPI
       |
       +---- I2C
       |
       +---- TIM
       |
       +---- Other peripheral functions

The GPIO alternate-function registers are:

    AFRL → pins 0-7
    AFRH → pins 8-15

These registers select which alternate function is connected
to a particular GPIO pin.

Therefore, GPIO is also part of the connection between physical
microcontroller pins and internal peripherals.

---

## 14. Example: PA5 as a GPIO Output

Suppose PA5 will be used as a digital output.

The conceptual configuration is:

    GPIOA
      |
      +---- MODER
      |      |
      |      +---- PA5 = Output
      |
      +---- OTYPER
      |      |
      |      +---- PA5 = Push-pull
      |
      +---- OSPEEDR
      |      |
      |      +---- PA5 = Selected output speed
      |
      +---- PUPDR
      |      |
      |      +---- PA5 = Selected pull configuration
      |
      +---- ODR
             |
             +---- PA5 = 0 / 1

After configuration, writing to the appropriate output register
changes the output state of PA5.

---

## 15. GPIO Address Calculation

GPIO register addresses can be calculated using:

    Register Address
    = Peripheral Base Address + Register Offset

For GPIOA:

    Base Address = 0x40020000

For ODR:

    Offset = 0x14

Therefore:

    ODR Address
    = 0x40020000 + 0x14
    = 0x40020014

This gives the physical memory-mapped address of GPIOA's ODR register.

---

## 16. From C Code to Hardware

Consider:

    GPIOA->ODR |= (1 << 5);

At the C language level, this is a readable abstraction.

At the hardware level, the CPU eventually performs memory accesses
to the corresponding address.

Conceptually:

    GPIOA->ODR
          |
          v
    GPIOA base address
          |
          +---- 0x40020000
          |
          v
    ODR offset
          |
          +---- 0x14
          |
          v
    Final address
          |
          +---- 0x40020014
          |
          v
    S-Bus
          |
          v
    AHB Bus Matrix
          |
          v
    AHB1
          |
          v
    GPIOA
          |
          v
    ODR
          |
          v
    GPIO output logic
          |
          v
    PA5

The CPU does not know the symbolic names:

    GPIOA
    ODR
    PA5

These names are abstractions provided by software.

The hardware ultimately operates using addresses, control signals,
and data.

---

## 17. Important Distinction: GPIO Speed vs Bus Speed

Two different concepts must not be confused.

### AHB1 Clock

The AHB1 bus has a clock frequency that determines the timing
of bus transactions within that bus domain.

### GPIO Output Speed

The GPIO OSPEEDR register controls characteristics of the GPIO
output driver.

Therefore:

    AHB1 frequency ≠ GPIO output speed

A GPIO configured for a particular output speed does not mean that
the GPIO pin is operating at the same frequency as the AHB1 bus.

The actual maximum signal frequency also depends on the GPIO hardware,
electrical characteristics, load, configuration, and how the software
generates the signal.

---

## 18. Summary

The GPIO peripheral can be understood as a collection of
memory-mapped registers controlling physical I/O pins.

The important structure is:

    GPIOA
      |
      +---- MODER   → Pin mode
      +---- OTYPER  → Output type
      +---- OSPEEDR → Output speed configuration
      +---- PUPDR   → Pull-up / pull-down
      +---- IDR     → Input data
      +---- ODR     → Output data
      +---- BSRR    → Bit set/reset
      +---- LCKR    → Configuration lock
      +---- AFRL    → Alternate functions 0-7
      +---- AFRH    → Alternate functions 8-15

The complete hardware access chain is:

    C Code
      ↓
    Compiler
      ↓
    Machine Instructions
      ↓
    Memory Address
      ↓
    Cortex-M4 S-Bus
      ↓
    AHB Bus Matrix
      ↓
    AHB1
      ↓
    GPIO Peripheral
      ↓
    GPIO Register
      ↓
    Register Bit
      ↓
    Physical GPIO Pin

Understanding this chain is the foundation for writing a
bare-metal GPIO driver.