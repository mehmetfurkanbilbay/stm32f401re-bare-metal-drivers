#include "i2c.h"

void I2C_Init(I2C_TypeDef *i2c, I2C_Config_t *config)
{
    /* CR2 bits 5:0 -> FREQ: tells the peripheral the APB1 clock frequency in MHz,
     * needed internally for timing generation */
    i2c->CR2 &= ~(0x3FUL);
    i2c->CR2 |= (config->peripheralClockMHz & 0x3FUL);

    if (config->speed == I2C_SPEED_STANDARD)
    {
        /* Standard mode: CCR = Fpclk / (2 * Fscl) */
        uint32_t ccrValue = config->peripheralClockMHz * 1000000UL / (2UL * 100000UL);
        i2c->CCR = ccrValue & 0xFFFUL;   // F/S bit stays 0 -> standard mode

        /* TRISE = (max rise time * Fpclk) + 1, standard mode max rise time = 1000ns */
        i2c->TRISE = config->peripheralClockMHz + 1;
    }
    else // I2C_SPEED_FAST
    {
        /* Fast mode: CCR = Fpclk / (3 * Fscl), using Tlow/Thigh = 2 (duty = 0) */
        uint32_t ccrValue = config->peripheralClockMHz * 1000000UL / (3UL * 400000UL);
        i2c->CCR = (ccrValue & 0xFFFUL) | (1UL << 15); // F/S bit = 1 -> fast mode

        /* Fast mode max rise time = 300ns */
        i2c->TRISE = ((config->peripheralClockMHz * 300UL) / 1000UL) + 1;
    }
}

void I2C_Enable(I2C_TypeDef *i2c)
{
    i2c->CR1 |= (1UL << 0); // PE bit: peripheral enable
}

void I2C_Disable(I2C_TypeDef *i2c)
{
    i2c->CR1 &= ~(1UL << 0);
}

void I2C_GenerateStart(I2C_TypeDef *i2c)
{
    i2c->CR1 |= (1UL << 8); // START bit
    while (!I2C_IsStartFlagSet(i2c)) {
        // wait until SB flag in SR1 is set (start condition generated)
    }
}

void I2C_GenerateStop(I2C_TypeDef *i2c)
{
    i2c->CR1 |= (1UL << 9); // STOP bit
}

void I2C_SendAddress(I2C_TypeDef *i2c, uint8_t address, uint8_t readNotWrite)
{
    /* 7-bit address goes in bits 7:1 of the byte, LSB (bit 0) is R/W:
     * 0 = write, 1 = read */
    uint8_t frame = (uint8_t)((address << 1) | (readNotWrite & 0x1));
    i2c->DR = frame;

    while (!I2C_IsAddressFlagSet(i2c)) {
        // wait until ADDR flag in SR1 is set (address acknowledged by slave)
    }
    I2C_ClearAddressFlag(i2c);
}

void I2C_WriteByte(I2C_TypeDef *i2c, uint8_t data)
{
    while (!I2C_IsTxEmpty(i2c)) {
        // wait until DR is free (TXE flag)
    }
    i2c->DR = data;

    while (!I2C_IsByteTransferFinished(i2c)) {
        // wait until the byte has fully shifted out (BTF flag)
    }
}

uint8_t I2C_ReadByte(I2C_TypeDef *i2c, uint8_t ackNextByte)
{
    if (ackNextByte)
        i2c->CR1 |= (1UL << 10);  // ACK bit: acknowledge, more bytes expected
    else
        i2c->CR1 &= ~(1UL << 10); // NACK: this is the last byte

    while (!I2C_IsRxNotEmpty(i2c)) {
        // wait until DR has received data (RXNE flag)
    }
    return (uint8_t)(i2c->DR & 0xFF);
}

uint8_t I2C_IsStartFlagSet(I2C_TypeDef *i2c)
{
    return (uint8_t)((i2c->SR1 >> 0) & 0x1UL); // SB bit
}

uint8_t I2C_IsAddressFlagSet(I2C_TypeDef *i2c)
{
    return (uint8_t)((i2c->SR1 >> 1) & 0x1UL); // ADDR bit
}

void I2C_ClearAddressFlag(I2C_TypeDef *i2c)
{
    /* ADDR is cleared by reading SR1 then SR2 (hardware sequence, not a write-clear) */
    (void)i2c->SR1;
    (void)i2c->SR2;
}

uint8_t I2C_IsTxEmpty(I2C_TypeDef *i2c)
{
    return (uint8_t)((i2c->SR1 >> 7) & 0x1UL); // TXE bit
}

uint8_t I2C_IsRxNotEmpty(I2C_TypeDef *i2c)
{
    return (uint8_t)((i2c->SR1 >> 6) & 0x1UL); // RXNE bit
}

uint8_t I2C_IsByteTransferFinished(I2C_TypeDef *i2c)
{
    return (uint8_t)((i2c->SR1 >> 2) & 0x1UL); // BTF bit
}

void I2C_WriteRegister(I2C_TypeDef *i2c, uint8_t slaveAddr, uint8_t regAddr, uint8_t data)
{
    I2C_GenerateStart(i2c);
    I2C_SendAddress(i2c, slaveAddr, 0); // 0 = write
    I2C_WriteByte(i2c, regAddr);
    I2C_WriteByte(i2c, data);
    I2C_GenerateStop(i2c);
}

uint8_t I2C_ReadRegister(I2C_TypeDef *i2c, uint8_t slaveAddr, uint8_t regAddr)
{
    uint8_t value;

    /* Write phase: tell the slave which register we want */
    I2C_GenerateStart(i2c);
    I2C_SendAddress(i2c, slaveAddr, 0); // 0 = write
    I2C_WriteByte(i2c, regAddr);

    /* Repeated start into read phase */
    I2C_GenerateStart(i2c);
    I2C_SendAddress(i2c, slaveAddr, 1); // 1 = read

    value = I2C_ReadByte(i2c, 0); // 0 = NACK, this is the only/last byte
    I2C_GenerateStop(i2c);

    return value;
}
