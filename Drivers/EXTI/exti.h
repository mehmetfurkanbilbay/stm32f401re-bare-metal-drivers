#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>
#include "gpio.h"

/*
 *EXTI Register Map
 *EXTI Base Address: 0x40013C00
 *
 *+0x00 IMR		- Interrupt Mask Register
 *+0x04 EMR		- Event Mask Register
 *+0x08 RTSR	- Rising Trigger Selection Register
 *+0x0C FTSR	- Falling Trigger Selection Register
 *+0x10 SWIER	- Software Interrupt Event Register
 *+0x14 PR		- Pending Register
 */


typedef struct {
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_TypeDef;

#define EXTI_BASE 0x40013C00
#define EXTI ((EXTI_TypeDef *)EXTI_BASE)


/*
 * SYSCFG Register Map (only the part relevant to EXTI)
 * SYSCFG Base Address: 0x40013800
 *
 * +0x00 MEMRMP     - Memory Remap Register
 * +0x04 PMC        - Peripheral Mode Configuration Register
 * +0x08 EXTICR[0]  - External Interrupt Configuration Register 1 (lines 0-3)
 * +0x0C EXTICR[1]  - External Interrupt Configuration Register 2 (lines 4-7)
 * +0x10 EXTICR[2]  - External Interrupt Configuration Register 3 (lines 8-11)
 * +0x14 EXTICR[3]  - External Interrupt Configuration Register 4 (lines 12-15)
 * +0x20 CMPCR      - Compensation Cell Control Register
 */

typedef struct {
	volatile uint32_t MEMRMP;
	volatile uint32_t PMC;
	volatile uint32_t EXTICR[4];
	uint32_t RESERVED0[2];
	volatile uint32_t CMPCR;
}SYSCFG_TypeDef;

#define SYSCFG_BASE 0x40013800UL
#define SYSCFG ((SYSCFG_TypeDef *)SYSCFG_BASE)

typedef enum {
	EXTI_TRIGGER_RISING  = 0x00,
	EXTI_TRIGGER_FALLING = 0x01,
	EXTI_TIGGER_BOTH 	 = 0x02
}EXTI_Trigger_t;

typedef struct {
	GPIO_TypeDef 	*port; 	// which GPIO port drives this line (GPIOA, GPIOB, ...)
	uint8_t 	 	pin;	// pin number 0-15, also the EXTI line number
	EXTI_Trigger_t	trigger;
}EXTI_Config_t;

void EXTI_Init(EXTI_Config_t *config);
void EXTI_ClearPending(uint8_t line);
uint8_t EXTI_GetPending(uint8_t line);




#endif
