/*
 * UART.c
 *
 *  Created on: 17-May-2024
 *      Author: Prashant Ajabe
 */

#include "UART.h"
//uint8_t index;

uint8_t tx_buffer_data[80];
uint8_t rx_buffer_data[80];
uint16_t tx_buffer_data_len = 0;
uint16_t rx_buffer_data_len = 0;
uint8_t tc_buffer[80],tc_buffer_index;
uint8_t txe_buffer[80],txe_buffer_index;
usart_handle usart_2_handle;
usart_handle usart_3_handle;
uint16_t count_interrupt_enterred;
uint16_t arr_length[6];
uint16_t count_ovr;
uint8_t tc_counter = 0;
uint8_t txe_counter = 0;
uint16_t logic_counter1 = 0;
uint16_t logic_counter2 = 0;
uint16_t logic_counter3 = 0;
uint16_t logic_counter4 = 0;
uint16_t logic_counter5 = 0;

static uart_callback_t callback_rx_data = 0;

void uart_2_configure_parameter(void)
{
	usart_2_handle.add_of_usartx = USART2;
	usart_2_handle.usart_configuration.mode = USART_MODE_TXRX;
	usart_2_handle.usart_configuration.no_of_stop_bits = USART_STOPBITS_1;
	usart_2_handle.usart_configuration.data_word_length = USART_WORDLEN_8BITS;
	usart_2_handle.usart_configuration.baudrate = USART_STD_BAUD_115200;
	usart_2_handle.usart_configuration.parity_control = USART_PARITY_DISABLE;
	//p_usart_handle->usart_configuration.dma_transmitter_en = USARTx_DMA_TRANSMITTER_EN;
	//p_usart_handle->usart_configuration.dma_receiver_en = USARTx_DMA_RECEIVER_EN;
	usart_2_handle.tx_buffer = &(tx_buffer_data[0]);
	usart_2_handle.rx_buffer = &(rx_buffer_data[0]);
	usart_2_handle.rx_len = 8;
	usart_init(&usart_2_handle);
}

void uart_3_configure_parameter(void)
{
	usart_3_handle.add_of_usartx = USART3;
	usart_3_handle.usart_configuration.mode = USART_MODE_TXRX;
	usart_3_handle.usart_configuration.no_of_stop_bits = USART_STOPBITS_1;
	usart_3_handle.usart_configuration.data_word_length = USART_WORDLEN_8BITS;
	usart_3_handle.usart_configuration.baudrate = USART_STD_BAUD_115200;
	usart_3_handle.usart_configuration.parity_control = USART_PARITY_DISABLE;
	//p_usart_handle->usart_configuration.dma_transmitter_en = USARTx_DMA_TRANSMITTER_EN;
	//p_usart_handle->usart_configuration.dma_receiver_en = USARTx_DMA_RECEIVER_EN;
	usart_3_handle.tx_buffer = &(tx_buffer_data[0]);
	usart_3_handle.rx_buffer = &(rx_buffer_data[0]);
	usart_3_handle.rx_len = 8;
	usart_init(&usart_3_handle);
}

void usart_init(usart_handle *p_usart_handle)
{
	config_stop_bit(p_usart_handle->add_of_usartx,p_usart_handle->usart_configuration.no_of_stop_bits);

	config_data_word_len(p_usart_handle->add_of_usartx,p_usart_handle->usart_configuration.data_word_length);

	config_parity(p_usart_handle->add_of_usartx,p_usart_handle->usart_configuration.parity_control);

	config_mode(p_usart_handle->add_of_usartx,p_usart_handle->usart_configuration.mode);

	usart_set_baudrate(p_usart_handle->add_of_usartx,p_usart_handle->usart_configuration.baudrate);

	usart_dma_mode_en_di(p_usart_handle->add_of_usartx,p_usart_handle->usart_configuration.dma_transmitter_en);

	usart_dma_mode_en_di(p_usart_handle->add_of_usartx,p_usart_handle->usart_configuration.dma_receiver_en);

	usart_en(p_usart_handle->add_of_usartx);

	//usart_read_interrupt(p_usart_handle->add_of_usartx);

	usart_rx_en(p_usart_handle->add_of_usartx);
}

