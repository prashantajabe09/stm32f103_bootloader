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

uint8_t bootloader_bl_get_ver_cmd(uint8_t* buffer)
{

}
// handle_X_command()
// check CRC, if match then send ACK --> obtain the reply --> send reply.
// checl CRC, if not match tehn send NACK --> poll for data again.
// ACK and NACK data is as per our rules and it must match with the host application value.
