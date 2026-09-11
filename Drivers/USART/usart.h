#ifndef USART_H
#define USART_H

#include <stdint.h>

/*
 * USART Register Map
 *
 * USART1 Base Address: 0x40011000  (APB2 bus)
 * USART2 Base Address: 0x40004400  (APB1 bus)
 * USART6 Base Address: 0x40011400  (APB2 bus)
 *
 * +0x00 -> SR    (Status Register)
 * +0x04 -> DR    (Data Register)
 * +0x08 -> BRR   (Baud Rate Register)
 * +0x0C -> CR1   (Control Register 1)
 * +0x10 -> CR2   (Control Register 2)
 * +0x14 -> CR3   (Control Register 3)
 * +0x18 -> GTPR  (Guard Time and Prescaler Register)
 */
typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

#define USART1_BASE 0x40011000UL
#define USART2_BASE 0x40004400UL
#define USART6_BASE 0x40011400UL

#define USART1 ((USART_TypeDef *)USART1_BASE)
#define USART2 ((USART_TypeDef *)USART2_BASE)
#define USART6 ((USART_TypeDef *)USART6_BASE)

typedef enum {
    USART_WORDLENGTH_8B = 0x00,
    USART_WORDLENGTH_9B = 0x01
} USART_WordLength_t;

typedef enum {
    USART_STOPBITS_1 = 0x00,
    USART_STOPBITS_2 = 0x02   // matches CR2 STOP[1:0] bit pattern
} USART_StopBits_t;

typedef enum {
    USART_PARITY_NONE = 0x00,
    USART_PARITY_EVEN = 0x01,
    USART_PARITY_ODD  = 0x03  // PCE bit + PS bit combined for convenience
} USART_Parity_t;

typedef struct {
    uint32_t             baudrate;    // e.g. 9600, 115200
    USART_WordLength_t   wordLength;
    USART_StopBits_t     stopBits;
    USART_Parity_t       parity;
} USART_Config_t;

void USART_Init(USART_TypeDef *usart, USART_Config_t *config, uint32_t peripheralClockHz);
void USART_Transmit(USART_TypeDef *usart, uint8_t data);
uint8_t USART_Receive(USART_TypeDef *usart);
uint8_t USART_IsTxEmpty(USART_TypeDef *usart);
uint8_t USART_IsRxNotEmpty(USART_TypeDef *usart);
void USART_SendString(USART_TypeDef *usart, const char *str);

#endif
