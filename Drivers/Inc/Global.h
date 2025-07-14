/*
 * Global.h
 *
 *  Created on: 29-Nov-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

/*
 * Global Macros
 */
#define NOT_APPLICABLE 0
#define ENABLE 		   1
#define DISABLE        0
#define SET 		   1
#define RESET          0


#define LED_ON() 			io_set_out(IO_TEST_LED, HIGH)
#define LED_OFF() 			io_set_out(IO_TEST_LED, LOW)
#define LED_TOGGLE()        io_toggle(IO_TEST_LED)



/*
 * Global Variable
 */
extern uint16_t adc_raw;
extern uint16_t capture_value;
extern uint8_t flag_send_data;
extern uint8_t tx_buffer_data[];
extern uint8_t rx_buffer_data[];
extern uint16_t tx_buffer_data_len;
extern uint16_t rx_buffer_data_len;
extern uint8_t tc_buffer[],tc_buffer_index;
extern uint8_t txe_buffer[],txe_buffer_index;
extern uint16_t counter_adc_sampling;
extern uint16_t counter_data_transmit;
extern uint16_t counter_led_toggle;
extern uint32_t counter_systick;
extern uint8_t spi_dma_based_write_complete;
#endif /* INC_GLOBAL_H_ */
