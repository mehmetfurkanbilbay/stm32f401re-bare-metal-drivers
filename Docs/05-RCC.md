
# RCC — Reset and Clock Control

## 1. What is RCC?

RCC stands for **Reset and Clock Control**.

RCC is one of the fundamental system peripherals of the STM32F401RE.
It is responsible for controlling the microcontroller's clock system
and peripheral reset signals.

In a bare-metal application, RCC is especially important because
peripherals usually need their clock to be enabled before their
registers can be configured and used.

For example, before configuring GPIOA, the GPIOA peripheral clock
must be enabled.


                     CPU
                      │
                      │
                    S-Bus
                      │
                      ↓
               AHB Bus Matrix
                      │
                      ↓
                     AHB1
                 ┌────┴────┐
                 ↓         ↓
                RCC      GPIOA
                 │
                 │
          Enable GPIOA Clock
                 │
                 └──────────────→ GPIOA can operate


---

# 2. Why Does a Peripheral Need a Clock?

Digital peripherals contain hardware logic that operates according to
clock signals.

Peripherals such as:

* GPIO
* USART
* SPI
* I2C
* TIM

use clock signals for their internal operation.

The STM32 can disable the clock of a peripheral when it is not being
used. This reduces unnecessary power consumption.

Therefore, a peripheral can physically exist inside the microcontroller
while its clock is disabled.

For example:

```text
GPIOA
 │
 ├── Exists in the MCU
 │
 └── Clock disabled
```

After enabling its clock:

```text
GPIOA
 │
 ├── Exists in the MCU
 │
 └── Clock enabled
          │
          ↓
     Registers can
       be accessed
```

This is why peripheral initialization usually begins with a clock
enable operation.

---

# 3. RCC and the Clock System

The STM32F401RE contains a clock tree that distributes clock signals
to different parts of the microcontroller.

A simplified representation is:

```text
                    Clock Sources
                         │
                         ↓
                        RCC
                         │
          ┌──────────────┼──────────────┐
          ↓              ↓              ↓
       System Clock     AHB            APB
                         │          ┌────┴────┐
                         │          ↓         ↓
                        AHB1       APB1      APB2
                         │
                 Peripheral Clocks
```

The actual clock tree is more complex and contains different clock
sources, prescalers, multiplexers, and clock paths.

For bare-metal development, the important idea is:

```text
Clock Source
     ↓
    RCC
     ↓
Clock Configuration
     ↓
Bus Clocks
     ↓
Peripheral Clocks
```

RCC therefore sits at the center of the STM32 clock configuration.

---

# 4. Clock Enable vs Clock Frequency

Two concepts must be clearly separated:

### Clock Enable

Determines whether a peripheral's clock is enabled.

```text
Clock Enable = ON
→ Peripheral clock is available

Clock Enable = OFF
→ Peripheral clock is disabled
```

### Clock Frequency

Determines the rate of the clock signal.

For example:

```text
AHB Clock
APB1 Clock
APB2 Clock
```

can operate at different frequencies depending on the clock
configuration.

Therefore:

```text
Clock Enable ≠ Clock Frequency
```

A register bit such as `GPIOAEN` does not determine the GPIOA clock
frequency.

It controls whether GPIOA's clock is enabled.

---

# 5. RCC and Peripheral Buses

The STM32F401RE contains multiple bus domains.

The important buses for peripheral clocking are:

```text
AHB1
AHB2
APB1
APB2
```

Different peripherals are connected to different bus domains.

A simplified representation is:

```text
                    AHB Bus Matrix
                          │
             ┌────────────┴────────────┐
             ↓                         ↓
           AHB1                      AHB2
             │                         │
       ┌─────┴─────┐                  │
       ↓           ↓                  ↓
     GPIO         RCC              AHB2 peripherals
       │
       ↓
    AHB/APB
     Bridge
       │
   ┌───┴────┐
   ↓        ↓
 APB1     APB2
```

This is important because the RCC register used to enable a peripheral
depends on the bus to which that peripheral belongs.

For example:

```text
GPIOA
 ↓
AHB1
 ↓
RCC_AHB1ENR
 ↓
GPIOAEN
```

---

