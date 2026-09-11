#ifndef SPI_H
#define SPI_H

#include <stdint.h>

/*
 * SPI Register Map
 *
 * SPI1 Base Address: 0x40013000  (APB2 bus)
 * SPI2 Base Address: 0x40003800  (APB1 bus)
 * SPI3 Base Address: 0x40003C00  (APB1 bus)
 *
 * +0x00 -> CR1     (Control Register 1)
 * +0x04 -> CR2     (Control Register 2)
 * +0x08 -> SR      (Status Register)
 * +0x0C -> DR      (Data Register)
 * +0x10 -> CRCPR   (CRC Polynomial Register)
 * +0x14 -> RXCRCR  (RX CRC Register)
 * +0x18 -> TXCRCR  (TX CRC Register)
 * +0x1C -> I2SCFGR (I2S Configuration Register)
 * +0x20 -> I2SPR   (I2S Prescaler Register)
 */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
} SPI_TypeDef;

#define SPI1_BASE 0x40013000UL
#define SPI2_BASE 0x40003800UL
#define SPI3_BASE 0x40003C00UL

#define SPI1 ((SPI_TypeDef *)SPI1_BASE)
#define SPI2 ((SPI_TypeDef *)SPI2_BASE)
#define SPI3 ((SPI_TypeDef *)SPI3_BASE)

typedef enum {
    SPI_MODE_SLAVE  = 0x00,
    SPI_MODE_MASTER = 0x01
} SPI_Mode_t;

typedef enum {
    SPI_CPOL_LOW  = 0x00,  // clock idle state = 0
    SPI_CPOL_HIGH = 0x01   // clock idle state = 1
} SPI_CPOL_t;

typedef enum {
    SPI_CPHA_1EDGE = 0x00, // data captured on first clock edge
    SPI_CPHA_2EDGE = 0x01  // data captured on second clock edge
} SPI_CPHA_t;

typedef enum {
    SPI_BAUD_DIV2   = 0x00,
    SPI_BAUD_DIV4   = 0x01,
    SPI_BAUD_DIV8   = 0x02,
    SPI_BAUD_DIV16  = 0x03,
    SPI_BAUD_DIV32  = 0x04,
    SPI_BAUD_DIV64  = 0x05,
    SPI_BAUD_DIV128 = 0x06,
    SPI_BAUD_DIV256 = 0x07
} SPI_BaudPrescaler_t;

typedef enum {
    SPI_FIRSTBIT_MSB = 0x00,
    SPI_FIRSTBIT_LSB = 0x01
} SPI_FirstBit_t;

typedef struct {
    SPI_Mode_t           mode;
    SPI_CPOL_t           cpol;
    SPI_CPHA_t           cpha;
    SPI_BaudPrescaler_t  baudPrescaler;
    SPI_FirstBit_t       firstBit;
} SPI_Config_t;

/* ==================== BASIC SPI FUNCTIONS ==================== */
void SPI_Init(SPI_TypeDef *spi, SPI_Config_t *config);
void SPI_Enable(SPI_TypeDef *spi);
void SPI_Disable(SPI_TypeDef *spi);
uint8_t SPI_TransmitReceive(SPI_TypeDef *spi, uint8_t data);
void SPI_Transmit(SPI_TypeDef *spi, uint8_t data);
uint8_t SPI_Receive(SPI_TypeDef *spi);
uint8_t SPI_IsTxEmpty(SPI_TypeDef *spi);
uint8_t SPI_IsRxNotEmpty(SPI_TypeDef *spi);
uint8_t SPI_IsBusy(SPI_TypeDef *spi);

#endif
