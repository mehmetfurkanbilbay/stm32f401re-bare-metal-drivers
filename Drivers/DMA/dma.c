#include "dma.h"

/* Helper: LISR/HISR bit positions repeat in a 4-stream pattern (0,1,2,3 / 4,5,6,7).
 * Within each group of 4: stream%4==0 -> base bit 0, ==1 -> base bit 6,
 * ==2 -> base bit 16, ==3 -> base bit 22. TCIF (transfer complete flag) sits
 * 5 bits after that base in every case. */
static uint8_t DMA_GetTCIFBitPos(uint8_t streamNum)
{
    uint8_t group = streamNum % 4;
    switch (group)
    {
        case 0: return 5;
        case 1: return 11;
        case 2: return 21;
        default: return 27; // case 3
    }
}

void DMA_Init(DMA_TypeDef *dma, uint8_t streamNum, DMA_Config_t *config)
{
    DMA_Stream_TypeDef *stream = &dma->STREAM[streamNum];

    /* Stream must be disabled before reconfiguring (RM0368 requirement) */
    stream->CR &= ~(1UL << 0); // EN = 0
    while (stream->CR & (1UL << 0)) {
        // wait until hardware confirms the stream is actually off
    }

    stream->PAR  = config->peripheralAddr;
    stream->M0AR = config->memoryAddr;
    stream->NDTR = config->dataCount;

    /* CR bits: 27:25 CHSEL, 7:6 DIR, 10 MINC, 8 CIRC */
    stream->CR &= ~(0x7UL << 25);
    stream->CR |= ((uint32_t)config->channel << 25);

    stream->CR &= ~(0x3UL << 6);
    stream->CR |= ((uint32_t)config->direction << 6);

    if (config->memoryIncrement)
        stream->CR |= (1UL << 10);  // MINC
    else
        stream->CR &= ~(1UL << 10);

    if (config->circularMode)
        stream->CR |= (1UL << 8);   // CIRC
    else
        stream->CR &= ~(1UL << 8);

    /* TCIE (bit 4): enable transfer-complete interrupt is left to the caller
     * if they want interrupts; polling mode (our default here) doesn't need it. */
}

void DMA_Enable(DMA_TypeDef *dma, uint8_t streamNum)
{
    dma->STREAM[streamNum].CR |= (1UL << 0); // EN bit
}

void DMA_Disable(DMA_TypeDef *dma, uint8_t streamNum)
{
    dma->STREAM[streamNum].CR &= ~(1UL << 0);
}

uint8_t DMA_IsTransferComplete(DMA_TypeDef *dma, uint8_t streamNum)
{
    uint8_t bitPos = DMA_GetTCIFBitPos(streamNum);
    uint32_t statusReg = (streamNum < 4) ? dma->LISR : dma->HISR;
    return (uint8_t)((statusReg >> bitPos) & 0x1UL);
}

void DMA_ClearTransferComplete(DMA_TypeDef *dma, uint8_t streamNum)
{
    uint8_t bitPos = DMA_GetTCIFBitPos(streamNum);
    if (streamNum < 4)
        dma->LIFCR = (1UL << bitPos);  // write-1-to-clear, like EXTI's PR
    else
        dma->HIFCR = (1UL << bitPos);
}