# 6. Peripheral Clock Enable Registers

RCC contains several registers used to enable and disable clocks for
different peripheral bus domains.

For example:

```text
RCC_AHB1ENR
RCC_AHB2ENR
RCC_APB1ENR
RCC_APB2ENR
```

The names describe the bus domain controlled by the register.

For example:

```text
RCC_AHB1ENR
      │
      ↓
AHB1 peripheral clock enables
```

Similarly:

```text
RCC_APB1ENR
      │
      ↓
APB1 peripheral clock enables
```

and:

```text
RCC_APB2ENR
      │
      ↓
APB2 peripheral clock enables
```

---

# 7. GPIOA Clock Enable

GPIOA is connected to the AHB1 bus.

Therefore, its clock is controlled through the AHB1 peripheral clock
enable register.

```text
GPIOA
  │
  ↓
AHB1
  │
  ↓
RCC_AHB1ENR
  │
  ↓
GPIOAEN
```

The `GPIOAEN` bit controls the GPIOA clock.

Conceptually:

```text
GPIOAEN = 0
    ↓
GPIOA clock disabled

GPIOAEN = 1
    ↓
GPIOA clock enabled
```

This is one of the first RCC operations we will perform when writing
a GPIO driver.

---

# 8. Bit Manipulation and Clock Enable

A peripheral clock enable is normally performed by modifying a single
bit inside an RCC register.

For example:

```c
RCC_AHB1ENR |= (1 << GPIOAEN);
```

The important concept here is:

```text
RCC_AHB1ENR
      │
      ↓
Set GPIOAEN bit
      │
      ↓
GPIOA clock enabled
```

The expression:

```c
1 << GPIOAEN
```

creates a value where only the GPIOAEN bit is set.

For example, if the desired bit is bit 0:

```text
1 << 0

0000 0001
```

If the desired bit is bit 2:

```text
1 << 2

0000 0100
```

---

# 9. Why Do We Use `|=`?

Suppose an RCC register currently contains:

```text
0000 1010
```

and we want to set bit 0.

Using:

```c
RCC_AHB1ENR |= (1 << 0);
```

produces:

```text
0000 1010
OR
0000 0001
─────────
0000 1011
```

Only the desired bit is set.

However, if we wrote:

```c
RCC_AHB1ENR = (1 << 0);
```

the result would be:

```text
0000 0001
```

The previous values of the other bits would be overwritten.

Therefore:

```text
|=  → Set a specific bit while preserving the other bits
```

This is a fundamental technique in bare-metal register programming.

---

# 10. Read-Modify-Write

An expression such as:

```c
RCC_AHB1ENR |= (1 << GPIOAEN);
```

can be understood as a read-modify-write operation.

Conceptually:

```text
        Read register
             │
             ↓
      Modify GPIOAEN bit
             │
             ↓
       Write register
```

More explicitly:

```text
RCC_AHB1ENR
     │
     ↓
   READ
     │
     ↓
Set GPIOAEN = 1
     │
     ↓
   WRITE
```

This same pattern appears frequently when configuring STM32
peripheral registers.

For example:

```c
GPIOA->MODER |= ...
GPIOA->ODR   |= ...
RCC->AHB1ENR |= ...
```

All of these involve manipulating individual bits inside registers.

---

# 11. RCC Registers Are Memory-Mapped

RCC registers are memory-mapped.

This means the CPU accesses them through addresses in the peripheral
address space.

The general idea is:

```text
RCC Base Address
       +
Register Offset
       ↓
RCC Register Address
```

This is exactly the same concept we used for GPIO.

For GPIO:

```text
GPIOA Base Address
       +
ODR Offset
       ↓
GPIOA ODR Address
```

For RCC:

```text
RCC Base Address
       +
Register Offset
       ↓
RCC Register Address
```

The CPU does not fundamentally understand the names:

```text
RCC
AHB1ENR
GPIOAEN
```

These are software representations of hardware registers and bits.

At the hardware level, the CPU performs memory accesses to addresses.

---

# 12. RCC Access Path

When software modifies an RCC register, the operation can be
visualized as:

