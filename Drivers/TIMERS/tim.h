#ifndef TIM_H
#define TIM_H

#include <stdint.h>

/*
 * TIM Register Map (General-purpose timers TIM2-TIM5)
 *
 * TIM2 Base Address: 0x40000000
 * TIM3 Base Address: 0x40000400
 * TIM4 Base Address: 0x40000800
 * TIM5 Base Address: 0x40000C00
 *
 * +0x00 CR1    - Control Register 1
 * +0x04 CR2    - Control Register 2
 * +0x08 SMCR   - Slave Mode Control Register
 * +0x0C DIER   - DMA/Interrupt Enable Register
 * +0x10 SR     - Status Register
 * +0x14 EGR    - Event Generation Register
 * +0x18 CCMR1  - Capture/Compare Mode Register 1
 * +0x1C CCMR2  - Capture/Compare Mode Register 2
 * +0x20 CCER   - Capture/Compare Enable Register
 * +0x24 CNT    - Counter
 * +0x28 PSC    - Prescaler
 * +0x2C ARR    - Auto-Reload Register
 * +0x30 RESERVED (RCR only exists on advanced timers, not here)
 * +0x34 CCR1   - Capture/Compare Register 1
 * +0x38 CCR2   - Capture/Compare Register 2
 * +0x3C CCR3   - Capture/Compare Register 3
 * +0x40 CCR4   - Capture/Compare Register 4
 * +0x44 RESERVED
 * +0x48 DCR    - DMA Control Register
 * +0x4C DMAR   - DMA Address for Full Transfer
 */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    uint32_t RESERVED0;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    uint32_t RESERVED1;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIM_TypeDef;

#define TIM2_BASE 0x40000000UL
#define TIM3_BASE 0x40000400UL
#define TIM4_BASE 0x40000800UL
#define TIM5_BASE 0x40000C00UL

#define TIM2 ((TIM_TypeDef *)TIM2_BASE)
#define TIM3 ((TIM_TypeDef *)TIM3_BASE)
#define TIM4 ((TIM_TypeDef *)TIM4_BASE)
#define TIM5 ((TIM_TypeDef *)TIM5_BASE)

/* Timer basic config, used for delay generation or general counting */
typedef struct {
    uint16_t prescaler;   // PSC value
    uint32_t period;      // ARR value
} TIM_Config_t;

/* ==================== PWM ==================== */
/* PWM channel selection — TIM2-5 each have 4 channels (CH1-CH4) */
typedef enum {
    TIM_CHANNEL_1 = 0,
    TIM_CHANNEL_2 = 1,
    TIM_CHANNEL_3 = 2,
    TIM_CHANNEL_4 = 3
} TIM_Channel_t;

typedef struct {
    uint16_t       prescaler;   // PSC value
    uint32_t       period;      // ARR value (defines PWM frequency)
    TIM_Channel_t  channel;     // which channel to configure
} TIM_PWM_Config_t;

void TIM_PWM_Init(TIM_TypeDef *tim, TIM_PWM_Config_t *config);
void TIM_PWM_SetDutyCycle(TIM_TypeDef *tim, TIM_Channel_t channel, uint32_t compareValue);
void TIM_PWM_Start(TIM_TypeDef *tim);

/* ==================== BASIC TIMER FUNCTIONS ==================== */
void TIM_Init(TIM_TypeDef *tim, TIM_Config_t *config);
void TIM_Start(TIM_TypeDef *tim);
void TIM_Stop(TIM_TypeDef *tim);
uint8_t TIM_GetUIFFlag(TIM_TypeDef *tim);
void TIM_ClearUIFFlag(TIM_TypeDef *tim);
void TIM_DelayMs(TIM_TypeDef *tim, uint32_t ms);

#endif
