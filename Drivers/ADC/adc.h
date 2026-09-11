#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/*
 * ADC Register Map
 *
 * ADC1 Base Address: 0x40012000
 *
 * +0x00 -> SR      (Status Register)
 * +0x04 -> CR1     (Control Register 1)
 * +0x08 -> CR2     (Control Register 2)
 * +0x0C -> SMPR1   (Sample Time Register 1, channels 10-18)
 * +0x10 -> SMPR2   (Sample Time Register 2, channels 0-9)
 * +0x14 -> JOFR1   (Injected Channel Data Offset Register 1)
 * +0x18 -> JOFR2
 * +0x1C -> JOFR3
 * +0x20 -> JOFR4
 * +0x24 -> HTR     (Watchdog Higher Threshold Register)
 * +0x28 -> LTR     (Watchdog Lower Threshold Register)
 * +0x2C -> SQR1    (Regular Sequence Register 1)
 * +0x30 -> SQR2    (Regular Sequence Register 2)
 * +0x34 -> SQR3    (Regular Sequence Register 3)
 * +0x38 -> JSQR    (Injected Sequence Register)
 * +0x3C -> JDR1    (Injected Data Register 1)
 * +0x40 -> JDR2
 * +0x44 -> JDR3
 * +0x48 -> JDR4
 * +0x4C -> DR      (Regular Data Register)
 */
typedef struct {
    volatile uint32_t SR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMPR1;
    volatile uint32_t SMPR2;
    volatile uint32_t JOFR1;
    volatile uint32_t JOFR2;
    volatile uint32_t JOFR3;
    volatile uint32_t JOFR4;
    volatile uint32_t HTR;
    volatile uint32_t LTR;
    volatile uint32_t SQR1;
    volatile uint32_t SQR2;
    volatile uint32_t SQR3;
    volatile uint32_t JSQR;
    volatile uint32_t JDR1;
    volatile uint32_t JDR2;
    volatile uint32_t JDR3;
    volatile uint32_t JDR4;
    volatile uint32_t DR;
} ADC_TypeDef;

#define ADC1_BASE 0x40012000UL
#define ADC1 ((ADC_TypeDef *)ADC1_BASE)

/*
 * ADC Common Register Map (shared prescaler/status, only relevant part on F401)
 * ADC_Common Base Address: 0x40012300
 *
 * +0x04 -> CCR  (Common Control Register)
 */
typedef struct {
    volatile uint32_t CSR;
    volatile uint32_t CCR;
} ADC_Common_TypeDef;

#define ADC_COMMON_BASE 0x40012300UL
#define ADC_COMMON ((ADC_Common_TypeDef *)ADC_COMMON_BASE)

typedef enum {
    ADC_SAMPLETIME_3CYCLES   = 0x00,
    ADC_SAMPLETIME_15CYCLES  = 0x01,
    ADC_SAMPLETIME_28CYCLES  = 0x02,
    ADC_SAMPLETIME_56CYCLES  = 0x03,
    ADC_SAMPLETIME_84CYCLES  = 0x04,
    ADC_SAMPLETIME_112CYCLES = 0x05,
    ADC_SAMPLETIME_144CYCLES = 0x06,
    ADC_SAMPLETIME_480CYCLES = 0x07
} ADC_SampleTime_t;

typedef enum {
    ADC_RESOLUTION_12BIT = 0x00,
    ADC_RESOLUTION_10BIT = 0x01,
    ADC_RESOLUTION_8BIT  = 0x02,
    ADC_RESOLUTION_6BIT  = 0x03
} ADC_Resolution_t;

typedef struct {
    ADC_Resolution_t  resolution;
    uint8_t           continuousMode; // 1 = keep converting automatically, 0 = single conversion
} ADC_Config_t;

/* ==================== BASIC ADC FUNCTIONS ==================== */
void ADC_Init(ADC_TypeDef *adc, ADC_Config_t *config);
void ADC_Enable(ADC_TypeDef *adc);
void ADC_Disable(ADC_TypeDef *adc);
void ADC_ConfigChannel(ADC_TypeDef *adc, uint8_t channel, ADC_SampleTime_t sampleTime);
void ADC_StartConversion(ADC_TypeDef *adc);
uint8_t ADC_IsConversionComplete(ADC_TypeDef *adc);
uint16_t ADC_ReadValue(ADC_TypeDef *adc);
uint16_t ADC_ReadChannel(ADC_TypeDef *adc, uint8_t channel, ADC_SampleTime_t sampleTime);

#endif
