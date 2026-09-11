# STM32F401RE — Flash Memory

## 1. What Is Flash Memory?

Flash memory is a **non-volatile memory**.

Non-volatile means that the stored data remains available when the microcontroller is powered off.

This makes Flash fundamentally different from SRAM.

| Memory    | Volatile?             | Main Purpose                     |
| --------- | --------------------- | -------------------------------- |
| Flash     | No                    | Program code and persistent data |
| SRAM      | Yes                   | Runtime variables, stack, heap   |
| Registers | No persistent storage | Hardware control and status      |

When an STM32F401RE is programmed, the compiled program is normally stored in the internal Flash memory.

For example:

```text
C source code
     |
     v
Compiler
     |
     v
Machine code
     |
     v
Flash memory
     |
     v
Cortex-M4 fetches instructions
```

Therefore, Flash is not simply a storage device.

It is part of the **CPU instruction execution path**.

---

# 2. Why Does the STM32 Need Flash?

Consider this program:

```c
int main(void)
{
    while (1)
    {
    }
}
```

The CPU cannot execute the C source code directly.

The compiler converts it into machine instructions.

Those instructions have to exist somewhere from which the Cortex-M4 can fetch them.

In a typical STM32F401RE application:

```text
                    STM32F401RE
                         |
                         v
                  Cortex-M4 CPU
                         |
                         |
                  Instruction Fetch
                         |
                         v
                 Flash Interface
                         |
                         v
                  Flash Memory
                         |
                         v
                  Machine Code
```

This is one of the most important reasons Flash exists in a microcontroller.

---

# 3. Flash vs SRAM

Flash and SRAM have different physical purposes.

### Flash

Flash is mainly used for:

* Program code
* Constant data
* Persistent application data
* Configuration data
* Boot code

### SRAM

SRAM is mainly used for:

* Runtime variables
* Stack
* Heap
* Temporary data
* Buffers

The important difference is persistence.

```text
Power OFF

Flash:
[ Program remains ]

SRAM:
[ Contents are lost ]
```

This is why the program can still exist after disconnecting the STM32 from power.

---

# 4. STM32F401RE Flash Capacity

The STM32F401RE belongs to the STM32F401xD/E group.

For the STM32F401xD/E devices, the main Flash memory can be up to **512 KB**.

For the STM32F401RE specifically:

```text
Flash Size = 512 KB
```

The main Flash memory occupies:

```text
0x08000000
      |
      |
      v
0x0807FFFF
```

Therefore:

```text
Flash start = 0x08000000
Flash end   = 0x0807FFFF
```

The address range contains:

```text
0x08000000 → 0x0807FFFF
```

which corresponds to 512 KB.

The STM32F401xD/E memory map confirms this range.

---

# 5. Flash Memory Organization

The Flash is not one completely uniform block.

It is divided into **sectors**.

For STM32F401xD/E:

```text
Main Flash

Sector 0    16 KB
Sector 1    16 KB
Sector 2    16 KB
Sector 3    16 KB
Sector 4    64 KB
Sector 5   128 KB
Sector 6   128 KB
Sector 7   128 KB
```

The complete organization is:

| Sector   | Start Address |  End Address |   Size |
| -------- | ------------: | -----------: | -----: |
| Sector 0 |  `0x08000000` | `0x08003FFF` |  16 KB |
| Sector 1 |  `0x08004000` | `0x08007FFF` |  16 KB |
| Sector 2 |  `0x08008000` | `0x0800BFFF` |  16 KB |
| Sector 3 |  `0x0800C000` | `0x0800FFFF` |  16 KB |
| Sector 4 |  `0x08010000` | `0x0801FFFF` |  64 KB |
| Sector 5 |  `0x08020000` | `0x0803FFFF` | 128 KB |
| Sector 6 |  `0x08040000` | `0x0805FFFF` | 128 KB |
| Sector 7 |  `0x08060000` | `0x0807FFFF` | 128 KB |

These sizes and addresses are specified in RM0368.

---

# 6. Why Are There Sectors?

A major difference between SRAM and Flash is how data is modified.

With SRAM, we can conceptually do:

```c
variable = 10;
```

and overwrite the previous value.

Flash does not work like normal SRAM.

Flash programming has physical limitations.

In particular, programming changes bits in a controlled direction, while returning bits to the erased state requires an **erase operation**.

