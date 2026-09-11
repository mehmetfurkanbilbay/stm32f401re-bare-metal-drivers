#include "rcc.h"

void RCC_GPIO_ClockEnable(GPIO_TypeDef *port){
	if (port == GPIOA)
		RCC->AHB1ENR |= (1UL << RCC_AHB1ENR_GPIOAEN_Pos);
	else if (port == GPIOB)
		RCC->AHB1ENR |= (1UL << RCC_AHB1ENR_GPIOBEN_Pos);
	else if (port == GPIOC)
			RCC->AHB1ENR |= (1UL << RCC_AHB1ENR_GPIOCEN_Pos);
	else if (port == GPIOD)
			RCC->AHB1ENR |= (1UL << RCC_AHB1ENR_GPIODEN_Pos);
	else if (port == GPIOE)
			RCC->AHB1ENR |= (1UL << RCC_AHB1ENR_GPIOEEN_Pos);
	else if (port == GPIOH)
			RCC->AHB1ENR |= (1UL << RCC_AHB1ENR_GPIOHEN_Pos);
}

void RCC_TIM_ClockEnable(TIM_TypeDef *tim)
{
    if (tim == TIM2)
        RCC->APB1ENR |= (1UL << RCC_APB1ENR_TIM2EN_Pos);
    else if (tim == TIM3)
        RCC->APB1ENR |= (1UL << RCC_APB1ENR_TIM3EN_Pos);
    else if (tim == TIM4)
        RCC->APB1ENR |= (1UL << RCC_APB1ENR_TIM4EN_Pos);
    else if (tim == TIM5)
        RCC->APB1ENR |= (1UL << RCC_APB1ENR_TIM5EN_Pos);
}


void RCC_USART_ClockEnable(USART_TypeDef *usart)
{
    if (usart == USART1)
        RCC->APB2ENR |= (1UL << RCC_APB2ENR_USART1EN_Pos);
    else if (usart == USART6)
        RCC->APB2ENR |= (1UL << RCC_APB2ENR_USART6EN_Pos);
    else if (usart == USART2)
        RCC->APB1ENR |= (1UL << RCC_APB1ENR_USART2EN_Pos);
}

/* ==================== SPI CLOCK ENABLE ==================== */
void RCC_SPI_ClockEnable(SPI_TypeDef *spi)
{
    if (spi == SPI1)
        RCC->APB2ENR |= (1UL << RCC_APB2ENR_SPI1EN_Pos);
    else if (spi == SPI2)
        RCC->APB1ENR |= (1UL << RCC_APB1ENR_SPI2EN_Pos);
    else if (spi == SPI3)
        RCC->APB1ENR |= (1UL << RCC_APB1ENR_SPI3EN_Pos);
}

/* ==================== I2C CLOCK ENABLE ==================== */
void RCC_I2C_ClockEnable(I2C_TypeDef *i2c)
{
    if (i2c == I2C1)
        RCC->APB1ENR |= (1UL << RCC_APB1ENR_I2C1EN_Pos);
    else if (i2c == I2C2)
        RCC->APB1ENR |= (1UL << RCC_APB1ENR_I2C2EN_Pos);
    else if (i2c == I2C3)
        RCC->APB1ENR |= (1UL << RCC_APB1ENR_I2C3EN_Pos);
}

/* ==================== ADC CLOCK ENABLE ==================== */
void RCC_ADC_ClockEnable(ADC_TypeDef *adc)
{
    if (adc == ADC1)
        RCC->APB2ENR |= (1UL << RCC_APB2ENR_ADC1EN_Pos);
}


/* ==================== DMA CLOCK ENABLE ==================== */
void RCC_DMA_ClockEnable(DMA_TypeDef *dma)
{
    if (dma == DMA1)
        RCC->AHB1ENR |= (1UL << RCC_AHB1ENR_DMA1EN_Pos);
    else if (dma == DMA2)
        RCC->AHB1ENR |= (1UL << RCC_AHB1ENR_DMA2EN_Pos);
}

/* ==================== RTC CLOCK ENABLE ==================== */
void RCC_RTC_ClockEnable(void)
{
    /* 1) PWR peripheral clock must be on before touching PWR registers */
    RCC->APB1ENR |= (1UL << 28); // PWREN bit

    /* 2) Unlock backup domain (RTC/BDCR) write access */
    PWR_EnableBackupAccess();

    /* 3) Start the external low-speed oscillator (LSE, 32.768kHz) */
    RCC->BDCR |= (1UL << RCC_BDCR_LSEON_Pos);
    while (!(RCC->BDCR & (1UL << RCC_BDCR_LSERDY_Pos))) {
        // wait until LSE is stable (LSERDY flag)
    }

    /* 4) Select LSE as the RTC clock source (RTCSEL = 01) */
    RCC->BDCR &= ~(0x3UL << RCC_BDCR_RTCSEL_Pos);
    RCC->BDCR |= (0x1UL << RCC_BDCR_RTCSEL_Pos);

    /* 5) Enable the RTC peripheral clock itself */
    RCC->BDCR |= (1UL << RCC_BDCR_RTCEN_Pos);
}


