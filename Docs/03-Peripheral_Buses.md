# Peripheral Buses

STM32F401RE uses several buses to connect the CPU, memory units, DMA controllers,
and peripherals.

The main buses are:

- AHB1
- AHB2
- APB1
- APB2

These buses are organized according to the performance and complexity requirements
of the connected hardware.

---

## 1. AHB1

AHB1 is a high-performance bus used for system components and peripherals that
require efficient and relatively fast access.

Examples of components connected to AHB1 include:

- GPIOA
- GPIOB
- GPIOC
- GPIOD
- GPIOE
- GPIOH
- DMA1
- DMA2
- RCC
- CRC
- Flash interface

The maximum AHB1 clock frequency of the STM32F401RE is 84 MHz.

A simplified view:

    AHB Bus Matrix
           |
           v
         AHB1
     +-----+-----+
     |     |     |
    GPIO  DMA   RCC
     |
   GPIOA
   GPIOB
   GPIOC
   ...

GPIO peripherals are placed on AHB1 so that the CPU and other bus masters can
access their registers efficiently through the high-performance AHB infrastructure.

---

## 2. AHB2

AHB2 is another AHB bus in the STM32F401RE memory and bus architecture.

For the STM32F401RE, the main peripheral connected to AHB2 is:

- USB OTG FS

The AHB2 address region starts at:

    0x50000000

and the STM32F401RE uses a portion of this region for USB OTG FS.

The maximum AHB2 clock frequency is 84 MHz.

Simplified:

    AHB Bus Matrix
           |
           v
         AHB2
           |
           v
       USB OTG FS

The exact peripherals connected to a bus depend on the specific STM32 device.
Therefore, the peripheral list should always be checked against the STM32F401RE
reference manual rather than assuming that all STM32F4 devices have the same
peripheral mapping.

---

## 3. APB1

APB1 (Advanced Peripheral Bus 1) is designed for peripherals that do not require
the complexity and performance of the AHB protocol.

Examples include:

- TIM2
- TIM3
- TIM4
- TIM5
- USART2
- I2C1
- I2C2
- I2C3
- SPI2
- SPI3
- PWR
- WWDG

The maximum APB1 clock frequency of the STM32F401RE is 42 MHz.

Simplified:

    AHB1
      |
      v
    AHB/APB1 Bridge
      |
      v
     APB1
      |
      +---- USART2
      +---- I2C1
      +---- I2C2
      +---- SPI2
      +---- TIM2
      +---- ...

APB1 is simpler than AHB and is intended for lower-bandwidth peripherals.

---

## 4. APB2

APB2 is another peripheral bus.

Examples of peripherals connected to APB2 include:

- TIM1
- USART1
- USART6
- ADC1
- SPI1
- SYSCFG
- EXTI
- TIM9
- TIM10
- TIM11

The maximum APB2 clock frequency of the STM32F401RE is 84 MHz.

Simplified:

    AHB1
      |
      v
    AHB/APB2 Bridge
      |
      v
     APB2
      |
      +---- USART1
      +---- SPI1
      +---- ADC1
      +---- TIM1
      +---- SYSCFG
      +---- EXTI
      +---- ...

---

## 5. Why Architectural Separation between AHB and APB Exists veya Design Trade-offs Between AHB and APB

Not every peripheral requires the same bus architecture.

For example, a GPIO or DMA controller can benefit from the higher-performance
AHB infrastructure, while a peripheral such as USART or I2C usually does not
need the full complexity and bandwidth of AHB.

Using a simpler peripheral bus has several advantages:

- simpler hardware
- lower implementation complexity
- lower power consumption
- sufficient performance for low-bandwidth peripherals

Therefore, peripherals are distributed between AHB and APB according to their
architectural requirements.

The important point is that APB should not simply be thought of as "AHB but
slower".

APB uses a simpler bus protocol designed specifically for peripheral access.

---

## 6. AHB/APB Bridge

The AHB/APB bridge connects the AHB side to an APB bus.

Its primary purpose is to convert transactions between the AHB and APB protocols.

For example, when the CPU accesses USART2:

    CPU
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
    AHB/APB1 Bridge
     |
     v
    APB1
     |
     v
    USART2
     |
     v
    Register

The bridge allows an AHB master to access a peripheral that is implemented on
the APB bus.

While the bridge can also manage clock domain crossing and bus timing adaptation, its primary role remains protocol translation between AHB and APB.

The APB clock can also be configured differently from the AHB clock.

Therefore, it is incorrect to define the bridge simply as a frequency divider.

---

## 7. AHB1, AHB2, APB1 and APB2

The overall structure can be simplified as:

                           Cortex-M4
                               |
                     +---------+---------+
                     |         |         |
                   I-Bus     D-Bus     S-Bus
                     |         |         |
                     +---------+---------+
                               |
                               v
                        AHB Bus Matrix
                               |
                 +-------------+-------------+
                 |             |             |
                 v             v             v
               AHB1          AHB2       AHB/APB Bridges
                 |             |             |
        +--------+-------+     |       +-----+-----+
        |        |       |     |       |           |
       GPIO     DMA     RCC   USB     APB1        APB2
                                     |             |
                                     |             |
                                  USART2        USART1
                                  I2C1          SPI1
                                  SPI2          ADC1
                                  TIM2          TIM1
                                  ...           ...