Therefore Flash uses an erase/program architecture.

The STM32F401 Flash supports:

* Sector erase
* Mass erase
* Programming
* Write protection

---

# 7. Erase vs Program

These two operations must be clearly distinguished.

## Program

Programming writes data into Flash.

Conceptually:

```text
Flash
   |
   v
Program operation
   |
   v
Change selected bits
```

## Erase

Erase returns Flash cells to the erased state.

On STM32F401, erase can be performed at sector level or for the whole user Flash.

Therefore:

```text
Erase
  |
  +---- Sector erase
  |
  +---- Mass erase
```

The important point is:

> Writing new data to Flash is not equivalent to overwriting SRAM.

---

# 8. Flash Programming Is Not a Normal Memory Write

This is extremely important for bare-metal programming.

When you write:

```c
GPIOA->ODR = value;
```

you are performing a normal memory-mapped peripheral register access.

Flash programming is different.

The Flash controller must first be configured.

The basic conceptual sequence is:

```text
Check Flash status
       |
       v
Unlock Flash control
       |
       v
Configure programming
       |
       v
Program / erase
       |
       v
Wait until operation completes
       |
       v
Check status/error flags
```

The Flash control register is protected after reset and must be unlocked using a specific key sequence before programming/erase operations.

---

# 9. Flash Memory Interface

The STM32 does not connect the Cortex-M4 directly to the physical Flash cells.

There is a **Flash memory interface** between the CPU and Flash.

Conceptually:

```text
             Cortex-M4
                 |
        +--------+--------+
        |        |        |
     I-Code   D-Code    S-Bus
        |        |        |
        +--------+        |
                 |        |
                 v        v
          Flash Interface
                 |
        +--------+--------+
        |                 |
        v                 v
   Flash Memory      Flash Registers
```

The Flash interface is responsible for:

* Flash reads
* Flash programming
* Flash erase
* Protection
* Prefetch
* Instruction cache
* Data cache
* Flash access timing

This architecture is explicitly described in RM0368.

---

# 10. Instruction Fetch From Flash

This is where Flash becomes directly connected to CPU execution.

Suppose the CPU needs the instruction located at:

```text
0x08000020
```

The CPU does not say:

```text
"Give me instruction number 8 from Flash."
```

Instead, the CPU performs a memory access to an address.

Conceptually:

```text
CPU
 |
 | Address = 0x08000020
 v
Cortex-M4 bus interface
 |
 v
STM32 interconnect
 |
 v
Flash interface
 |
 v
Flash memory
 |
 v
Instruction data
 |
 v
CPU
```

The Flash interface handles CPU AHB I-Code and D-Code accesses to Flash.

---

# 11. Why Is Flash Addressed at 0x08000000?

The STM32F401RE maps its main Flash memory into the address space beginning at:

```text
0x08000000
```

Therefore:

```text
0x08000000 → Flash location 0
0x08000004 → Flash location +4
0x08000008 → Flash location +8
...
```

For example:

```text
Address              Content
------------------------------------------------
0x08000000           First word
0x08000004           Second word
0x08000008           Third word
0x0800000C           Fourth word
```

The CPU accesses these locations through normal memory addressing.

---

# 12. Boot Process and Flash

After reset, the Cortex-M4 needs to know where execution starts.

The STM32F401 boot mechanism can select different boot memories.

For normal application execution:

```text
BOOT configuration
        |
        v
Main Flash selected
        |
        v
Flash mapped into boot space
        |
        v
CPU obtains initial stack pointer
        |
        v
CPU obtains Reset_Handler address
        |
        v
Program starts
```

The STM32F401 reference manual describes that, after startup, the CPU fetches the top-of-stack value and then begins execution from the boot memory. Main Flash can be selected as the boot space.

---

# 13. Vector Table

The beginning of the application image normally contains the **vector table**.

Conceptually:

```text
Flash

0x08000000
+-----------------------------+
| Initial Stack Pointer       |
+-----------------------------+
| Reset_Handler address       |
+-----------------------------+
| NMI_Handler address         |
+-----------------------------+
| HardFault_Handler address   |
+-----------------------------+
| ...                         |
+-----------------------------+
```

The first entry provides the initial stack pointer.

The second entry provides the address of the reset handler.

Therefore Flash contains not only application code.

It also contains important startup information.

---

