/*
 * flash.c
 *
 *  Created on: Jul 18, 2025
 *      Author: PRASHANT AJABE
 */

#include "flash.h"

uint32_t option_byte_info[4];

/**
 * @brief      sector wise flash erase.
 *
 * @param[in]  sector number
 *
 * @return     flash status
 *
 * @note  	   from the sector number, base address calculated and then that sector is erased.
 *
 */

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
		while(!(FLASH->SR & 0x20));
		FLASH->CR &= ~(1 << 1);
		flash_lock();
		return FLASH_ERASE_SUCCESS;
	}
}

/**
 * @brief      flash unlock
 *
 * @param[in]  void
 *
 * @return     void
 *
 * @note  	   none
 *
 */
void flash_unlock(void)
{
	FLASH->KEYR = KEY_1;
	FLASH->KEYR = KEY_2;
}

/**
 * @brief      option byte unlock
 *
 * @param[in]  void
 *
 * @return     void
 *
 * @note  	   none
 *
 */
void op_byte_unlock(void)
{
	FLASH->OPTKEYR = KEY_1;
	FLASH->OPTKEYR = KEY_2;
}


/**
 * @brief      check flash memory protected or not
 *
 * @param[in]  void
 *
 * @return     void
 *
 * @note  	   Check for Write Protection register value.
 *
 */
uint8_t is_flash_memory_write_protected(void)
{
	if (FLASH->WRPR == 0)
		return FLASH_MEMORY_WRITE_PROTECTED;
	else
		return FLASH_MEMORY_NOT_WRITE_PROTECTED;
}

/**
 * @brief      option byte lock
 *
 * @param[in]  void
 *
 * @return     void
 *
 * @note  	   none
 *
 */
void flash_lock(void)
{
	FLASH->CR |= (1 << 7);
}

/**
 * @brief      flash mass Erase
 *
 * @param[in]  void
 *
 * @return     flash erase status
 *
 * @note  	   none
 *
 */
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
		while(!(FLASH->SR & 0x20));
		FLASH->CR &= ~(1 << 1);
		flash_lock();
		return FLASH_ERASE_SUCCESS;

	}
}

/**
 * @brief      flash memory write
 *
 * @param[in]  data to be write and base address
 *
 * @return     Flash memoey write status
 *
 * @note  	   none
 *
 */
uint8_t flash_mem_write(uint16_t data, uint32_t address)
{
	uint16_t* temp_addr = (uint16_t*)address; // Forced for 16-bit address
	uint16_t temp_var = 0;
	if (FLASH->SR & 0x01) // check of busy
	{
		return FLASH_BUSY;
	}
	else
	{
		flash_unlock();
		FLASH->CR |= (1 << 0); // Set PG bit
		*temp_addr = data; // write data to address.
		while (FLASH->SR & 0x01); // waite for busy flag to get clear.
		if (FLASH->SR & (1 << 5)) // check for EOP.
		{
			FLASH->SR |= (1 << 5);
		}
		FLASH->CR &= ~(1 << 0); // Reset PG bit.
		flash_lock();
		if (data == *temp_addr)
		{
			return WRITE_SUCCESS;
		}
		else
		{
			return WRITE_FAILED;
		}
	}
}

/**
 * @brief      option byte info read
 *
 * @param[in]  void
 *
 * @return     void
 *
 * @note  	   read the information at option byte address.
 *
 */
void option_byte_read(void)
{
	uint32_t* op_byte_base_addr = (uint32_t*)OP_BYTE_BASE_ADDR;
	for (uint8_t i = 0; i < 4;i++)
	{
		option_byte_info[i] = *op_byte_base_addr++;
	}
}

/**
 * @brief      option byte erase
 *
 * @param[in]  void
 *
 * @return     status of flash
 *
 * @note  	   none.
 *
 */
uint8_t option_byte_erase(void)
{
	uint32_t* op_addr = OP_BYTE_BASE_ADDR;
	if (FLASH->SR & 0x01) // check of busy
	{
		return FLASH_BUSY;
	}
	else
	{
		flash_unlock();
		op_byte_unlock();
		FLASH->CR |= (1 << 5); // set OPTER
		FLASH->CR |= (1 << 6); // start erase
		while (FLASH->SR & 0x01); // waite for busy flag to get clear.
		FLASH->CR &= ~(1 << 5);
		if (FLASH->SR & 0x20)
		{
			FLASH->SR |= (1 << 5);
		}
		flash_lock();
		for (uint8_t i = 0; i < 4;i++)
		{
			if (*op_addr++ != 0xFFFFFFFF)
			{

				return OP_BYTE_ERASE_FAILED;
			}
		}
		return OP_BYTE_ERASE_SUCCESS;
	}
}

/**
 * @brief      option byte write
 *
 * @param[in]   address and pointer to data
 *
 * @return     void
 *
 * @note  	   all the information filled in option byte address along with the desired address.
 *
 */
void option_byte_write(uint32_t base_addr,uint8_t* p_data)
{
	uint16_t* op_addr = (uint16_t*)OP_BYTE_BASE_ADDR;
	uint32_t* p_data_temp = (uint32_t*)p_data;
	uint16_t temp_16_bit_storage = 0;
	uint8_t address_index = (base_addr & 0xFF) / 4;

	if (FLASH->SR & 0x01) // check of busy
	{
		return FLASH_BUSY;
	}
	else
	{
		flash_unlock();
		op_byte_unlock();
		FLASH->CR |= (1 << 4); // set OPTPG
		for (uint8_t i = 0; i < 4; i++)
		{
			if (address_index != i)
			{
				temp_16_bit_storage = (word_to_byte(option_byte_info[i],2,1) << 8) | (word_to_byte(option_byte_info[i],1,1));
				*op_addr++ = temp_16_bit_storage;
				temp_16_bit_storage = (word_to_byte(option_byte_info[i],4,1) << 8) | (word_to_byte(option_byte_info[i],3,1));
				*op_addr++ = temp_16_bit_storage;
			}
			else
			{
				temp_16_bit_storage = (word_to_byte(*p_data_temp,2,1) << 8) | (word_to_byte(*p_data_temp,1,1));
				*op_addr++ = temp_16_bit_storage;
				temp_16_bit_storage = (word_to_byte(*p_data_temp,4,1) << 8) | (word_to_byte(*p_data_temp,3,1));
				*op_addr++ = temp_16_bit_storage;
			}
		}
	}
}
