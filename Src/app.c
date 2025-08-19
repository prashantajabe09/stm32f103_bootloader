/*
 * app.c
 *
 *  Created on: Jul 9, 2025
 *      Author: PRASHANT AJABE
 */


#include "app.h"

uint8_t usart_return = 0;
static uint8_t flag_flash_memory, flag_ram_memory;

uint8_t supported_cmd[] = {
		BL_GET,
		BL_GET_VER,
		BL_GET_CID,
		BL_READ_MEM,
		BL_GO_TO_ADDR,
		BL_FLASH_MEM_WRITE,
		BL_FLASH_ERASE,
};

uint8_t buffer[BUFF_SIZE];


/**
 * @brief      control jumps to valid base address.
 *
 * @param[in]  base address of the user application
 *
 * @return    void
 *
 * @note  None
 *
 */

void jump_to_user_app(uint32_t user_app_base_address)
{
	uint32_t reset_handler_addr;
	uint32_t msp_value;

	// function pointer for Reset handler.
	void(*app_reset_handler)(void);

	msp_value = *(volatile uint32_t*)user_app_base_address;

	set_msp(msp_value);

	reset_handler_addr = *(volatile uint32_t*)(user_app_base_address + 4);

	// Assign the reset handler address to function pointer.
	app_reset_handler = (void(*)(void))reset_handler_addr;

	// assign vector table base address.
	SCB->VTOR = user_app_base_address;

	// call reset handler.
	app_reset_handler();
}

/**
 * @brief      inline assembly to set msp value.
 *
 * @param[in]  msp value
 *
 * @return    void
 *
 * @note  none
 *
 */
void set_msp(uint32_t msp_value)
{
	__asm volatile("MSR msp,%0"::"r"(msp_value));
}

/**
 * @brief     check for the BL command
 *
 * @param[in]  void
 *
 * @return    void
 *
 * @note  none
 *
 */
void bl_read_uart_data(void)
{
	while(1)
	{
		// clear buffer.
		memory_set(buffer,0,BUFF_SIZE);

		// Read for command.
		usart_read(&usart_2_handle,&buffer[0],2,1000);

		switch(buffer[0])
		{
			case BL_GET:
				bl_get(buffer);
				break;
			case BL_GET_VER:
				bl_get_ver_cmd(buffer);
				break;
			case BL_GET_CID:
				bl_get_cid_cmd(buffer);
				break;
			case BL_GO_TO_ADDR:
				bl_go_to_addr(buffer);
				break;
			case BL_FLASH_ERASE:
				bl_flash_erase(buffer);
				break;
			case BL_FLASH_MEM_WRITE:
				bl_mem_write(buffer);
				break;
			default:

		}
	}
}

/**
 * @brief      flash memory write
 *
 * @param[in]  buffer pointer carrying the payload to be written, base address, and payload length
 *
 * @return    void
 *
 * @note  	status of the flash write.
 *
 */
uint8_t execute_flash_mem_write(uint8_t* buffer,uint32_t base_addr,uint32_t payload_len)
{
	uint16_t temp_var = 0;
	uint8_t return_value = 0;

	// logic to extract 2 bytes of payload as flash write is of half-word size.
	for(uint32_t i = 0; i < payload_len; i += 2)
	{
		if (i + 1 < payload_len)
		{
			temp_var = buffer[i] | (buffer[i + 1] << 8);
		}
		else
		{
			temp_var = buffer[i] | (0xFF << 8);
		}

		// write to flash
		return_value = flash_mem_write(temp_var,base_addr + i);
	}
	return return_value;
}

/**
 * @brief      RAM memory write
 *
 * @param[in]  buffer pointer carrying the payload to be written, base address, and payload length
 *
 * @return    void
 *
 * @note  	status of the write.
 *
 */
uint8_t execute_ram_mem_write(uint8_t* buffer,uint32_t base_addr,uint32_t payload_len)
{
	uint32_t* p_base_addr = (uint32_t*)base_addr;

	// write to the base address
	*p_base_addr = *(uint32_t*)buffer;

	// read and cross check write
	if (*p_base_addr == *(uint32_t*)buffer)
	{
		return WRITE_SUCCESS;
	}
	else
	{
		return WRITE_FAILED;
	}
}


