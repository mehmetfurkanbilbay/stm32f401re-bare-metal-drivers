#ifndef RCC_H
#define RCC_H

#include <stdint.h>

#include "gpio.h"
#include "tim.h"
#include "usart.h"
#include "spi.h"
#include "i2c.h"
#include "adc.h"
#include "dma.h"
#include "pwr.h"

/*
 * RCC Register Map
 *
 * RCC Base Address: 0x40023800
 *
 * +0x00 -> CR				(Clock Control Register)
 * +0x04 -> PLLCFGR			(PLL Configuration Register)
 * +0x08 -> CFGR			(Clock Configuration Register)
 * +0x0C -> CIR				(Clock Interrupt Register)
 * +0x10 -> AHB1RSTR		(AHB1 Peripheral Reset Register)
 * +0x14 -> AHB2RSTR		(AHB2 Peripheral Reset Register)
 * +0x20 -> APB1RSTR		(APB1 Peripheral Reset Register)
 * +0x24 -> APB2RSTR		(ApB2 Peripheral Reset Register)
 * +0x30 -> AHB1ENR			(AHB1 peripheral clock enable register)
 * +0x34 -> AHB2ENR			(AHB2 peripheral clock enable register)
 * +0x40 -> APB1ENR			(APB1 peripheral clock enable register)
 * +0x44 -> APB2ENR			(APB2 peripheral clock enable register)
 */

typedef struct {
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	uint32_t RESERVED0[2];		// 0x18 - 0x1C
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	uint32_t RESERVED1[2];		// 0x28 - 0x2C
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	uint32_t RESERVED2[2];		// 0x38 - 0x3C
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	uint32_t RESERVED3[10];	// 0x48 - 0x6C (LPENR registers, not used here)
	volatile uint32_t BDCR;	// 0x70 - Backup Domain Control Register
	volatile uint32_t CSR;	// 0x74 - Control/Status Register
}RCC_TypeDef;


#define RCC_BASE 0x40023800UL
#define RCC ((RCC_TypeDef *)RCC_BASE)

/* ==================== GPIO CLOCK ENABLE ==================== */
/* AHB1ENR bit positions */
#define RCC_AHB1ENR_GPIOAEN_Pos 0
#define RCC_AHB1ENR_GPIOBEN_Pos 1
#define RCC_AHB1ENR_GPIOCEN_Pos 2
#define RCC_AHB1ENR_GPIODEN_Pos 3
#define RCC_AHB1ENR_GPIOEEN_Pos 4
#define RCC_AHB1ENR_GPIOHEN_Pos 7

/* Function prototype */
void RCC_GPIO_ClockEnable(GPIO_TypeDef *port);

/* ==================== TIMER CLOCK ENABLE ==================== */
/* APB1ENR bit positions for general-purpose timers */
#define RCC_APB1ENR_TIM2EN_Pos 0
#define RCC_APB1ENR_TIM3EN_Pos 1
#define RCC_APB1ENR_TIM4EN_Pos 2
#define RCC_APB1ENR_TIM5EN_Pos 3

void RCC_TIM_ClockEnable(TIM_TypeDef *tim);

/* ==================== USART CLOCK ENABLE ==================== */
#define RCC_APB2ENR_USART1EN_Pos 4
#define RCC_APB2ENR_USART6EN_Pos 5
#define RCC_APB1ENR_USART2EN_Pos 17

void RCC_USART_ClockEnable(USART_TypeDef *usart);

/* ==================== SPI CLOCK ENABLE ==================== */
#define RCC_APB2ENR_SPI1EN_Pos  12
#define RCC_APB1ENR_SPI2EN_Pos  14
#define RCC_APB1ENR_SPI3EN_Pos  15

void RCC_SPI_ClockEnable(SPI_TypeDef *spi);

/* ==================== I2C CLOCK ENABLE ==================== */
#define RCC_APB1ENR_I2C1EN_Pos  21
#define RCC_APB1ENR_I2C2EN_Pos  22
#define RCC_APB1ENR_I2C3EN_Pos  23

void RCC_I2C_ClockEnable(I2C_TypeDef *i2c);

/* ==================== ADC CLOCK ENABLE ==================== */
#define RCC_APB2ENR_ADC1EN_Pos  8

void RCC_ADC_ClockEnable(ADC_TypeDef *adc);


/* ==================== DMA CLOCK ENABLE ==================== */
#define RCC_AHB1ENR_DMA1EN_Pos  21
#define RCC_AHB1ENR_DMA2EN_Pos  22

void RCC_DMA_ClockEnable(DMA_TypeDef *dma);


/* ==================== RTC CLOCK ENABLE ==================== */
/* BDCR bit positions */
#define RCC_BDCR_LSEON_Pos   0
#define RCC_BDCR_LSERDY_Pos  1
#define RCC_BDCR_RTCSEL_Pos  8   // 2 bits: 01 = LSE selected as RTC clock source
#define RCC_BDCR_RTCEN_Pos   15

void RCC_RTC_ClockEnable(void);

#endif