```text
C Code
  │
  ↓
Compiler
  │
  ↓
Machine Instruction
  │
  ↓
Memory Address
  │
  ↓
S-Bus
  │
  ↓
AHB Bus Matrix
  │
  ↓
AHB1
  │
  ↓
RCC
  │
  ↓
RCC Register
```

For example:

```c
RCC_AHB1ENR |= ...
```

is eventually translated into machine instructions that perform
a memory access to the RCC register.

The CPU is not performing an operation called:

```text
"enable GPIOA"
```

Instead, it is modifying a specific bit at a specific memory-mapped
register address.

---

# 13. RCC and GPIO Initialization

The relationship between RCC and GPIO can be visualized as:

```text
                    CPU
                     │
                     ↓
                    RCC
                     │
             Enable GPIOA Clock
                     │
                     ↓
                   GPIOA
                     │
        ┌────────────┼────────────┐
        ↓            ↓            ↓
      MODER        PUPDR         ODR
        │            │            │
        ↓            ↓            ↓
      Pin Mode    Pull Config   Output Data
```

The order is important.

First:

```text
RCC → Enable GPIOA clock
```

Then:

```text
GPIOA → Configure the pin
```

Then:

```text
GPIOA → Read/write GPIO registers
```

---

# 14. Clock Enable Does Not Configure the GPIO Pin

Enabling the GPIOA clock does **not** mean that GPIOA is configured
as an output.

These are two separate operations.

### RCC

```text
GPIOAEN = 1
```

means:

```text
GPIOA clock is enabled
```

### GPIO

```text
MODER
```

determines the operating mode of the GPIO pin.

For example:

```text
MODER = 01
```

for a particular pin means:

```text
Output mode
```

Therefore:

```text
RCC
 ↓
Enable peripheral clock

GPIO
 ↓
Configure peripheral
```

This distinction is essential when writing drivers.

---

# 15. RCC and Reset Control

RCC also controls reset signals for peripherals.

The name:

```text
Reset and Clock Control
```

therefore describes two major responsibilities:

```text
RCC
 │
 ├── Clock Control
 │
 └── Reset Control
```

A peripheral can be placed into reset and later released from reset.

Conceptually:

```text
             RCC
              │
       ┌──────┴──────┐
       ↓             ↓
    Clock          Reset
    Control        Control
       │             │
       ↓             ↓
 Peripheral       Peripheral
   Clock             Reset
```

Clock and reset are different mechanisms.

```text
Clock:
"Is the peripheral receiving its clock?"

Reset:
"Is the peripheral being held in its reset state?"
```

---

# 16. RCC and Power Consumption

Peripheral clock gating is also related to power management.

If a peripheral is not being used, its clock can be disabled.

Conceptually:

```text
Peripheral not used
       │
       ↓
Clock disabled
       │
       ↓
Reduced unnecessary switching
       │
       ↓
Lower power consumption
```

Therefore, clock enable registers are not only a software
initialization mechanism.

They are also part of the microcontroller's power management
architecture.

---

# 17. RCC Clock Tree and Prescalers

RCC also contains configuration for clock division.

A simplified representation is:

```text
             System Clock
                  │
                  ↓
                Prescaler
                  │
                  ↓
                 AHB
                  │
                  ↓
               AHB Clock
                  │
             ┌────┴────┐
             ↓         ↓
           APB1       APB2
```

A prescaler can divide a clock frequency before it reaches a
particular bus domain.

For example:

```text
Input Clock
    │
    ↓
Prescaler
    │
    ↓
Bus Clock
```

This is different from enabling or disabling a peripheral clock.

Remember:

```text
Prescaler
→ Changes clock frequency

Enable bit
→ Enables/disables a peripheral clock
```

---

# 18. RCC and the Bare-Metal Driver Architecture

When we eventually write our own drivers, RCC will become a
dependency of other peripheral drivers.

For example:

```text
             RCC Driver
                 │
       ┌─────────┼─────────┐
       ↓         ↓         ↓
    GPIO       USART       SPI
    Driver     Driver     Driver
```

A GPIO driver needs a way to enable the GPIO peripheral clock.

A USART driver needs a way to enable the USART peripheral clock.

An SPI driver needs a way to enable the SPI peripheral clock.

