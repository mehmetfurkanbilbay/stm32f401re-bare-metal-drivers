#include "exti.h"
#include "rcc.h"

/* Translate a GPIO port pointer to the port code EXTICR expects (A=0, B=1, ...) */
static uint8_t EXTI_GetPortCode(GPIO_TypeDef *port)
{
    if (port == GPIOA) return 0x0;
    if (port == GPIOB) return 0x1;
    if (port == GPIOC) return 0x2;
    if (port == GPIOD) return 0x3;
    if (port == GPIOE) return 0x4;
    if (port == GPIOH) return 0x7;
    return 0x0;
}

void EXTI_Init(EXTI_Config_t *config)
{
    uint8_t line      = config->pin;
    uint8_t portCode  = EXTI_GetPortCode(config->port);

    /* 1) SYSCFG clock must be on before touching EXTICR registers */
    RCC->APB2ENR |= (1UL << 14); // bit 14 = SYSCFGEN

    /* 2) route this EXTI line to the chosen GPIO port
     *    each EXTICR register holds 4 lines, 4 bits per line */
    uint8_t regIndex  = line / 4;
    uint8_t bitOffset = (line % 4) * 4;
    SYSCFG->EXTICR[regIndex] &= ~(0xF << bitOffset);
    SYSCFG->EXTICR[regIndex] |= (portCode << bitOffset);

    /* 3) configure which edge triggers the interrupt */
    switch (config->trigger)
    {
        case EXTI_TRIGGER_RISING:
            EXTI->RTSR |= (1UL << line);
            EXTI->FTSR &= ~(1UL << line);
            break;
        case EXTI_TRIGGER_FALLING:
            EXTI->FTSR |= (1UL << line);
            EXTI->RTSR &= ~(1UL << line);
            break;
        case EXTI_TRIGGER_BOTH:
            EXTI->RTSR |= (1UL << line);
            EXTI->FTSR |= (1UL << line);
            break;
    }

    /* 4) unmask the line so it can actually generate an interrupt */
    EXTI->IMR |= (1UL << line);
}

void EXTI_ClearPending(uint8_t line)
{
    EXTI->PR = (1UL << line); // this bit is write-1-to-clear
}

uint8_t EXTI_GetPending(uint8_t line)
{
    return (uint8_t)((EXTI->PR >> line) & 0x1UL);
}