void usart_set_baudrate(usartx_regdef_t *p_usartx,uint32_t baudrate)
{

	/*
	 * Baudrate = Fck/(16*USARTDIV)
	 * UARTDIV = Fck/(16*Baudrate)
	 *
	 * USART1 - APB2
	 * USART2-5 - APB1
	 * USARTDIV is an unsigned fixed point number that is coded on the USART_BRR register.
	 */
	float temp,fraction;
	uint32_t mantissa;
	// TO DO: ADD CONDITION FOR CHECKING WHICH UART IT IS BEFORE GETTING THE CLOCK OF THE PERIPHERAL.
	uint32_t pclk = apb_get_clock(APB1); // It gets the Clock source for mentioned APB.
	uint32_t div_fraction;
	temp = pclk / (16.0 * baudrate);
	fraction = temp - (int)temp;
	mantissa = temp - fraction;

	div_fraction = round_off((16 * fraction)); // Function to round off the decimal number
	if (div_fraction > 15)
	{
		div_fraction = 0;
		mantissa += 1;
	}
	p_usartx->BRR |= (mantissa << 4);
	p_usartx->BRR |= div_fraction;
}

void uart_transmit(usart_handle *p_usart_handle,uint8_t* p_data, uint8_t len)
{
	p_usart_handle->tx_buffer = p_data;
	p_usart_handle->tx_len = len;
	usart_write_polling(p_usart_handle);
}
void uart_send_string(const char* s)
{
	usart_2_handle.tx_buffer = s;
	usart_write_polling(&usart_2_handle);
}

void uart_sent_uint(uint32_t num)
{
	usart_2_handle.tx_buffer = (uint8_t*)&num;
	usart_write_polling(&usart_2_handle);
}
void usart_write_polling(usart_handle *p_usart_handle)
{

	//p_usart_handle->tx_buffer = tx_buffer_data;
	//usart_tx_en(p_usart_handle->add_of_usartx);
	//while (*p_usart_handle->tx_buffer != '\0')
	while ((p_usart_handle->tx_len)--)
	{
		// 8 bit data len
		// 9 bit data len
		while ( get_flag_status(p_usart_handle->add_of_usartx,USART_FLAG_TXE) == 0);

		if (p_usart_handle->usart_configuration.data_word_length == USART_WORDLEN_9BITS)
		{
			// IF Parity DISABLED
			if (p_usart_handle->usart_configuration.parity_control == USART_PARITY_DISABLE)
			{
				//*pData = buffer;
				p_usart_handle->add_of_usartx->DR = (*p_usart_handle->tx_buffer & (uint16_t)0x1FF);
				p_usart_handle->tx_buffer++;
				p_usart_handle->tx_buffer++;
			}
			// IF Parity ENABLED
			else
			{
				p_usart_handle->add_of_usartx->DR = (*p_usart_handle->tx_buffer & (uint8_t)0xFF);
				p_usart_handle->tx_buffer++;
			}
		}
		else if (p_usart_handle->usart_configuration.data_word_length == USART_WORDLEN_8BITS)
		{
			//p_usart_handle->add_of_usartx->DR = (*buffer & (uint8_t)0xFF);
			//temp_chara = (*buffer);
			// printable character anyhoe is 7-bits, So it doesnt matter if we handle the parity control.
			p_usart_handle->add_of_usartx->DR = *p_usart_handle->tx_buffer;
			p_usart_handle->tx_buffer++;
		}




	}
	// Wait till TC Got completed
	while (get_flag_status(p_usart_handle->add_of_usartx,USART_FLAG_TC) != 1);
	//usart_tx_di(p_usart_handle->add_of_usartx);
}

