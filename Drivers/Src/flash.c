/*
 * flash.c
 *
 *  Created on: Jul 18, 2025
 *      Author: PRASHANT AJABE
 */

#include "flash.h"

uint8_t flash_sector_erase(sector_e sector_no)
{
	uint32_t sector_addr = FLASH_BASEADDR + FLASH_PAGE_SIZE * sector_no;
	if (FLASH->SR & 0x01)
	{
		return FLASH_BUSY;
	}
	else
	{
		flash_unlock();
		FLASH->CR |= (1 << 1); /* Page erase chosen. */
		FLASH->AR = sector_addr; /* address of sector to be erase. */
		FLASH->CR |= (1 << 6); /* start page erase */
		while (FLASH->SR & 0x01);
		return FLASH_ERASE_SUCCESS;
	}
}
void flash_unlock(void)
{
	FLASH->KEYR = KEY_1;
	FLASH->KEYR = KEY_2;
}

void flash_lock(void)
{
	FLASH->CR |= (1 << 7);
}

uint8_t flash_mass_erase(void)
{
	if (FLASH->SR & 0x01)
	{
		return FLASH_BUSY;
	}
	else
	{
		flash_unlock();
		FLASH->CR |= (1 << 2); /* start page erase */
		FLASH->CR |= (1 << 6); /* start page erase */
		while (FLASH->SR & 0x01);
		return FLASH_ERASE_SUCCESS;
	}
}
