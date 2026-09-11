#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>

/*
 * NVIC (Nested Vectored Interrupt Controller) - ARM Cortex-M4 core peripheral
 * Fixed base address defined by ARM, not ST
 *
 * ISER[8] 0xE000E100 -> Interrupt Set-Enable Registers (32 IRQs each, 8 regs = 256 IRQs)
 * ICER[8] 0xE000E180 -> Interrupt Clear-Enable Registers
 * ISPR[8] 0xE000E200 -> Interrupt Set-Pending Registers
 * ICPR[8] 0xE000E280 -> Interrupt Clear-Pending Registers
 * IABR[8] 0xE000E300 -> Interrupt Active Bit Registers
 * IP[240] 0xE000E400 -> Interrupt Priority Registers, byte-accessible, 1 byte per IRQ
 */

typedef struct {
	volatile uint32_t ISER[8];
	uint32_t RESERVED0[24];
	volatile uint32_t ICER[8];
	uint32_t RESERVED1[24];
	volatile uint32_t ISPR[8];
	uint32_t RESERVED2[24];
	volatile uint32_t ICPR[8];
	uint32_t RESERVED3[24];
	volatile uint32_t IABR[8];
	uint32_t RESERVED4[56];
	volatile uint32_t IP[240];
}NVIC_TypeDef;

#define NVIC_BASE 0xE000E100UL
#define NVIC ((NVIC_TypeDef *)NVIC_BASE)

/* IRQ numbers used by EXTI lines on STM32F401 (see vector table in RM0368) */
#define EXTI0_IRQn      6
#define EXTI1_IRQn      7
#define EXTI2_IRQn      8
#define EXTI3_IRQn      9
#define EXTI4_IRQn      10
#define EXTI9_5_IRQn    23   // shared handler for lines 5-9
#define EXTI15_10_IRQn  40   // shared handler for lines 10-15

void NVIC_EnableIRQ(uint8_t IRQn);
void NVIC_DisableIRQ(uint8_t IRQn);
void NVIC_SetPriority(uint8_t IRQn, uint8_t priority);

#endif

