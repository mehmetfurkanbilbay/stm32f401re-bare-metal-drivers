#ifndef IWDG_H
#define IWDG_H

#include <stdint.h>

/*
 * IWDG Register Map
 *
 * IWDG Base Address: 0x40003000
 *
 * +0x00 -> KR    (Key Register — write-only, controls access)
 * +0x04 -> PR    (Prescaler Register)
 * +0x08 -> RLR   (Reload Register)
 * +0x0C -> SR    (Status Register)
 */
typedef struct {
    volatile uint32_t KR;
    volatile uint32_t PR;
    volatile uint32_t RLR;
    volatile uint32_t SR;
} IWDG_TypeDef;

#define IWDG_BASE 0x40003000UL
#define IWDG ((IWDG_TypeDef *)IWDG_BASE)

typedef enum {
    IWDG_PRESCALER_4   = 0x00,
    IWDG_PRESCALER_8   = 0x01,
    IWDG_PRESCALER_16  = 0x02,
    IWDG_PRESCALER_32  = 0x03,
    IWDG_PRESCALER_64  = 0x04,
    IWDG_PRESCALER_128 = 0x05,
    IWDG_PRESCALER_256 = 0x06
} IWDG_Prescaler_t;

/* ==================== BASIC IWDG FUNCTIONS ==================== */
void IWDG_Init(IWDG_Prescaler_t prescaler, uint16_t reloadValue);
void IWDG_Start(void);
void IWDG_Reload(void);

#endif
