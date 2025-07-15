/*
 * common.c
 *
 *  Created on: 30-Jul-2024
 *      Author: Prashant Ajabe
 */
#include "common.h"

void mcu_init(void)
{
	clock_init();
	io_init();

	uart_2_configure_parameter();
	uart_3_configure_parameter();

	systick_init();

}

uint32_t round_off(float number)
{
	float fraction;
	uint8_t carry;
	uint32_t realPart = (int)number;
	fraction = number - (int)number;
	if (fraction < 0.50)
	{
		carry = 0;
	}
	else if (fraction >= 0.50)
	{
		carry = 1;
	}

	return (realPart + (uint32_t)carry);
}


void print_msg(char* format,...)
{
//	char str[80];
//	va_list args;
//	va_start(args,format);
//	vsprintf(str,format,args);
//	usart_2_handle.tx_buffer = &(str);
//	usart_write_polling(&usart_2_handle);

	char str[80];
	uint8_t  length = 0;
	memory_set(&str,'\0',80);
	va_list args;
	va_start(args,format);
	vsprintf(str,format,args);
	length = str_len(&str);
	uart_transmit(usart_2_handle,&str,length);

}

void debug_msg(char* format,...)
{
	char str[80];
	uint8_t length = 0;
	memory_set(&str,'\0',80);
	va_list args;
	va_start(args,format);
	vsprintf(str,format,args);
	length = str_len(&str);
	uart_transmit(usart_3_handle,&str,length);

}

void memory_set(void* ptr,int value, int num)
{
	unsigned char* p = (unsigned char*)ptr;
	while(num--)
	{
		*p++ = (unsigned int)value;
	}

}

uint8_t str_len(uint8_t* p_data)
{
	uint8_t length;
	while(*p_data++ != '\0')
		++length;
	return length;
}
