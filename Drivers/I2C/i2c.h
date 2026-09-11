#ifndef I2C_H
#define I2C_H

#include <stdint.h>

/*
 * I2C Register Map
 *
 * I2C1 Base Address: 0x40005400  (APB1 bus)
 * I2C2 Base Address: 0x40005800  (APB1 bus)
 * I2C3 Base Address: 0x40005C00  (APB1 bus)
 *
 * +0x00 -> CR1     (Control Register 1)
 * +0x04 -> CR2     (Control Register 2)
 * +0x08 -> OAR1    (Own Address Register 1)
 * +0x0C -> OAR2    (Own Address Register 2)
 * +0x10 -> DR      (Data Register)
 * +0x14 -> SR1     (Status Register 1)
 * +0x18 -> SR2     (Status Register 2)
 * +0x1C -> CCR     (Clock Control Register)
 * +0x20 -> TRISE   (Rise Time Register)
 * +0x24 -> FLTR    (Noise Filter Register)
 */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
    volatile uint32_t FLTR;
} I2C_TypeDef;

#define I2C1_BASE 0x40005400UL
#define I2C2_BASE 0x40005800UL
#define I2C3_BASE 0x40005C00UL

#define I2C1 ((I2C_TypeDef *)I2C1_BASE)
#define I2C2 ((I2C_TypeDef *)I2C2_BASE)
#define I2C3 ((I2C_TypeDef *)I2C3_BASE)

typedef enum {
    I2C_SPEED_STANDARD = 0x00, // 100 kHz
    I2C_SPEED_FAST      = 0x01 // 400 kHz
} I2C_Speed_t;

typedef struct {
    uint32_t     peripheralClockMHz; // APB1 clock in MHz (e.g. 42)
    I2C_Speed_t  speed;
} I2C_Config_t;

/* ==================== BASIC I2C FUNCTIONS ==================== */
void I2C_Init(I2C_TypeDef *i2c, I2C_Config_t *config);
void I2C_Enable(I2C_TypeDef *i2c);
void I2C_Disable(I2C_TypeDef *i2c);

void I2C_GenerateStart(I2C_TypeDef *i2c);
void I2C_GenerateStop(I2C_TypeDef *i2c);
void I2C_SendAddress(I2C_TypeDef *i2c, uint8_t address, uint8_t readNotWrite);
void I2C_WriteByte(I2C_TypeDef *i2c, uint8_t data);
uint8_t I2C_ReadByte(I2C_TypeDef *i2c, uint8_t ackNextByte);

uint8_t I2C_IsStartFlagSet(I2C_TypeDef *i2c);
uint8_t I2C_IsAddressFlagSet(I2C_TypeDef *i2c);
void I2C_ClearAddressFlag(I2C_TypeDef *i2c);
uint8_t I2C_IsTxEmpty(I2C_TypeDef *i2c);
uint8_t I2C_IsRxNotEmpty(I2C_TypeDef *i2c);
uint8_t I2C_IsByteTransferFinished(I2C_TypeDef *i2c);

/* Convenience wrapper for a full register write, e.g. writing to a sensor register */
void I2C_WriteRegister(I2C_TypeDef *i2c, uint8_t slaveAddr, uint8_t regAddr, uint8_t data);
uint8_t I2C_ReadRegister(I2C_TypeDef *i2c, uint8_t slaveAddr, uint8_t regAddr);

#endif
