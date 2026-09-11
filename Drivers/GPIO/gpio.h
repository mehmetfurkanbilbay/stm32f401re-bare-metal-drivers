#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/*
 * GPIO Register Map
 *
 * GPIOx Base Address
 * +0x00 -> MODER
 * +0x04 -> OTYPER
 * +0x08 -> OSPEEDR
 * +0x0C -> PUPDR
 * +0x10 -> IDR
 * +0x14 -> ODR
 * +0x18 -> BSRR
 * +0x1C -> LCKR
 * +0x20 -> AFRL
 * +0x24 -> AFRH
 */

typedef struct{
	volatile uint32_t MODER;
	volatile uint32_t OTYPER;
	volatile uint32_t OSPEEDR;
	volatile uint32_t PUPDR;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t LCKR;
	volatile uint32_t AFRL;
	volatile uint32_t AFRH;
}GPIO_TypeDef;

/* GPIO Base Addresses */

#define GPIOA_BASE 0x40020000UL
#define GPIOB_BASE 0x40020400UL
#define GPIOC_BASE 0x40020800UL
#define GPIOD_BASE 0x40020C00UL
#define GPIOE_BASE 0x40021000UL
#define GPIOH_BASE 0x40021C00UL

/* GPIO Peripheral Pointers */

#define GPIOA ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE ((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOH ((GPIO_TypeDef *)GPIOH_BASE)

typedef enum {
	GPIO_MODE_INPUT  = 0x00,
	GPIO_MODE_OUTPUT = 0x01,
	GPIO_MODE_AF 	 = 0x02,
	GPIO_MODE_ANALOG = 0x03,
}GPIO_Mode_t;

typedef enum {
	GPIO_OTYPE_PP = 0x00,  // Push Pull
	GPIO_OTYPE_OD = 0x01   // Open-Drain
}GPIO_OType_t;

typedef enum {
	GPIO_SPEED_LOW		= 0x00,
	GPIO_SPEED_MEDIUM   = 0x01,
	GPIO_SPEED_HIGH		= 0x02,
	GPIO_SPEED_VHIGH	= 0x03
}GPIO_Speed_t;

typedef enum {
	GPIO_PUPD_NONE = 0x00,
	GPIO_PUPD_PU   = 0x01,
	GPIO_PUPD_PD   = 0x02
}GPIO_PuPd_t;

typedef struct {
	uint8_t 		pin;  // 0-15
	GPIO_Mode_t		mode;
	GPIO_OType_t	otype;
	GPIO_Speed_t	speed;
	GPIO_PuPd_t		pupd;
}GPIO_Config_t;

void GPIO_Init(GPIO_TypeDef *port, GPIO_Config_t *config);
void GPIO_WritePin(GPIO_TypeDef *port, uint8_t pin, uint8_t state);
uint8_t GPIO_ReadPin(GPIO_TypeDef *port, uint8_t pin);
void GPIO_TogglePin(GPIO_TypeDef *port, uint8_t pin);

/* Alternate function number: 0-15, check RM0368 "Alternate function mapping" table
 * for which AF number corresponds to which peripheral on a given pin */
void GPIO_SetAlternateFunction(GPIO_TypeDef *port, uint8_t pin, uint8_t af);



#endif