/**
 * @brief      Option Byte  memory write
 *
 * @param[in]  buffer pointer, base address.
 *
 * @return    void
 *
 * @note  	none
 *
 */

void execute_option_byte_write(uint8_t* buffer,uint32_t base_addr)
{
	// store the current option byte data.
	option_byte_read();

	//Erase the option byte.
	option_byte_erase();

	// write the option byte
	option_byte_write(base_addr,buffer);

	// reset the system to take effect of the new option byte
	system_reset();
}

/**
 * @brief      verify valid command
 *
 * @param[in]  buffer pointer containing the received command
 *
 * @return    status of the validity of command
 *
 * @note  	none
 *
 */
uint8_t is_valid_cmd(uint8_t* rec_cmd)
{
	if (rec_cmd == NULL)
		return INVALID_CMD;

	if( (rec_cmd[0] ^ rec_cmd[1]) == 0xFF)
	{
		return VALID_CMD;
	}

	return INVALID_CMD;
}

/**
 * @brief      Check if protection is action for flash memory
 *
 * @param[in]  void
 *
 * @return    status of the whether flash is protected or not.
 *
 * @note  	none
 *
 */
uint8_t is_protection_active(void)
{
	return is_flash_memory_write_protected();
}

/**
 * @brief      Check if memory is supported or not
 *
 * @param[in]  base address
 *
 * @return    status of whether memory supported or not.
 *
 * @note  	Memory will be supported only if input memory address is in the range
 *
 */
uint8_t is_supported_memory(uint32_t base_addr)
{
	if (base_addr >= USER_APP_FLASH_BASE_ADDR && base_addr <= FLASH_END_ADDR)
	{
		flag_flash_memory = 1;
		return MEMORY_SUPPORTED;
	}
	else if (base_addr >= USER_APP_RAM_BASE_ADDR && base_addr <= RAM_END_ADDRESS)
	{
		flag_ram_memory = 1;
		return MEMORY_SUPPORTED;
	}
	else
	{
		return MEMORY_NOT_SUPPORTED;
	}
}

/**
 * @brief      Check if address is option byte address or not
 *
 * @param[in]  base address
 *
 * @return    status of whether memory is in the range of the option byte or not.
 *
 * @note  	NONE
 *
 */

uint8_t is_option_byte_address(uint32_t base_addr)
{
	if (base_addr >= OP_BYTE_BASE_ADDR && base_addr <= OP_BYTE_WRP_2_3_REG_ADDR)
		return OPTION_BYTE_ADDRESS;
	else
		return NOT_OPTION_BYTE_ADDRESS;
}

/**
 * @brief      Check
 *
 * @param[in]  base address
 *
 * @return    status of whether memory is in the range of the option byte or not.
 *
 * @note  	NONE
 *
 */

