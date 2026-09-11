#ifndef PWR_H
#define PWR_H

#include <stdint.h>

/*
 * PWR Register Map
 *
 * PWR Base Address: 0x40007000
 *
 * +0x00 -> CR   (Power Control Register)
 * +0x04 -> CSR  (Power Control/Status Register)
 */
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CSR;
} PWR_TypeDef;

#define PWR_BASE 0x40007000UL
#define PWR ((PWR_TypeDef *)PWR_BASE)

/* ==================== BACKUP DOMAIN ACCESS ==================== */
void PWR_EnableBackupAccess(void);

#endif
