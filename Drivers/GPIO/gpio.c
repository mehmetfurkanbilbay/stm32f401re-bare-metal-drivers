#include "gpio.h"

void GPIO_Init(GPIO_TypeDef *port, GPIO_Config_t *config){


	uint32_t pin = config->pin;

	/* MODER: each pin occupies 2 bits (pin2, pin2+1)
	 We first clear that pin's bits to 00 to prevent conflictsduring the OR operation
	|= cannot clear existing 1s, it only sets bits.*/

	port-> MODER &= ~(0x3UL << (pin * 2)); 					// clear: clear the corresponding 2 bits
	port-> MODER |= ((uint32_t)config->mode << (pin * 2));	// set: write new mode


	/* 2) OTYPER: each pin occupies 1 bit */
	port->OTYPER &= ~(0x01UL << pin);
	port->OTYPER |=  ((uint32_t)config->otype << pin);

	/* 3) OSPEEDR: each pin occupies 2 bit */
	port->OSPEEDR &= ~(0x3UL << (pin * 2));
	port->OSPEEDR |= ((uint32_t)config->speed << (pin * 2));

	/* 4) PUPDR: each pin occupies 2 bit */
	port->PUPDR &= ~(0x3UL << (pin * 2));
	port->PUPDR |= ((uint32_t)config->pupd << (pin * 2));


}

void GPIO_WritePin(GPIO_TypeDef *port, uint8_t pin, uint8_t state)
{
    if (state)
        port->BSRR = (1UL << pin);        // Set
    else
        port->BSRR = (1UL << (pin + 16)); // Reset
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *port, uint8_t pin)
{
    return (uint8_t)((port->IDR >> pin) & 0x1UL);
}

void GPIO_TogglePin(GPIO_TypeDef *port, uint8_t pin)
{
    port->ODR ^= (1UL << pin);
}


void GPIO_SetAlternateFunction(GPIO_TypeDef *port, uint8_t pin, uint8_t af)
{
    /* AFRL covers pins 0-7, AFRH covers pins 8-15.
     * Each pin uses 4 bits within its register. */
    if (pin < 8)
    {
        port->AFRL &= ~(0xFUL << (pin * 4));
        port->AFRL |= ((uint32_t)af << (pin * 4));
    }
    else
    {
        uint8_t shifted = pin - 8; // AFRH indexes pins 8-15 starting from bit 0
        port->AFRH &= ~(0xFUL << (shifted * 4));
        port->AFRH |= ((uint32_t)af << (shifted * 4));
    }
}
