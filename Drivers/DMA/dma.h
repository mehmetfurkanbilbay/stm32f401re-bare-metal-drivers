#ifndef DMA_H
#define DMA_H

#include <stdint.h>

/*
 * DMA Register Map
 *
 * DMA1 Base Address: 0x40026000
 * DMA2 Base Address: 0x40026400
 *
 * +0x00 LISR   - Low Interrupt Status Register   (streams 0-3)
 * +0x04 HISR   - High Interrupt Status Register  (streams 4-7)
 * +0x08 LIFCR  - Low Interrupt Flag Clear Register
 * +0x0C HIFCR  - High Interrupt Flag Clear Register
 * +0x10 onwards -> 8 stream blocks, each 0x18 bytes (6 registers):
 *   SxCR   - Stream x Configuration Register
 *   SxNDTR - Stream x Number of Data Register
 *   SxPAR  - Stream x Peripheral Address Register
 *   SxM0AR - Stream x Memory 0 Address Register
 *   SxM1AR - Stream x Memory 1 Address Register (double-buffer mode)
 *   SxFCR  - Stream x FIFO Control Register
 */
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t NDTR;
    volatile uint32_t PAR;
    volatile uint32_t M0AR;
    volatile uint32_t M1AR;
    volatile uint32_t FCR;
} DMA_Stream_TypeDef;

typedef struct {
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
    DMA_Stream_TypeDef STREAM[8]; // STREAM[0]..STREAM[7]
} DMA_TypeDef;

#define DMA1_BASE 0x40026000UL
#define DMA2_BASE 0x40026400UL

#define DMA1 ((DMA_TypeDef *)DMA1_BASE)
#define DMA2 ((DMA_TypeDef *)DMA2_BASE)

typedef enum {
    DMA_DIR_PERIPH_TO_MEM = 0x00,
    DMA_DIR_MEM_TO_PERIPH = 0x01,
    DMA_DIR_MEM_TO_MEM    = 0x02
} DMA_Direction_t;

typedef struct {
    uint8_t          channel;        // 0-7, which peripheral request maps to this stream (RM0368 table)
    DMA_Direction_t  direction;
    uint32_t         peripheralAddr; // e.g. &USART2->DR
    uint32_t         memoryAddr;     // e.g. address of your buffer
    uint16_t         dataCount;      // number of items to transfer
    uint8_t          memoryIncrement; // 1
