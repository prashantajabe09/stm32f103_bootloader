/*
 * app.c
 *
 *  Created on: Jul 9, 2025
 *      Author: PRASHANT AJABE
 */


#include "app.h"

uint8_t supported_cmd[] = {
		BL_GET_VER,
		BL_GET_HELP,
		BL_GET_CID,
		BL_GET_RDP_STATUS,
		BL_GO_TO_ADDR,
		BL_FLASH_ERASE,
		BL_MEM_WRITE,
		BL_EN_R_W_PROTECT,
		BL_MEM_READ,
		BL_READ_SECTOR_STATUS,
		BL_OTP_READ,
		BL_DIS_R_W_PROTECT
};

uint8_t buffer[BUFF_SIZE];

void jump_to_user_app(void)
{
	uint32_t reset_handler_addr;
	uint32_t msp_value;
	void(*app_reset_handler)(void);

	msp_value = *(volatile uint32_t*)USER_APPLICATION_BASE_ADDR;
	print_msg("application msp: %x \r\n",msp_value);
	set_msp(msp_value);

	reset_handler_addr = *(volatile uint32_t*)(USER_APPLICATION_BASE_ADDR + 4);
	print_msg("reset handler addr: %x \r\n",reset_handler_addr);
	app_reset_handler = (void(*)(void))reset_handler_addr;

	SCB->VTOR = USER_APPLICATION_BASE_ADDR;

	app_reset_handler();
}

void set_msp(uint32_t msp_value)
{
	__asm volatile("MSR msp,%0"::"r"(msp_value));
}

void bl_read_uart_data(void)
{
	uint8_t receive_length = 0;
	while(1)
	{
		memory_set(buffer,0,BUFF_SIZE);
		usart_read(&usart_2_handle,&buffer[0],1,1000);
		receive_length = buffer[0];
		usart_read(&usart_2_handle,&buffer[1],receive_length,1000);
		switch(buffer[1])
		{
			case BL_GET_VER:
				bl_get_ver_cmd(buffer);
				break;
			case BL_GET_HELP:
				bl_get_help_cmd(buffer);
				break;
			case BL_GET_CID:
				bl_get_cid_cmd(buffer);
				break;
			case BL_GET_RDP_STATUS:
				bl_get_flash_rdp(buffer);
				break;
			case BL_GO_TO_ADDR:
				bl_go_to_addr(buffer);
				break;
			case BL_FLASH_ERASE:
				bl_flash_erase(buffer);
				break;
			default:

		}
	}
}

uint8_t bl_check_crc(uint8_t* p_data,uint32_t len,uint32_t host_crc)
{
	return crc_check(CRC,p_data,len,host_crc);
}

void bl_get_ver_cmd(uint8_t* buffer)
{
	uint8_t bl_version;
	uint32_t command_packet_length  = buffer[0] + 1;
	uint32_t host_crc = *(uint32_t*)(&buffer[0] + command_packet_length - 4);

	if (bl_check_crc(&buffer[0],2,host_crc))
	{
		bl_send_ack(1);
		bl_version = get_bl_version();
		uart_transmit(&usart_2_handle,&bl_version,1);
	}
	else
	{
		bl_send_nack();
	}
}

void bl_get_help_cmd(uint8_t* buffer)
{
	uint32_t command_packet_length  = buffer[0] + 1;
	uint32_t host_crc = *(uint32_t*)(&buffer[0] + command_packet_length - 4);
	if (bl_check_crc(&buffer[0],2,host_crc))
	{
		bl_send_ack(sizeof(supported_cmd));
		uart_transmit(&usart_2_handle,(uint8_t*)supported_cmd,sizeof(supported_cmd));
	}
	else
	{
		bl_send_nack();
	}
}

void bl_get_cid_cmd(uint8_t* buffer)
{
	uint16_t id = 0;
	uint32_t command_packet_length  = buffer[0] + 1;
	uint32_t host_crc = *(uint32_t*)(&buffer[0] + command_packet_length - 4);
	if (bl_check_crc(&buffer[0],2,host_crc))
	{
		bl_send_ack(2);
		id = DBGMCU_IDCODE & 0xFFF;
		uart_transmit(&usart_2_handle,(uint8_t*)&id,2);
	}
	else
	{
		bl_send_nack();
	}
}

uint8_t get_rdp(void)
{
	 uint8_t rdp_status = 0;
	 volatile uint32_t* p_rdp_reg = (uint32_t*)0x1FFFF800;
	 rdp_status = (uint8_t)(*p_rdp_reg & 0xFF);
	 return rdp_status;
}

void bl_get_flash_rdp(uint8_t* buffer)
{
	uint8_t rdp = 0;
	uint32_t command_packet_length  = buffer[0] + 1;
	uint32_t host_crc = *(uint32_t*)(&buffer[0] + command_packet_length - 4);
	if (bl_check_crc(&buffer[0],2,host_crc))
	{
		bl_send_ack(1);
		rdp = get_rdp();
		uart_transmit(&usart_2_handle,&rdp,1);
	}
	else
	{
		bl_send_nack();
	}
}

uint8_t is_add_valid(uint32_t addr)
{
	// need to implement
	if (addr >= FLASH_BASEADDR && addr <= FLASH_END)
	{
		return ADDR_VALID;
	}

}
void bl_go_to_addr(uint8_t* buffer)
{
	uint32_t addr = 0;
	uint8_t addr_valid = 0;
	uint32_t msp_value = 0;
	uint32_t command_packet_length  = buffer[0] + 1;
	uint32_t host_crc = *(uint32_t*)(&buffer[0] + command_packet_length - 4);
	if (bl_check_crc(&buffer[0],(command_packet_length - 4),host_crc))
	{
		bl_send_ack(1);
		addr = (*(uint32_t*)&buffer[2]);
		if (is_add_valid(addr) == ADDR_VALID)
		{
			uart_transmit(&usart_2_handle,&addr_valid,1);
			switch(addr)
			{
			case USER_APPLICATION_BASE_ADDR:
				msp_value = *(volatile uint32_t*)addr;
				set_msp(msp_value);
				SCB->VTOR = addr;
				//addr += 1; // Due to T-bit
				void(*jump_to)(void) =  *((uint32_t*)(addr + 4));
				jump_to();
				break;
			default:
				break;
			}

		}
	}
	else
	{
		bl_send_nack();
	}
}

void bl_flash_erase(uint8_t* buffer)
{
	uint8_t flash_erase_status = 0;
	uint32_t command_packet_length  = buffer[0] + 1;
	uint32_t host_crc = *(uint32_t*)(&buffer[0] + command_packet_length - 4);
	if (bl_check_crc(&buffer[0],(command_packet_length - 4),host_crc))
	{
		bl_send_ack(1);
		flash_erase_status = flash_sector_erase();
		uart_transmit(&usart_2_handle,&flash_erase_status,1);
	}
	else
	{
		bl_send_nack();
	}
}
void bl_send_ack(uint32_t length)
{
	uint8_t temp_arr[2];
	temp_arr[0] = BL_ACK;
	temp_arr[1] = (uint8_t)length;
	uart_transmit(&usart_2_handle,temp_arr,2);
}

void bl_send_nack(void)
{
	uint8_t temp_var = BL_NACK;
	uart_transmit(&usart_2_handle,&temp_var,1);
}

uint8_t get_bl_version(void)
{
	return BL_VERSION;
}