# 14. Why Does Flash Need Wait States?

This is one of the most important concepts in STM32 Flash.

The CPU can become very fast when HCLK increases.

However, Flash memory has a finite access time.

Imagine:

```text
CPU
 |
 | "I need an instruction."
 v
Flash
 |
 | "I need some time to provide it."
 v
Instruction
```

At low CPU frequencies, Flash can respond within the required timing.

At higher frequencies, one CPU clock cycle becomes shorter.

Eventually:

```text
CPU cycle < Flash access time
```

The CPU would need the data before Flash can safely provide it.

Therefore the Flash interface inserts **wait states**.

---

# 15. What Is a Wait State?

A wait state is an additional CPU cycle used to accommodate Flash access timing.

For example:

```text
0 Wait States

CPU:
|---- cycle ----|

Flash:
|---- access ----|
```

With one wait state:

```text
CPU:
|---- cycle ----|---- cycle ----|

Flash:
|---- access -------------------|
             ^
          wait state
```

Conceptually:

```text
More CPU speed
      |
      v
Shorter CPU cycle
      |
      v
Flash cannot respond within one cycle
      |
      v
Additional wait states
```

---

# 16. STM32F401RE Wait-State Configuration

The number of Flash wait states is controlled by:

```text
FLASH_ACR
```

specifically:

```text
LATENCY[3:0]
```

The STM32F401 reference manual defines:

```text
0000 -> 0 wait states
0001 -> 1 wait state
0010 -> 2 wait states
...
```

For a supply voltage of 2.7–3.6 V:

|       HCLK | Wait States |
| ---------: | ----------: |
|   0–30 MHz |        0 WS |
| >30–60 MHz |        1 WS |
| >60–84 MHz |        2 WS |

This is why Flash configuration cannot be considered independently from RCC/clock configuration.

---

# 17. Relationship Between RCC and Flash

Suppose the MCU starts with:

```text
HCLK = 16 MHz
```

At this frequency:

```text
0 WS
```

is sufficient for the 2.7–3.6 V range.

Now suppose we increase HCLK to:

```text
84 MHz
```

Then the Flash latency must be configured appropriately before increasing the CPU clock.

Conceptually:

```text
RCC
 |
 | Increase HCLK
 v
CPU becomes faster
 |
 v
Flash needs more access time
 |
 v
Configure FLASH_ACR LATENCY
```

ST recommends configuring the new Flash latency before increasing the CPU frequency.

This creates an important connection:

```text
RCC
 |
 | Clock frequency
 v
CPU
 |
 | Instruction fetch
 v
Flash Interface
 |
 | LATENCY
 v
Flash
```

---

# 18. FLASH_ACR

`FLASH_ACR` is the **Flash Access Control Register**.

Its address offset is:

```text
0x00
```

It controls Flash access timing and acceleration features.

Important fields are:

```text
FLASH_ACR

31                         13 12 11 10 9 8 7 4 3       0
+----------------------------+--+--+--+--+-+----+---------+
|          Reserved          |  |  |  |  | |Res.| LATENCY |
+----------------------------+--+--+--+--+-+----+---------+
                             |  |  |  |  |
                             |  |  |  |  +-- PRFTEN
                             |  |  |  +----- ICEN
                             |  |  +-------- DCEN
                             |  +----------- ICRST
                             +-------------- DCRST
```

Important bits:

| Bits | Field   | Purpose                        |
| ---- | ------- | ------------------------------ |
| 12   | DCRST   | Data cache reset               |
| 11   | ICRST   | Instruction cache reset        |
| 10   | DCEN    | Data cache enable              |
| 9    | ICEN    | Instruction cache enable       |
| 8    | PRFTEN  | Prefetch enable                |
| 3:0  | LATENCY | Flash wait-state configuration |

---

# 19. ART Accelerator

STM32F401 contains an **ART Accelerator**.

ART stands for:

```text
Adaptive Real-Time
```

Its purpose is to reduce the performance penalty caused by Flash access latency.

The Flash is relatively slow compared with the Cortex-M4 core at high clock frequencies.

The ART accelerator uses mechanisms such as:

* Instruction prefetch
* Instruction cache
* Branch cache

to reduce the amount of time the CPU has to wait for Flash.

Conceptually:

```text
Without acceleration:

CPU ---> Flash ---> CPU
          ^
        delay


With acceleration:

CPU ---> ART ---> instruction
          |
          +--> Prefetch
          |
          +--> Instruction cache
```

The accelerator does not mean that the physical Flash suddenly becomes faster.

Instead, it reduces the performance impact of Flash access.

---

# 20. Prefetch

Suppose the CPU is executing sequential instructions:

```text
Instruction 1
Instruction 2
Instruction 3
Instruction 4
Instruction 5
...
```

Instead of waiting for every instruction individually, the Flash interface can fetch the next instruction line in advance.

Conceptually:

```text
CPU is executing:

[1]

Meanwhile:

Flash ---> [2][3][4][5]
             ^
           prefetch
```

The next instructions can therefore already be available when the CPU needs them.

Prefetch is controlled by:

```text
FLASH_ACR.PRFTEN
```

The reference manual describes Flash reads as 128-bit lines and the prefetch mechanism for sequential instruction execution.

---

# 21. Instruction Cache

The STM32F401 Flash interface also provides instruction cache functionality.

The purpose is to keep recently fetched instruction lines available.

Conceptually:

```text
CPU
 |
 v
Instruction Cache
 |
 +---- Hit ----> Instruction available
 |
 +---- Miss ---> Flash
```

If the requested instruction is already in the cache:

```text
CPU ---> Cache ---> Instruction
```

If not:

```text
CPU ---> Cache ---> Flash
                    |
                    v
                  Data
                    |
                    v
                  Cache
                    |
                    v
                   CPU
```

The instruction cache is controlled by:

```text
FLASH_ACR.ICEN
```

The STM32F401 Flash interface provides 64 instruction-cache lines of 128 bits.

---

# 22. Data Cache

The Flash interface also provides a data cache.

This matters when data or literal pools are accessed from Flash.

It is controlled by:

```text
FLASH_ACR.DCEN
```

The STM32F401 Flash interface provides 8 data-cache lines of 128 bits.

For the beginning of bare-metal development, the most important acceleration concepts are:

```text
Prefetch
Instruction Cache
Data Cache
ART Accelerator
```

---

# 23. Flash Register Block

Flash itself has memory addresses such as:

```text
0x08000000
```

But the **Flash controller also has registers**.

This is an important distinction.

There are two different concepts:

```text
Flash Memory
    |
    +--> Stores program/data
    |
    +--> Address: 0x08000000 ...

Flash Interface Registers
    |
    +--> Control Flash hardware
    |
    +--> FLASH_ACR
    +--> FLASH_KEYR
    +--> FLASH_SR
    +--> FLASH_CR
    +--> ...
```

The CPU accesses the Flash controller through memory-mapped registers.

---

# 24. Important Flash Registers

The most important registers for bare-metal Flash programming are:

| Register        | Offset | Purpose                      |
| --------------- | -----: | ---------------------------- |
| `FLASH_ACR`     | `0x00` | Access timing/cache/prefetch |
| `FLASH_KEYR`    | `0x04` | Unlock key                   |
| `FLASH_OPTKEYR` | `0x08` | Option-byte unlock           |
| `FLASH_SR`      | `0x0C` | Status/errors                |
| `FLASH_CR`      | `0x10` | Control/program/erase        |

These offsets are specified in RM0368.

---

# 25. FLASH_SR — Status Register

`FLASH_SR` tells software what is happening inside the Flash controller.

One particularly important bit is:

```text
BSY
```

BSY means:

```text
Busy
```

Conceptually:

```text
FLASH_SR.BSY = 1
        |
        v
Flash operation ongoing


FLASH_SR.BSY = 0
        |
        v
Flash operation finished
```

The STM32F401 reference manual defines BSY as indicating whether a Flash operation is in progress.

---

# 26. Flash Programming Sequence

A simplified programming process looks like:

```text
             Start
               |
               v
       Check FLASH_SR.BSY
               |
               v
          Unlock Flash
               |
               v
       Configure FLASH_CR
               |
               v
        Program Flash
               |
               v
       Wait for BSY = 0
               |
               v
       Check error flags
               |
               v
              Done
```

The actual driver must follow the exact programming sequence specified by RM0368.

Do not treat Flash programming as:

```c
*(uint32_t *)address = data;
```

That is not sufficient to perform a valid STM32F401 Flash programming operation.

---

# 27. Flash Unlock Sequence

After reset, the Flash control register is locked.