bl_status_t bl_mem_write(uint8_t* buffer)
{
	uint32_t base_addr;
	uint8_t payload_len;

	// Step1: validate the command
	if (buffer == NULL || !is_valid_cmd(&buffer[0]))
		return BL_INVALID_CMD;

	// Step2: Check if protection active or not.
	if (is_protection_active())
		return BL_PROTECTED_MEMORY;

	bl_send_ack(1);

	// Step3: read base address
	if (usart_read(&usart_2_handle,&buffer[0],5,1000) != USART_READ_OK)
		return BL_ERROR_RX;

	// Step4: verify checksum
	base_addr = *( (uint32_t*)&buffer[0]);
	if (!bl_verify_checksum(&buffer[0],5))
	{
		bl_send_nack();
		return BL_CHECKSUM_FAILED;
	}
	bl_send_ack(1);

	// Step5: Read Payload length

	if (usart_read(&usart_2_handle,&buffer[0],1,1000) != USART_READ_OK)
		return BL_ERROR_RX;

	payload_len = buffer[0] + 1;

	// Step6: Read Payload
	if (usart_read(&usart_2_handle,&buffer[0],payload_len + 1,1000) != USART_READ_OK)
		return BL_ERROR_RX;

	// Step7: verify the payload checksum
	if (!bl_verify_checksum(&buffer[0],payload_len + 1))
	{
		bl_send_nack();
		return BL_CHECKSUM_FAILED;
	}

	//step8: check if it is supported memory or option byte.
	if (is_supported_memory(base_addr))
	{
		if (flag_flash_memory)
		{
			flag_flash_memory = 0;
			if (execute_flash_mem_write(&(buffer[0]),base_addr,payload_len) == WRITE_SUCCESS)
			{
				bl_send_ack(1);
			}
			else
			{
				bl_send_nack();
				return BL_FLASH_WRITE_FAILED;
			}
		}
		else if (flag_ram_memory)
		{
			flag_ram_memory = 0;
			if (execute_ram_mem_write(&(buffer[0]),base_addr,payload_len) == WRITE_SUCCESS)
			{
				bl_send_ack(1);
			}
			else
			{
				bl_send_nack();
				return BL_RAM_WRITE_FAILED;
			}
		}
	}
	else if (is_option_byte_address(base_addr))
	{
		execute_option_byte_write(&(buffer[0]),base_addr);
	}
	else
	{
		bl_send_nack();
		return BL_MEMORY_NOT_SUPPORTED;
	}
	return BL_OK;

}

/**
 * @brief      verify checksum.
 *
 * @param[in]  buffer Pointer to data buffer and length on which checksum need to verify
 *
 * @return    status of operation
 *
 * @note  XOR checckusm logic is used.
 *
 */
bl_status_t bl_verify_checksum(uint8_t* buffer,uint8_t len)
{
	uint8_t checksum = 0;

	// verify input
	if (buffer == NULL)
		return NULL_POINTER;

	for (uint8_t i = 0; i < len;i++)
	{
		checksum ^= buffer[i];
	}

	if (checksum == 0)
	{
		return CHECKSUM_MATCHED;
	}
	else
	{
		return CHECKSUM_NOT_MATCHED;
	}
}

/**
 * @brief get bootloader version command.
 *
 * @param[in]  buffer Pointer to received command buffer
 *
 * @return    status of operation
 *
 * @note This implementation sends two ACKs: one after command validation
 *       and one after sending the payload, it is as per AN3155 application note.
 *
 */
bl_status_t bl_get_ver_cmd(uint8_t* buffer)
{
	uint8_t bl_version = BL_VERSION;

	if (buffer == NULL || !is_valid_cmd(&buffer[0]))
		return BL_INVALID_CMD;

	bl_send_ack(1);

	// send the bl version
	uart_transmit(&usart_2_handle,&bl_version,1);

	bl_send_ack(1);

	return BL_OK;
}


/**
 * @brief get bootloader version and supported command.
 *
 * @param[in]  buffer Pointer to received command buffer
 *
 * @return    status of operation
 *
 * @note This implementation sends two ACKs: one after command validation
 *       and one after sending the payload, it is as per AN3155 application note.
 *
 */
bl_status_t bl_get(uint8_t* buffer)
{
	uint8_t cmd_count =(uint8_t) (sizeof(supported_cmd)/sizeof(supported_cmd[0]));
	uint8_t bl_version = BL_VERSION;

	if (buffer == NULL || !is_valid_cmd(&buffer[0]))
		return BL_INVALID_CMD;

	bl_send_ack(1);

	// send number of bytes
	uart_transmit(&usart_2_handle,&cmd_count,1);

	// send version
	uart_transmit(&usart_2_handle,&bl_version,1);

	// send supported commands
	uart_transmit(&usart_2_handle,supported_cmd,cmd_count);

	bl_send_ack(1);

	return BL_OK;

}

/**
 * @brief get bootloader chip ID.
 *
 * @param[in]  buffer Pointer to received command buffer
 *
 * @return    status of operation
 *
 * @note This implementation sends two ACKs: one after command validation
 *       and one after sending the payload, it is as per AN3155 application note.
 *
 */
