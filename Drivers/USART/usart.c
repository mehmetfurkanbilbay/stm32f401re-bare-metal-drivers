#include "usart.h"

void USART_Init(USART_TypeDef *usart, USART_Config_t *config, uint32_t peripheralClockHz)
{
    /* 1) Word length: CR1 bit 12 (M bit). 0 = 8 bits, 1 = 9 bits */
    usart->CR1 &= ~(1UL << 12);
    usart->CR1 |= ((uint32_t)config->wordLength << 12);

    /* 2) Parity: CR1 bit 10 (PCE - parity control enable), bit 9 (PS - parity selection) */
    usart->CR1 &= ~(0x3UL << 9);
    if (config->parity == USART_PARITY_EVEN)
        usart->CR1 |= (1UL << 10);              // PCE = 1, PS = 0 (even)
    else if (config->parity == USART_PARITY_ODD)
        usart->CR1 |= (1UL << 10) | (1UL << 9);  // PCE = 1, PS = 1 (odd)
    // USART_PARITY_NONE -> both bits stay cleared

    /* 3) Stop bits: CR2 bits 13:12 */
    usart->CR2 &= ~(0x3UL << 12);
    usart->CR2 |= ((uint32_t)config->stopBits << 12);

    /* 4) Baud rate: BRR = peripheral_clock / baudrate, split into mantissa (bits 15:4)
     * and fraction (bits 3:0). Using integer division here (no fractional rounding). */
    uint32_t usartdiv = peripheralClockHz / config->baudrate;
    usart->BRR = usartdiv;

    /* 5) Enable transmitter, receiver, and the USART peripheral itself */
    usart->CR1 |= (1UL << 3);  // TE  (transmitter enable)
    usart->CR1 |= (1UL << 2);  // RE  (receiver enable)
    usart->CR1 |= (1UL << 13); // UE  (USART enable)
}

void USART_Transmit(USART_TypeDef *usart, uint8_t data)
{
    while (!USART_IsTxEmpty(usart)) {
        // wait until the data register is free (TXE flag set)
    }
    usart->DR = data;
}

uint8_t USART_Receive(USART_TypeDef *usart)
{
    while (!USART_IsRxNotEmpty(usart)) {
        // wait until a byte has arrived (RXNE flag set)
    }
    return (uint8_t)(usart->DR & 0xFF);
}

uint8_t USART_IsTxEmpty(USART_TypeDef *usart)
{
    return (uint8_t)((usart->SR >> 7) & 0x1UL); // TXE bit
}

uint8_t USART_IsRxNotEmpty(USART_TypeDef *usart)
{
    return (uint8_t)((usart->SR >> 5) & 0x1UL); // RXNE bit
}

void USART_SendString(USART_TypeDef *usart, const char *str)
{
    while (*str != '\0')
    {
        USART_Transmit(usart, (uint8_t)*str);
        str++;
    }
}