uint8_t usart_read(usart_handle *p_usart_handle, uint8_t *p_data, uint16_t length,uint32_t timeout)
{
	uint32_t start = get_systick();
	while ( ((get_systick() - start) < timeout) && length > 0 )
	{
		while((get_flag_status(p_usart_handle->add_of_usartx,USART_FLAG_RXNE)))
		{

			if (length-- > 0)
			{
				//print_msg("length:%d",length);
				if (p_usart_handle->usart_configuration.data_word_length == USART_WORDLEN_9BITS)
				{
					if (p_usart_handle->usart_configuration.parity_control == USART_PARITY_DISABLE)
					{
						//*((uint16_t*)(p_usart_handle->rx_buffer)) = ((uint16_t*)(p_usart_handle->add_of_usartx->DR & 0x1FF));
						*((uint16_t*)(p_data)) = (p_usart_handle->add_of_usartx->DR & 0x1FF);
						p_data++;
						p_data++;
					}
					else
					{
						//*(p_usart_handle->rx_buffer) = ((uint8_t*)(p_usart_handle->add_of_usartx->DR & 0xFF));
						*(p_data) = (p_usart_handle->add_of_usartx->DR & 0xFF);
						p_data++;
					}
				}
				else if (p_usart_handle->usart_configuration.data_word_length == USART_WORDLEN_8BITS)
				{
					if (p_usart_handle->usart_configuration.parity_control == USART_PARITY_DISABLE)
					{
						//*(p_usart_handle->rx_buffer) = ((uint8_t*)(p_usart_handle->add_of_usartx->DR & 0xFF));
						*(p_data) = (p_usart_handle->add_of_usartx->DR & 0xFF);
						p_data++;
					}
					else
					{
						//*(p_usart_handle->rx_buffer) = ((uint8_t*)(p_usart_handle->add_of_usartx->DR & 0xFF));
						*(p_data) = (p_usart_handle->add_of_usartx->DR & 0xFF);
						p_data++;
					}
				}
			}

		}
	}
	if (length == 0)
	{
		return USART_READ_OK;
	}
	return length;
}

void usart_en(usartx_regdef_t* p_usartx)
{
	p_usartx->CR1 |= (1 << USART_CR1_EN);
}

void config_stop_bit(usartx_regdef_t* p_usartx, uint8_t stop_bits)
{
	p_usartx->CR2 &=  ~(STOP_BIT_MASK());
	p_usartx->CR2 |=  (stop_bits << USART_CR2_STOP_BIT);
}

void config_data_word_len(usartx_regdef_t* p_usartx, uint8_t data_word_len)
{
	p_usartx->CR1 |= (data_word_len << USART_CR1_DATA_WORD_LEN);
}

void config_parity(usartx_regdef_t* p_usartx, uint8_t parity_type)
{
	if (parity_type == USART_PARITY_EN_EVEN)
	{
		p_usartx->CR1 |= (1 << USART_CR1_PARITY_CONTROL_EN); // Enabled the Parity Control, Even Parity Bydefault Selected.
	}
	else if(parity_type == USART_PARITY_EN_ODD)
	{
		p_usartx->CR1 |= ( (1 << USART_CR1_PARITY_CONTROL_EN) | (1 << USART_CR1_PARITY_SELECT) );
	}
}

void config_mode(usartx_regdef_t* p_usartx, uint8_t mode)
{
	switch (mode)
	{
	case USART_MODE_ONLY_TX:
		p_usartx->CR1 |= (1 << USART_CR1_TX_EN);
		break;
	case USART_MODE_ONLY_RX:
		p_usartx->CR1 |= (1 << USART_CR1_RX_EN);
		break;
	case USART_MODE_TXRX:
		p_usartx->CR1 |= ( (1 << USART_CR1_TX_EN) | (1 << USART_CR1_RX_EN) );
	}
}

