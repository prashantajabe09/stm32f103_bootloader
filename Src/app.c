/*
 * app.c
 *
 *  Created on: Jul 9, 2025
 *      Author: PRASHANT AJABE
 */


#include "app.h"

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

void bootloader_read_uart_data(void)
{
	uint8_t receive_length = 0;
	while(1)
	{
		memoey_set(buffer,0,BUFF_SIZE);
		usart_read(&usart_2_handle,buffer[0],1,1000);
		receive_length = buffer[0];
		usart_read(&usart_2_handle,&buffer[1],receive_length,1000);
		switch(buffer[1])
		{
			case BL_GET_VER:
				bootloader_bl_get_ver_cmd(buffer);
				break;
			default:

		}
	}
	// This function will be called in infinite while loop.
	// will poll for the data over the uart.
	// Format of the command is:
	// length to follow (1 byte) + command (1 byte) + CRC (4 bytes)
	// first read one byte, which will give the length of the command.
	// then read the second byte, which will give the command.
	// then implement the switch statement to implement the logic for that particular command.
}

uint8_t bootloader_check_crc(uint8_t* p_data,uint32_t len,uint32_t host_crc)
{
	return crc_check(CRC,p_data,len,host_crc);
}

uint8_t bootloader_bl_get_ver_cmd(uint8_t* buffer)
{
	uint8_t bl_version;
	uint32_t command_packet_length  = buffer[0] + 1;
	uint32_t host_crc = *(uint32_t*)(&buffer[0] + command_packet_length - 4);
	if (bootloader_check_crc(&buffer[0],2,host_crc))
	{
		// send ack
		bootloader_send_ack(1);

		bl_version = get_bl_version();

		uart_transmit(usart_2_handle,&bl_version,1);
	}
	else
	{
		bootloader_send_nack();
	}
}

void bootloader_send_ack(uint32_t length)
{
	uint8_t temp_arr[2];
	temp_arr[0] = BL_ACK;
	temp_arr[1] = length;
	uart_transmit(usart_2_handle,&temp_arr,2);
}

void bootloader_send_nack(void)
{
	uint8_t temp_var = BL_NACK;
	uart_transmit(usart_2_handle,&temp_var,1);
}

uint8_t get_bl_version(void)
{
	return BL_VERSION;
}
// handle_X_command()
// check CRC, if match then send ACK --> obtain the reply --> send reply.
// checl CRC, if not match tehn send NACK --> poll for data again.
// ACK and NACK data is as per our rules and it must match with the host application value.