To unlock it, STM32F401 requires two key values to be written consecutively to `FLASH_KEYR`:

```text
KEY1 = 0x45670123
KEY2 = 0xCDEF89AB
```

The purpose is to prevent accidental programming or erase operations.

An incorrect sequence causes the control register to remain locked until reset.

Conceptually:

```text
FLASH_CR
   |
   | locked
   v
FLASH_KEYR
   |
   | KEY1
   | KEY2
   v
FLASH_CR
   |
   | unlocked
   v
Program / Erase allowed
```

---

# 28. Why Does Flash Have Protection?

Flash often contains the firmware of a device.

If anyone could freely erase or modify it, the application could easily be destroyed.

STM32F401 therefore provides protection mechanisms such as:

* Read protection
* Write protection
* PCROP
* Option bytes

The reference manual describes RDP, write protection and PCROP as part of the Flash protection system.

For basic driver development, understand the existence and purpose of these mechanisms first.

Detailed protection configuration can be studied later.

---

# 29. Flash and `volatile`

Memory-mapped hardware registers are commonly declared using `volatile`.

For example, conceptually:

```c
volatile uint32_t *reg;
```

`volatile` tells the compiler that accesses to this memory location are observable and must not be optimized away as if the memory were an ordinary variable.

For example:

```c
while (FLASH_SR & BSY)
{
}
```

The value of `FLASH_SR` can change because of hardware activity.

Therefore the compiler must perform the required memory reads rather than assuming the value never changes.

Important:

`volatile` does **not** mean:

* atomic
* thread-safe
* synchronized
* faster
* protected from hardware races

It only affects compiler optimization/visibility of accesses.

---

# 30. Flash and the Linker Script

This is one of the most important connections in an embedded project.

The compiler generates sections such as:

```text
.text
.rodata
.data
.bss
```

These sections must be placed into physical memory.

The linker script determines where they are placed.

A typical arrangement is:

```text
Flash
0x08000000
     |
     +-------------------+
     | .isr_vector       |
     +-------------------+
     | .text             |
     +-------------------+
     | .rodata           |
     +-------------------+
     | initialization    |
     | data              |
     +-------------------+
     |
     v
```

SRAM:

```text
SRAM
0x20000000
     |
     +-------------------+
     | .data             |
     +-------------------+
     | .bss              |
     +-------------------+
     | heap              |
     +-------------------+
     |       ...         |
     |                   |
     | stack             |
     +-------------------+
```

This means the linker is directly connected to the hardware memory map.

---

# 31. `.text`

The `.text` section normally contains executable machine code.

For example:

```c
void foo(void)
{
    // code
}
```

The generated machine instructions for `foo()` normally reside in `.text`.

The linker places `.text` into Flash.

Conceptually:

```text
C function
    |
    v
Machine code
    |
    v
.text
    |
    v
Flash
```

Therefore:

```text
CPU instruction fetch
        |
        v
Flash
        |
        v
.text
```

---

# 32. `.rodata`

`.rodata` means **read-only data**.

Examples:

```c
const char message[] = "Hello";
const int table[] = {1, 2, 3, 4};
```

Such data can normally be placed in Flash because it does not need to change at runtime.

Conceptually:

```text
const data
    |
    v
.rodata
    |
    v
Flash
```

---

# 33. `.data`

This is a very important embedded concept.

Consider:

```c
int counter = 10;
```

`counter` needs to be writable at runtime.

Therefore its runtime location should be SRAM.

But the initial value `10` must exist somewhere when the MCU starts.

Therefore:

```text
Flash
   |
   | Initial value = 10
   v
Startup code
   |
   | copy
   v
SRAM
   |
   v
counter = 10
```

So `.data` has two relevant locations:

```text
Load Memory Address (LMA)
        |
        v
      Flash

Virtual/Runtime Memory Address (VMA)
        |
        v
      SRAM
```

The startup code copies the initialized data from Flash to SRAM.

This is one of the main reasons understanding Flash requires understanding the linker script and startup code.

---

# 34. `.bss`

Consider:

```c
int counter;
```

There is no explicit initial value.

The C language requires static-storage-duration variables such as this to start as zero.

The `.bss` section is normally placed in SRAM.

At startup:

```text
.bss in SRAM
      |
      v
Startup code
      |
      v
Set section to zero
```

Therefore:

```text
.data -> initialized values copied from Flash to SRAM

.bss  -> SRAM region cleared to zero
```

---

# 35. Stack

The stack is normally located in SRAM.

The CPU uses the stack for things such as:

* Function calls
* Local variables
* Saved registers
* Exception handling

The initial stack pointer value is stored in the vector table.

Conceptually:

```text
Flash
0x08000000
     |
     +--> Initial MSP value
     |
     +--> Reset_Handler
```

After reset:

```text
CPU
 |
 | Read initial MSP
 v
SP = SRAM address
```

Then the CPU can begin normal program execution.

---

# 36. Heap

The heap is also normally located in SRAM.

It is used for dynamic memory allocation, for example:

```c
malloc()
```

However, many embedded systems avoid dynamic allocation because deterministic memory behavior is often more important.

A typical SRAM layout can conceptually look like:

```text
Higher address
+-------------------+
| Stack             |
|       ↓           |
+-------------------+
|                   |
| Free space        |
|                   |
+-------------------+
|       ↑           |
| Heap              |
+-------------------+
| .bss              |
+-------------------+
| .data             |
+-------------------+
Lower address
```

The exact layout is determined by the linker script and startup/runtime environment.

---

# 37. Startup Code

Startup code executes before `main()`.

Its responsibilities typically include:

```text
Reset
 |
 v
Set stack pointer
 |
 v
Copy .data from Flash -> SRAM
 |
 v
Clear .bss
 |
 v
System initialization
 |
 v
Call main()
```

Conceptually:

```text
             RESET
               |
               v
        Reset_Handler
               |
       +-------+-------+
       |               |
       v               v
 Set Stack       Initialize memory
                       |
               +-------+-------+
               |               |
               v               v
        Flash -> .data     Clear .bss
               |               |
               +-------+-------+
                       |
                       v
                     main()
```

Therefore:

```text
Flash
  |
  +---- Program
  +---- Constants
  +---- Initial .data values
  |
  v
Startup code
  |
  v
SRAM
  |
  +---- .data
  +---- .bss
  +---- Stack
  +---- Heap
```

---

# 38. The Complete Hardware Picture

At this point, Flash can be connected to the concepts studied previously.

Consider instruction execution.

```text
                 STM32F401RE
                     |
                     v
                Cortex-M4
                     |
                 I-Code bus
                     |
                     v
                 Bus Matrix
                     |
                     v
             Flash Interface
                     |
          +----------+----------+
          |                     |
          v                     v
      ART / Cache          Flash Memory
                                |
                                v
                         0x08000000...
                                |
                                v
                           Machine Code
                                |
                                v
                            Cortex-M4
```

For Flash controller registers:

```text
Cortex-M4
    |
    | Address
    v
Bus / Interconnect
    |
    v
Flash Interface Registers
    |
    +--> FLASH_ACR
    +--> FLASH_KEYR
    +--> FLASH_SR
    +--> FLASH_CR
```

This is the same fundamental concept as GPIO:

```text
CPU
 |
 | address
 v
Bus / routing
 |
 v
Target
 |
 v
Register / Memory
```

The difference is that the target here is the Flash memory subsystem.

---

# 39. Flash Memory vs Flash Registers

Do not confuse these two:

### Flash memory

```text
0x08000000 ...
```

Contains:

* Program code
* Constants
* Stored application data

### Flash interface registers

Used to control the Flash hardware:

```text
FLASH_ACR
FLASH_KEYR
FLASH_SR
FLASH_CR
...
```

This distinction is critical when writing a bare-metal driver.

---

# 40. What Happens During Normal Program Execution?

A simplified sequence is:

```text
1. MCU reset
       |
       v
2. Boot configuration selects Flash
       |
       v
3. CPU obtains initial stack pointer
       |
       v
4. CPU obtains Reset_Handler
       |
       v
5. Startup code executes
       |
       v
6. .data copied from Flash to SRAM
       |
       v
7. .bss cleared
       |
       v
8. main() starts
       |
       v
9. CPU continuously fetches instructions
       |
       v
10. Instructions are obtained from Flash
    through the Flash interface
```

---

# 41. What Happens When Flash Is Programmed?

Programming is a different process:

