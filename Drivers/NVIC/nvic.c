#include "nvic.h"

void NVIC_EnableIRQ(uint8_t IRQn)
{
    /* each ISER register covers 32 IRQs -> divide by 32 to find the register,
     * modulo 32 to find the bit inside it */
    NVIC->ISER[IRQn >> 5] = (1UL << (IRQn & 0x1F));
}

void NVIC_DisableIRQ(uint8_t IRQn)
{
    NVIC->ICER[IRQn >> 5] = (1UL << (IRQn & 0x1F));
}

void NVIC_SetPriority(uint8_t IRQn, uint8_t priority)
{
    /* Cortex-M4 on F401 implements only the top 4 bits of each priority byte
     * (16 priority levels), so shift the value into place */
    NVIC->IP[IRQn] = (uint8_t)(priority << 4);
}
