/*
 * System.c
 *
 *  Created on: 11-Sep-2024
 *      Author: Prashant Ajabe
 */

#include "System.h"

uint16_t counter_adc_sampling;
uint16_t counter_data_transmit;
uint16_t counter_led_toggle;
uint32_t counter_systick;
void nvic_en_irq(irqn_type irqn)
{
	uint8_t register_position = irqn / 32;
	uint8_t irq_position = irqn % 32;
	NVIC->ISER[register_position] = (1 << irq_position);
}

void systick_init(void)
{
	// Reload Value
	SYSTICK->RVR = 7999; //for 1 ms @ 8 Mhz
	// Clear the current count
	SYSTICK->CVR = 0;

	// Clock Selection
	SYSTICK->CSR |= (1 << 2) | (1 << 1) | ((1 << 0)); //AHB selected

}

void SysTick_Handler(void)
{
	counter_adc_sampling++;
	counter_data_transmit++;
	counter_led_toggle++;
	counter_systick++; // 1 ms tick.
}

uint32_t get_systick(void)
{
	return counter_systick;
}

void system_reset(void)
{
	SCB->AIRCR |= (0x5FA << 16) | (1 << 2);  // VECTKEY + SYSRESETREQ
    while (1); // Wait for reset to happen
}