```text
Application / Driver
        |
        v
Check FLASH_SR
        |
        v
Unlock FLASH_CR
        |
        v
Configure FLASH_CR
        |
        v
Erase sector if required
        |
        v
Program data
        |
        v
Wait for BSY = 0
        |
        v
Check errors
        |
        v
Flash contains new data
```

An important hardware restriction is that Flash read accesses can stall while a program/erase operation is in progress. Consequently, code/data fetches from Flash cannot proceed normally during that operation.

This becomes important when designing a Flash driver.

---

# 42. Flash Driver Perspective

When we eventually write a bare-metal Flash driver, we should not start by memorizing code.

We should derive the driver from the hardware.

The process will be:

```text
Reference Manual
       |
       v
Flash memory map
       |
       v
Flash register map
       |
       v
Register offsets
       |
       v
Bit fields
       |
       v
Required sequence
       |
       v
Driver API
       |
       v
Register-level implementation
```

For example:

```text
FLASH_ACR
    |
    +--> LATENCY
    +--> PRFTEN
    +--> ICEN
    +--> DCEN


FLASH_SR
    |
    +--> BSY
    +--> EOP
    +--> WRPERR
    +--> PGAERR
    +--> PGPERR
    +--> PGSERR


FLASH_CR
    |
    +--> PG
    +--> SER
    +--> SNB
    +--> PSIZE
    +--> STRT
    +--> LOCK
```

These fields will later become the basis of our driver.

---

# 43. Key Concepts to Remember

The following chain should be understood before writing the Flash driver:

```text
Flash
 |
 +-- Non-volatile memory
 |
 +-- Stores program code
 |
 +-- Stores read-only data
 |
 +-- Has sector-based organization
 |
 +-- Requires special erase/program operations
 |
 +-- Has a Flash memory interface
 |
 +-- Has memory-mapped control registers
 |
 +-- Requires wait-state configuration
 |
 +-- Uses ART / prefetch / cache mechanisms
 |
 +-- Interacts with RCC clock configuration
 |
 +-- Is connected to linker script
 |
 +-- Is connected to startup code
 |
 +-- Provides initial program image
```

---

# 44. The Most Important Mental Model

Do not memorize Flash as:

> "Flash is where the program is stored."

That is correct but incomplete.

Use this model instead:

```text
                 SOFTWARE
                     |
                     v
              Compiled Program
                     |
                     v
                Linker Script
                     |
                     v
                 Flash Image
                     |
                     v
          +----------------------+
          |   Flash Memory       |
          |                      |
          | .isr_vector          |
          | .text                |
          | .rodata              |
          | .data initial values |
          +----------------------+
                     |
                     v
             Flash Interface
                     |
          +----------+----------+
          |                     |
          v                     v
       I-Code                 D-Code
          |                     |
          +----------+----------+
                     |
                     v
                 Cortex-M4
                     |
                     v
               Instruction
                  Execute
```

The key idea is:

> **Flash is both a physical non-volatile memory and an active part of the MCU's instruction/data access architecture.**

---

# 45. What You Should Know Before Moving to the Driver

Before writing the Flash driver, you should be able to answer these questions:

1. Why does STM32F401RE need Flash?
2. Why is Flash different from SRAM?
3. What is the Flash start address?
4. What is the Flash end address on STM32F401RE?
5. What is a sector?
6. Why can't Flash be treated exactly like SRAM?
7. What is the difference between erase and program?
8. What is the Flash memory interface?
9. What is an instruction fetch?
10. Why does Flash need wait states?
11. What does `FLASH_ACR.LATENCY` control?
12. What is prefetch?
13. What is instruction cache?
14. What is ART Accelerator?
15. What is `FLASH_SR.BSY`?
16. Why does `FLASH_CR` need to be unlocked?
17. What is the difference between Flash memory and Flash registers?
18. Why is `.text` normally placed in Flash?
19. Why does `.data` have an initial copy in Flash but a runtime copy in SRAM?
20. What does startup code do before `main()`?
21. Where does the stack live?
22. Why is the linker script relevant to Flash?
23. What happens to Flash access during an erase/program operation?
24. How does RCC clock frequency affect Flash latency?

If these are understood, we can move from **Flash architecture → register map → bare-metal Flash driver**.

## Reference

Primary source for STM32F401RE Flash architecture and register behavior:

* STMicroelectronics, **RM0368 — STM32F401xB/C and STM32F401xD/E Reference Manual**, especially Sections 2 and 3.