Therefore, RCC is part of the infrastructure used by peripheral
drivers.

---

# 19. RCC and Hardware Abstraction

In a HAL-based project, the programmer might call a function such as:

```c
HAL_GPIO_Init(...)
```

or another high-level initialization function.

The HAL internally performs the required register operations.

In a bare-metal driver, we want to understand and implement these
operations ourselves.

Instead of hiding the process:

```text
Application
    ↓
HAL
    ↓
RCC registers
    ↓
GPIO registers
```

we want to understand:

```text
Application
    ↓
Our Driver
    ↓
RCC Register
    ↓
GPIO Register
    ↓
Hardware
```

This is one of the main reasons RCC knowledge is important for
bare-metal development.

---

# 20. Example: GPIOA Initialization Flow

A simplified initialization flow for GPIOA is:

```text
                 Start
                   │
                   ↓
          Identify GPIOA bus
                   │
                   ↓
                  AHB1
                   │
                   ↓
         Find RCC clock-enable
              register
                   │
                   ↓
            Set GPIOAEN = 1
                   │
                   ↓
          GPIOA clock enabled
                   │
                   ↓
        Configure GPIOA MODER
                   │
                   ↓
       Configure other registers
                   │
                   ↓
          Use GPIOA peripheral
```

This sequence demonstrates the relationship between the bus
architecture, RCC, and GPIO.

---

# 21. Important RCC Concepts

The following concepts should be understood before implementing
the RCC part of a bare-metal driver:

### RCC

Reset and Clock Control.

### Clock Source

The source from which a clock signal originates.

### Clock Tree

The structure that distributes and modifies clock signals inside
the microcontroller.

### Prescaler

A circuit/configuration that divides a clock frequency.

### Clock Enable

Controls whether a peripheral clock is enabled.

### Reset Control

Controls the reset state of peripherals.

### Peripheral Bus

The bus domain to which a peripheral is connected.

For example:

```text
GPIOA → AHB1
```

The bus determines which RCC enable register is relevant.

---

# 22. Important Distinction

The following three concepts should not be confused:

```text
Address
Clock
Protocol
```

### Address

Answers:

```text
"Where am I accessing?"
```

Example:

```text
RCC register address
GPIOA register address
```

### Clock

Answers:

```text
"At what timing/frequency does the hardware operate?"
```

Example:

```text
AHB clock
APB1 clock
APB2 clock
```

### Protocol / Bus

Answers:

```text
"How does the transaction travel between components?"
```

Example:

```text
CPU
 ↓
S-Bus
 ↓
AHB Bus Matrix
 ↓
AHB1
 ↓
RCC
```

Keeping these three concepts separate prevents many mistakes in
microcontroller programming.

---

# 23. Simplified RCC Architecture

The concepts discussed above can be summarized as:

```text
                         Clock Sources
                              │
                              ↓
                             RCC
                    ┌─────────┼─────────┐
                    │         │         │
                    ↓         ↓         ↓
                  SYSCLK      AHB       APB
                              │       ┌──┴──┐
                              │       ↓     ↓
                             AHB1    APB1  APB2
                              │
                    ┌─────────┼─────────┐
                    ↓         ↓         ↓
                  GPIO       RCC       DMA
```

The exact internal clock tree is more detailed, but this diagram is
enough to understand the relationship between RCC and the bus
architecture.

---

# 24. RCC in the STM32F401RE Bare-Metal Project

For this project, the most important RCC relationship to remember is:

```text
GPIOA
  ↓
AHB1
  ↓
RCC_AHB1ENR
  ↓
GPIOAEN
  ↓
GPIOA Clock Enabled
```

Then:

```text
GPIOA Clock Enabled
        ↓
GPIOA Register Access
        ↓
MODER / OTYPER / OSPEEDR / PUPDR
        ↓
IDR / ODR / BSRR
        ↓
Physical GPIO Pin
```

This creates the complete initialization chain:

```text
CPU
 ↓
RCC
 ↓
Enable GPIOA Clock
 ↓
GPIOA
 ↓
Configure GPIO
 ↓
Access GPIO Register
 ↓
Physical Pin
```




