#include "spi.h"

void SPI_Init(SPI_TypeDef *spi, SPI_Config_t *config)
{
    /* CR1 bit layout:
     * bit 2   MSTR   - master selection
     * bit 1-0 CPOL/CPHA - clock polarity/phase
     * bit 5-3 BR[2:0]   - baud rate prescaler
     * bit 7   LSBFIRST  - frame format
     * bit 9   SSM       - software slave management
     * bit 8   SSI       - internal slave select (forces NSS high internally)
     */
    spi->CR1 = 0; // start from a known state

    spi->CR1 |= ((uint32_t)config->mode << 2);          // MSTR
    spi->CR1 |= ((uint32_t)config->cpol << 1);           // CPOL
    spi->CR1 |= ((uint32_t)config->cpha << 0);            // CPHA
    spi->CR1 |= ((uint32_t)config->baudPrescaler << 3);  // BR[2:0]
    spi->CR1 |= ((uint32_t)config->firstBit << 7);        // LSBFIRST

    /* Software NSS management: we control chip-select via a normal GPIO pin
     * instead of the dedicated NSS hardware pin. SSM=1 + SSI=1 keeps the
     * peripheral from thinking it's a slave being deselected. */
    spi->CR1 |= (1UL << 9);  // SSM
    spi->CR1 |= (1UL << 8);  // SSI
}

void SPI_Enable(SPI_TypeDef *spi)
{
    spi->CR1 |= (1UL << 6); // SPE bit: SPI peripheral enable
}

void SPI_Disable(SPI_TypeDef *spi)
{
    spi->CR1 &= ~(1UL << 6); // SPE bit: SPI peripheral disable
}

uint8_t SPI_TransmitReceive(SPI_TypeDef *spi, uint8_t data)
{
    while (!SPI_IsTxEmpty(spi)) {
        // wait until the transmit buffer is free (TXE flag)
    }
    spi->DR = data;

    while (!SPI_IsRxNotEmpty(spi)) {
        // SPI is full-duplex: every byte sent also shifts a byte in
    }
    return (uint8_t)(spi->DR & 0xFF);
}

void SPI_Transmit(SPI_TypeDef *spi, uint8_t data)
{
    SPI_TransmitReceive(spi, data); // discard the received byte
}

uint8_t SPI_Receive(SPI_TypeDef *spi)
{
    return SPI_TransmitReceive(spi, 0xFF); // send dummy byte to generate clock
}

uint8_t SPI_IsTxEmpty(SPI_TypeDef *spi)
{
    return (uint8_t)((spi->SR >> 1) & 0x1UL); // TXE bit
}

uint8_t SPI_IsRxNotEmpty(SPI_TypeDef *spi)
{
    return (uint8_t)((spi->SR >> 0) & 0x1UL); // RXNE bit
}

uint8_t SPI_IsBusy(SPI_TypeDef *spi)
{
    return (uint8_t)((spi->SR >> 7) & 0x1UL); // BSY bit
}