uint8_t get_flag_status(usartx_regdef_t *p_usartx, uint8_t flag)
{
	if (p_usartx->SR & flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void usart_interrupt_based_transmit(usart_handle *p_usart_handle)
{
	usart_write_interrupt(p_usart_handle->add_of_usartx);
	usart_tx_en(p_usart_handle->add_of_usartx);
}

void usart_write_interrupt(usartx_regdef_t *p_usartx)
{
	p_usartx->CR1 |= (1 << USART_CR1_TXE_INTEERUPT_EN)| (1 << USART_CR1_TC_INTERRUPT_EN);
	nvic_en_irq(USART2_Global_IRQn);
}

void usart_read_interrupt(usart_handle *p_usart_handle,uint8_t *p_data, uint8_t size)
{
	// init the interrupt at usart level
	p_usart_handle->rx_buffer = p_data;
	p_usart_handle->rx_len = size;
	p_usart_handle->add_of_usartx->CR1 |= (1 << USART_CR1_RXNE_INTERRUPT_EN);// | (1 << USART_CR1_TC_INTERRUPT_EN) );
	// init the interrupt at NVIC level. TO DO: Implement the function for this
	nvic_en_irq(USART2_Global_IRQn);

}

void register_uart_callback(uart_callback_t callback)
{
	callback_rx_data = callback;
}

void usart_interrupt_handling(usart_handle *p_usart_handle)
{
	if (get_flag_status(p_usart_handle->add_of_usartx,USART_FLAG_TC) == 1)
	{
		tc_buffer[tc_buffer_index++] = *(p_usart_handle->tx_buffer);
		if (*(p_usart_handle->tx_buffer) == '\0')
		{
			p_usart_handle->add_of_usartx->CR1 &= ~(1 << USART_CR1_TXE_INTEERUPT_EN);
			p_usart_handle->add_of_usartx->CR1 &= ~(1 << USART_CR1_TC_INTERRUPT_EN);
			p_usart_handle->add_of_usartx->CR1 &= ~(1 << USART_CR1_TX_EN);
		}
	}
	if (get_flag_status(p_usart_handle->add_of_usartx,USART_FLAG_TXE) == 1)
	{
		txe_counter++;
		txe_buffer[txe_buffer_index++] = *(p_usart_handle->tx_buffer);
		if (*(p_usart_handle->tx_buffer) != '\0')
		{
			if (p_usart_handle->usart_configuration.data_word_length == USART_WORDLEN_9BITS)
			{
				// IF Parity DISABLED
				if (p_usart_handle->usart_configuration.parity_control == USART_PARITY_DISABLE)
				{
					uint16_t *p_data = (uint16_t*)p_usart_handle->tx_buffer;
					p_usart_handle->add_of_usartx->DR = (*(p_data) & (uint16_t)0x1FF);
					p_usart_handle->tx_buffer++;
					p_usart_handle->tx_buffer++;
				}
				// IF Parity ENABLED
				else
				{
					p_usart_handle->add_of_usartx->DR = (*(p_usart_handle->tx_buffer) & (uint8_t)0xFF);
					p_usart_handle->tx_buffer++;
				}
			}
			else if (p_usart_handle->usart_configuration.data_word_length == USART_WORDLEN_8BITS)
			{
				p_usart_handle->add_of_usartx->DR = (*p_usart_handle->tx_buffer);
				p_usart_handle->tx_buffer++;
			}
		}
		else
		{
			p_usart_handle->add_of_usartx->CR1 &= ~(1 << USART_CR1_TXE_INTEERUPT_EN);
		}
	}
	if (get_flag_status(p_usart_handle->add_of_usartx,USART_FLAG_RXNE) == 1)
	{
		count_interrupt_enterred = count_interrupt_enterred + 1;

			if (p_usart_handle->usart_configuration.data_word_length == USART_WORDLEN_9BITS)
			{
				if (p_usart_handle->usart_configuration.parity_control == USART_PARITY_DISABLE)
				{
					//*((uint16_t*)(p_usart_handle->rx_buffer)) = ((uint16_t*)(p_usart_handle->add_of_usartx->DR & 0x1FF));
					*((uint16_t*)(p_usart_handle->rx_buffer)) = (p_usart_handle->add_of_usartx->DR & 0x1FF);
					p_usart_handle->rx_buffer++;
					p_usart_handle->rx_buffer++;
				}
				else
				{
					//*(p_usart_handle->rx_buffer) = ((uint8_t*)(p_usart_handle->add_of_usartx->DR & 0xFF));
					*(p_usart_handle->rx_buffer) = (p_usart_handle->add_of_usartx->DR & 0xFF);
					p_usart_handle->rx_buffer++;
				}
			}
			else if (p_usart_handle->usart_configuration.data_word_length == USART_WORDLEN_8BITS)
			{
				if (p_usart_handle->usart_configuration.parity_control == USART_PARITY_DISABLE)
				{
					//*(p_usart_handle->rx_buffer) = ((uint8_t*)(p_usart_handle->add_of_usartx->DR & 0xFF));
					*(p_usart_handle->rx_buffer) = (p_usart_handle->add_of_usartx->DR & 0xFF);
					p_usart_handle->rx_buffer++;
					p_usart_handle->rx_len--;
				}
				else
				{
					//*(p_usart_handle->rx_buffer) = ((uint8_t*)(p_usart_handle->add_of_usartx->DR & 0xFF));
					*(p_usart_handle->rx_buffer) = (p_usart_handle->add_of_usartx->DR & 0xFF);
					p_usart_handle->rx_buffer++;
				}
			}

			if (p_usart_handle->rx_len == 0)
			{
				p_usart_handle->add_of_usartx->CR1 &= ~(1 << USART_CR1_RXNE_INTERRUPT_EN);
				callback_rx_data(rx_buffer_data);
			}
		}
//		else if (p_usart_handle->rx_len == 0)
//		{
//			p_usart_handle->add_of_usartx->CR1 &= ~(1 << USART_CR1_RXNE_INTERRUPT_EN);
//			callback_rx_data(rx_buffer_data);
//		}

}

void usart_tx_en(usartx_regdef_t *p_usartx)
{
	p_usartx->CR1 |= (1 << USART_CR1_TX_EN);
}

void usart_rx_en(usartx_regdef_t *p_usartx)
{
	p_usartx->CR1 |= (1 << USART_CR1_RX_EN);
}

void usart_tx_di(usartx_regdef_t *p_usartx)
{
	p_usartx->CR1 &= ~(1 << USART_CR1_TX_EN);
}

void usart_rx_di(usartx_regdef_t *p_usartx)
{
	p_usartx->CR1 &= ~(1 << USART_CR1_RX_EN);
}

void usart_dma_mode_en_di(usartx_regdef_t *p_usartx,uint8_t en_di)
{
	switch(en_di){
	case USARTx_DMA_TRANSMITTER_EN:
		p_usartx->CR3 |= USARTx_DMA_TRANSMITTER_EN_POS;
		break;
	case USARTx_DMA_TRANSMITTER_DI:
		p_usartx->CR3 &= USARTx_DMA_TRANSMITTER_DI_POS;
		break;
	case USARTx_DMA_RECEIVER_EN:
		p_usartx->CR3 |= USARTx_DMA_RECEIVER_EN_POS;
		break;
	case USARTx_DMA_RECEIVER_DI:
		p_usartx->CR3 &= USARTx_DMA_RECEIVER_DI_POS;
		break;
	}
}

void dma_based_usart_transmission(uint16_t tx_buffer_data_len)
{
	// assign the count
	uint16_t data_len = tx_buffer_data_len;
	dma_counter_set(DMA1,7,data_len);
	// Enable DMA
	usart_tx_en(USART2);
	dma_channel_en(DMA1,7,1);


}
void USART2_IRQHandler(void)
{
	usart_interrupt_handling(&usart_2_handle);
}

