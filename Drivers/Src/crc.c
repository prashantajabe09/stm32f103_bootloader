/*
 * crc.c
 *
 *  Created on: Jul 14, 2025
 *      Author: PRASHANT AJABE
 */

#include "crc.h"

void crc_reset(crc_regdef_t* p_crc)
{
	p_crc->CR |= (1 << 0);
}

void crc_clock_en(void)
{
	RCC->AHBENR |= (1 << 6);
}

void crc_clock_di(void)
{
	RCC->AHBENR &= ~(1 << 6);
}

uint32_t crc_calculate(crc_regdef_t* p_crc, uint8_t* p_data, uint32_t len)
{
	uint32_t calculated_crc = 0;
	uint32_t temp_value = 0;
	for (uint32_t i = 0; i < len;i++)
	{
		temp_value = p_data[i];
		p_crc->DR = temp_value;
	}
	return calculated_crc = p_crc->DR;
}

uint8_t crc_check(crc_regdef_t* p_crc, uint8_t* p_data, uint32_t len,uint32_t received_crc)
{
	uint32_t calculated_crc = 0;

	crc_clock_en();

	crc_reset(p_crc);

	calculated_crc = crc_calculate(p_crc,p_data,len);

	crc_clock_di();

	return (calculated_crc == received_crc);
}
