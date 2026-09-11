#include "adc.h"

void ADC_Init(ADC_TypeDef *adc, ADC_Config_t *config)
{
    /* Common prescaler: ADCCLK = PCLK2 / prescaler. We fix /4 here (safe default,
     * ADCCLK must stay <= 36MHz on F401). CCR bits 17:16 = ADCPRE */
    ADC_COMMON->CCR &= ~(0x3UL << 16);
    ADC_COMMON->CCR |= (0x1UL << 16); // 01 = PCLK2 / 4

    /* CR1 bits 25:24 -> RES (resolution) */
    adc->CR1 &= ~(0x3UL << 24);
    adc->CR1 |= ((uint32_t)config->resolution << 24);

    /* CR2 bit 1 -> CONT (continuous conversion mode) */
    if (config->continuousMode)
        adc->CR2 |= (1UL << 1);
    else
        adc->CR2 &= ~(1UL << 1);

    /* CR2 bit 10 -> EOCS: generate EOC flag after each regular conversion
     * (default behavior we want for simple single/continuous reads) */
    adc->CR2 |= (1UL << 10);
}

void ADC_Enable(ADC_TypeDef *adc)
{
    adc->CR2 |= (1UL << 0); // ADON bit
}

void ADC_Disable(ADC_TypeDef *adc)
{
    adc->CR2 &= ~(1UL << 0);
}

void ADC_ConfigChannel(ADC_TypeDef *adc, uint8_t channel, ADC_SampleTime_t sampleTime)
{
    /* Sample time: channels 0-9 in SMPR2, channels 10-18 in SMPR1, 3 bits per channel */
    if (channel < 10)
    {
        adc->SMPR2 &= ~(0x7UL << (channel * 3));
        adc->SMPR2 |= ((uint32_t)sampleTime << (channel * 3));
    }
    else
    {
        uint8_t shifted = channel - 10;
        adc->SMPR1 &= ~(0x7UL << (shifted * 3));
        adc->SMPR1 |= ((uint32_t)sampleTime << (shifted * 3));
    }

    /* Regular sequence: for a single-conversion setup we only use SQR3 (channel 1
     * of the sequence), and set sequence length to 1 in SQR1 bits 23:20 (L = 0000) */
    adc->SQR3 &= ~(0x1FUL); // clear SQ1 bits
    adc->SQR3 |= (channel & 0x1FUL);

    adc->SQR1 &= ~(0xFUL << 20); // L = 0000 -> sequence length 1
}

void ADC_StartConversion(ADC_TypeDef *adc)
{
    adc->CR2 |= (1UL << 30); // SWSTART bit: start regular conversion by software
}

uint8_t ADC_IsConversionComplete(ADC_TypeDef *adc)
{
    return (uint8_t)((adc->SR >> 1) & 0x1UL); // EOC bit
}

uint16_t ADC_ReadValue(ADC_TypeDef *adc)
{
    return (uint16_t)(adc->DR & 0xFFFFUL); // reading DR also clears EOC
}

uint16_t ADC_ReadChannel(ADC_TypeDef *adc, uint8_t channel, ADC_SampleTime_t sampleTime)
{
    ADC_ConfigChannel(adc, channel, sampleTime);
    ADC_StartConversion(adc);

    while (!ADC_IsConversionComplete(adc)) {
        // wait for EOC flag
    }

    return ADC_ReadValue(adc);
}
