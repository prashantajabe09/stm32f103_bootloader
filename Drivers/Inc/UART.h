/*
 * UART.h
 *
 *  Created on: 17-May-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f103x.h"
#define  USART_EN 	     (1 << 13)
#define  USART_TE_EN     (1 <<  3)
#define  USART_RE_EN     (1 << 2)
#define USART_DATALEN_8  8
#define USART_DATALEN_9  9
#define  USART_8BIT 	~(1 << 12)
#define  USART_9BIT		(1 << 12)

extern uint16_t count_interrupt_enterred;
extern uint16_t arr_length[6];
extern uint16_t count_ovr;

/*
 * Bit Position Macros
 */
#define USART_CR2_STOP_BIT 				12
#define USART_CR1_DATA_WORD_LEN 		12
#define USART_CR1_PARITY_CONTROL_EN 	10
#define USART_CR1_PARITY_SELECT			 9
#define USART_CR1_TX_EN 				 3
#define USART_CR1_RX_EN					 2
#define USART_CR1_EN 					13
#define USART_SR1_TXE					 7
#define USART_SR1_RXNE					 5
#define USART_SR1_TC					 6

/* CR1 interrupt flag bit position
 * PE interrupt --> Bit 8, TXE interrupt --> bit 7
 * TC interrupt --> bit 6, RXNE interrupt --> bit 5
 * IDLE interrupt --> bit 4
 */
#define USART_CR1_INTERRUPT_FLAG_EN  0x80

//USART2_TX PA2
//USART2_RX PA3


typedef struct{
	//uint8_t Mode; /*Tx or Rx or Both*/
	uint8_t data_word_length; /*8-bit long data or 9-bit long*/
	uint8_t no_of_stop_bits;
	uint32_t baudrate;
	uint32_t parity_control;
	uint8_t dma_transmitter_en;
	uint8_t dma_receiver_en;
}usartx_config;

typedef struct{
	usartx_regdef_t* add_of_usartx;
	usartx_config usart_configuration;
	uint8_t *tx_buffer; //Pointer to unsigned int 8 bits.
	uint8_t *rx_buffer;
	uint8_t tx_len;
	uint8_t rx_len;
}usart_handle;

extern usart_handle usart_2_handle;

#define USART_FLAG_TXE 		(1 << USART_SR1_TXE)
#define USART_FLAG_RXNE 	(1 << USART_SR1_RXNE)
#define USART_FLAG_TC		(1 << USART_SR1_TC)

/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX      0
#define USART_MODE_ONLY_RX      1
#define USART_MODE_TXRX         2


/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200         1200
#define USART_STD_BAUD_2400         2400
#define USART_STD_BAUD_9600         9600
#define USART_STD_BAUD_19200        19200
#define USART_STD_BAUD_38400        38400
#define USART_STD_BAUD_57600        57600
#define USART_STD_BAUD_115200       115200
#define USART_STD_BAUD_230400       230400
#define USART_STD_BAUD_460800       460800
#define USART_STD_BAUD_921600       921600
#define USART_STD_BAUD_2M           2000000
#define SUART_STD_BAUD_3M           3000000


/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_DISABLE        0
#define USART_PARITY_EN_EVEN        1
#define USART_PARITY_EN_ODD         2


/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS         0
#define USART_WORDLEN_9BITS         1


/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1        0
#define USART_STOPBITS_0_5      1
#define USART_STOPBITS_2        2
#define USART_STOPBITS_1_5      3

/*
 * USART DMA MODE ENABLE/DISABLE BIT POSITION
 */
#define USARTx_DMA_TRANSMITTER_EN_POS 	(1 << 7)
#define USARTx_DMA_TRANSMITTER_DI_POS 	(~(1 << 7))
#define USARTx_DMA_RECEIVER_EN_POS 		(1 << 6)
#define USARTx_DMA_RECEIVER_DI_POS      (~(1 << 6))

/*
 * USART DMA MODE EN OR DI
 */
#define USARTx_DMA_TRANSMITTER_EN	0
#define USARTx_DMA_TRANSMITTER_DI   1
#define USARTx_DMA_RECEIVER_EN		2
#define USARTx_DMA_RECEIVER_DI		3



#define USART_CR1_TXE_INTEERUPT_EN	7
#define USART_CR1_RXNE_INTERRUPT_EN	5
#define USART_CR1_TC_INTERRUPT_EN 	6


#define STOP_BIT_MASK()			(0X03 << USART_CR2_STOP_BIT)

typedef void (*uart_callback_t)(uint8_t* rx_data);

void register_uart_callback(uart_callback_t callback);
void uart_x_configure_parameter(usart_handle *p_usart_handle);
void usart_init(usart_handle *p_usart_handle);
void usart_write_polling(usart_handle *p_usart_handle);
void usart_read(usart_handle *p_usart_handle, uint8_t *p_data, uint16_t length,uint32_t timeout);
void usart_en(usartx_regdef_t *p_usartx);
void usart_set_baudrate(usartx_regdef_t *p_usartx,uint32_t baudrate);
void config_stop_bit(usartx_regdef_t *p_usartx, uint8_t stop_bits);
void config_data_word_len(usartx_regdef_t *p_usartx, uint8_t data_word_len);
void config_parity(usartx_regdef_t *p_usartx, uint8_t parity_type);
void config_mode(usartx_regdef_t *p_usartx, uint8_t mode);
uint8_t get_flag_status(usartx_regdef_t *p_usartx, uint8_t flag);
void usart_write_interrupt(usartx_regdef_t *p_usartx);
void usart_read_interrupt(usart_handle *p_usart_handle,uint8_t *p_data, uint8_t size);
void usart_interrupt_handling(usart_handle *p_usart_handle);
void usart_tx_en(usartx_regdef_t *p_usartx);
void usart_rx_en(usartx_regdef_t *p_usartx);
void usart_tx_di(usartx_regdef_t *p_usartx);
void usart_rx_di(usartx_regdef_t *p_usartx);
void usart_interrupt_based_transmit(usart_handle *p_usart_handle);
void usart_dma_mode_en_di(usartx_regdef_t *p_usartx,uint8_t en_di);
void dma_based_usart_transmission(uint16_t tx_buffer_data_len);
void uart_send_string(const char* s);
void uart_sent_uint(uint32_t num);
#endif /* INC_UART_H_ */
