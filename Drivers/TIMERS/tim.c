#include "tim.h"

void TIM_Init(TIM_TypeDef *tim, TIM_Config_t *config)
{
    tim->PSC = config->prescaler;
    tim->ARR = config->period;

    /* force update event so PSC/ARR are latched into the shadow registers immediately */
    tim->EGR |= (1UL << 0); // UG bit
}

void TIM_Start(TIM_TypeDef *tim)
{
    tim->CR1 |= (1UL << 0); // CEN bit: counter enable
}

void TIM_Stop(TIM_TypeDef *tim)
{
    tim->CR1 &= ~(1UL << 0); // CEN bit: counter disable
}

uint8_t TIM_GetUIFFlag(TIM_TypeDef *tim)
{
    return (uint8_t)((tim->SR >> 0) & 0x1UL); // UIF bit: update interrupt flag
}

void TIM_ClearUIFFlag(TIM_TypeDef *tim)
{
    tim->SR &= ~(1UL << 0); // write 0 to clear (unlike EXTI's PR, this is NOT write-1-to-clear)
}

void TIM_DelayMs(TIM_TypeDef *tim, uint32_t ms)
{
    /* Assumes APB1 timer clock = 84 MHz (default F401 clock tree via CMSIS SystemInit)
     * PSC = 83999 -> counter clock = 84MHz / (83999+1) = 1 kHz -> 1 tick = 1 ms */
    TIM_Config_t cfg = {
        .prescaler = 83999,
        .period    = ms
    };

    TIM_Init(tim, &cfg);
    TIM_ClearUIFFlag(tim);
    TIM_Start(tim);

    while (!TIM_GetUIFFlag(tim)) {
        // busy-wait until the counter overflows once
    }

    TIM_Stop(tim);
    TIM_ClearUIFFlag(tim);
}

/* ==================== PWM ==================== */
void TIM_PWM_Init(TIM_TypeDef *tim, TIM_PWM_Config_t *config)
{
    tim->PSC = config->prescaler;
    tim->ARR = config->period;

    /* Each CCMR register holds settings for 2 channels (8 bits per channel).
     * CCMR1 -> channels 1-2, CCMR2 -> channels 3-4.
     * OCxM bits (bits 6:4 within each channel's 8-bit field) = 110 -> PWM Mode 1
     * OCxPE bit (bit 3) = 1 -> enable preload register (needed for smooth duty updates) */
    switch (config->channel)
    {
        case TIM_CHANNEL_1:
            tim->CCMR1 &= ~(0x7UL << 4);       // clear OC1M
            tim->CCMR1 |= (0x6UL << 4);        // OC1M = 110 (PWM Mode 1)
            tim->CCMR1 |= (1UL << 3);          // OC1PE = 1 (preload enable)
            tim->CCER  |= (1UL << 0);          // CC1E = 1 (enable channel 1 output)
            break;
        case TIM_CHANNEL_2:
            tim->CCMR1 &= ~(0x7UL << 12);
            tim->CCMR1 |= (0x6UL << 12);       // OC2M
            tim->CCMR1 |= (1UL << 11);         // OC2PE
            tim->CCER  |= (1UL << 4);          // CC2E
            break;
        case TIM_CHANNEL_3:
            tim->CCMR2 &= ~(0x7UL << 4);
            tim->CCMR2 |= (0x6UL << 4);        // OC3M
            tim->CCMR2 |= (1UL << 3);          // OC3PE
            tim->CCER  |= (1UL << 8);          // CC3E
            break;
        case TIM_CHANNEL_4:
            tim->CCMR2 &= ~(0x7UL << 12);
            tim->CCMR2 |= (0x6UL << 12);       // OC4M
            tim->CCMR2 |= (1UL << 11);         // OC4PE
            tim->CCER  |= (1UL << 12);         // CC4E
            break;
    }

    /* ARPE bit in CR1: enable auto-reload preload, keeps ARR updates glitch-free */
    tim->CR1 |= (1UL << 7);

    /* force an update event so PSC/ARR/CCMR settings are latched immediately */
    tim->EGR |= (1UL << 0);
}

void TIM_PWM_SetDutyCycle(TIM_TypeDef *tim, TIM_Channel_t channel, uint32_t compareValue)
{
    switch (channel)
    {
        case TIM_CHANNEL_1: tim->CCR1 = compareValue; break;
        case TIM_CHANNEL_2: tim->CCR2 = compareValue; break;
        case TIM_CHANNEL_3: tim->CCR3 = compareValue; break;
        case TIM_CHANNEL_4: tim->CCR4 = compareValue; break;
    }
}

void TIM_PWM_Start(TIM_TypeDef *tim)
{
    tim->CR1 |= (1UL << 0); // CEN bit: start the counter
}