bl_status_t bl_get_cid_cmd(uint8_t* buffer)
{
	uint16_t id = DBGMCU_IDCODE & 0xFFF;
	uint8_t length = 1;

	if (buffer == NULL || !is_valid_cmd(&buffer[0]))
		return BL_INVALID_CMD;

	bl_send_ack(1);

	// Send the number of bytes - 1.
	uart_transmit(&usart_2_handle,(uint8_t*)&length,1);

	// Send chip-ID
	uart_transmit(&usart_2_handle,(uint8_t*)&id,2); // send id.

	bl_send_ack(1);

	return BL_OK;
}

/**
 * @brief get bootloader Go command.
 *
 * @param[in]  buffer Pointer to received command buffer
 *
 * @return    status of operation
 *
 * @note This implementation jumps to address after verifying whether it is valid address or not.
 *
 */

bl_status_t bl_go_to_addr(uint8_t* buffer)
{
	uint32_t base_addr = 0;

	// Check for NULL pointer and valid command.
	if (buffer == NULL || !is_valid_cmd(&buffer[0]))
		return BL_INVALID_CMD;

	// Check for protection.
	if (is_protection_active())
		return BL_PROTECTED_MEMORY;

	bl_send_ack(1);

	// Read Address.
	if (usart_read(&usart_2_handle,&buffer[0],5,1000) != USART_READ_OK)
		return BL_ERROR_RX;

	// verify checksum
	base_addr = *( (uint32_t*)&buffer[0]);
	if (bl_verify_checksum(&buffer[0],5) && is_supported_memory(base_addr))
	{
		bl_send_ack(1);
		jump_to_user_app(base_addr);
	}
	else
	{
		bl_send_nack();
		return BL_CHECKSUM_FAILED;
	}

	return BL_OK;
}

/**
 * @brief      Flash Erase command.
 *
 * @param[in]  buffer Pointer to received command buffer
 *
 * @return    status of operation
 *
 * @note This implementation erase the either entire flash or number of pages from base page number.
 *
 */
bl_status_t bl_flash_erase(uint8_t* buffer)
{
	uint8_t pages_to_erase,base_page_number;

	// Check for NULL pointer and valid command.
	if (buffer == NULL || !is_valid_cmd(&buffer[0]))
		return BL_INVALID_CMD;

	// Check for protection.
	if (is_protection_active())
		return BL_PROTECTED_MEMORY;

	bl_send_ack(1);

	// Read the base number of pages to erase, base page number and checksum.
	if (usart_read(&usart_2_handle,&buffer[0],3,1000) != USART_READ_OK)
		return BL_ERROR_RX;

	// Mass Erase condition.
	if (buffer[0] == 0xFF)
	{
		flash_mass_erase();
	}
	else
	{
		pages_to_erase = buffer[0];
		base_page_number = buffer[1];
		// Verify checksum
		if (!bl_verify_checksum(&buffer[0],3))
		{
			bl_send_nack();
			return BL_CHECKSUM_FAILED;
		}

		for (uint8_t i = 0;i < pages_to_erase;i++)
		{
			flash_sector_erase(base_page_number + i);
		}

	}

	bl_send_ack(1);

	return BL_OK;
}

/**
 * @brief send ACK
 *
 * @param[in]  length
 *
 * @return   void
 *
 * @note This implementation transmit the ACK. length paramter is there for compatibility with earlier BL version.
 *
 */
void bl_send_ack(uint32_t length)
{
	uint8_t temp_arr[2];
	temp_arr[0] = BL_ACK;
	temp_arr[1] = (uint8_t)length;
	uart_transmit(&usart_2_handle,&temp_arr[0],1);
}

/**
 * @brief send NACK
 *
 * @param[in]  void
 *
 * @return   void
 *
 * @note This implementation transmit the NACK.
 *
 */
void bl_send_nack(void)
{
	uint8_t temp_var = BL_NACK;
	uart_transmit(&usart_2_handle,&temp_var,1);
}


/**
 * @brief get bootloader version
 *
 * @param[in]  void
 *
 * @return    BL version
 *
 * @note This implementation return the BL version.
 *
 */
uint8_t get_bl_version(void)
{
	return BL_VERSION;
}

