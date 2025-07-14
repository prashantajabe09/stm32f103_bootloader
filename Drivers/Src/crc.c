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


uint32_t crc_cal(crc_regdef_t* p_crc, uint32_t* p_data, uint32_t len)
{
	uint32_t i = 0;
	uint32_t calculated_crc = 0;

	crc_clock_en();

	crc_reset(p_crc);

	for (i = 0; i < len;i++)
	{
		p_crc->DR = p_data[i];
	}

	calculated_crc = p_crc->DR;

	crc_clock_di();

	return calculated_crc;
}
