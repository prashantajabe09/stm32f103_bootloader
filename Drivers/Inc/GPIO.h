/*
 * GPIO.h
 *
 *  Created on: 13-May-2024
 *      Author: Prashant Ajabe
 */

/*
 * The scope of #define is limited to the file in which it is defined.
 * So, #defines which are created in one source file are not available in a different source file.
 * Typically, #defines which are shared between multiple files are stored in a header file(*.h) which
 * is included in each source file that requires the #define.
 */
#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "stm32f103x.h"



/*
 * Enum are used to assign the names to the constant.
 */
typedef enum
{
	P0_0,P0_1,P0_2,P0_3,P0_4,P0_5,P0_6,P0_7,P0_8,P0_9,P0_10,P0_11,P0_12,P0_13,P0_14,P0_15,//Port A
	P1_0,P1_1,P1_2,P1_3,P1_4,P1_5,P1_6,P1_7,P1_8,P1_9,P1_10,P1_11,P1_12,P1_13,P1_14,P1_15,//Port B
	P2_0,P2_1,P2_2,P2_3,P2_4,P2_5,P2_6,P2_7,P2_8,P2_9,P2_10,P2_11,P2_12,P2_13,P2_14,P2_15,//Port C
}gpio_pins;

typedef enum
{
	IO_ANALOG_CH_2 			= P0_1,
	IO_ANALOG_CH_1 			= P0_0,
	IO_UART_TXD 			= P0_2,
	IO_UART_RXD 			= P0_3,
	SPI1_NSS    			= P0_4,
	SPI1_SCK    			= P0_5,
	SPI1_MISO				= P0_6,
	SPI1_MOSI    			= P0_7,
	IO_PWM_MOTORS_X 		= P0_8,
	IO_SENSOR_IN_TEMP 		= P0_9,
	IO_UNUSED_5 			= P0_10,
	CAN_RX					= P0_11,
	CAN_TX		 			= P0_12,
	IO_UNUSED_8 			= P0_13,
	IO_UNUSED_9 			= P0_14,
	IO_UNUSED_10 			= P0_15,
	PUSH_BUTTON				= P1_0,
	IO_UNUSED_12			= P1_1,
	IO_UNUSED_13			= P1_2,
	IO_UNUSED_14			= P1_3,
	IO_UNUSED_15			= P1_4,
	IO_UNUSED_16			= P1_5,
	IO_KEYPAD_UP_ARROW 		= P1_6,
	IO_KEYPAD_DOWN_ARROW 	= P1_7,
	IO_KEYPAD_LEFT_ARROW 	= P1_8,
	IO_KEYPAD_RIGHT_ARROW 	= P1_9,
	IO_I2C2_CLOCK 			= P1_10,
	IO_I2C2_DATA 			= P1_11,
	IO_HOME_SENSOR_1 		= P1_12,
	IO_HOME_SENSOR_2 		= P1_13,
	IO_MOTOR_DIRECTION_X 	= P1_14,
	IO_MOTOR_DIRECTION_Y 	= P1_15,
	IO_UNUSED_17 			= P2_0,
	IO_UNUSED_18			= P2_1,
	IO_UNUSED_19 			= P2_2,
	IO_UNUSED_20			= P2_3,
	IO_UNUSED_21 			= P2_4,
	IO_UNUSED_22 			= P2_5,
	IO_UNUSED_23			= P2_6,
	IO_UNUSED_24			= P2_7,
	IO_UNUSED_25			= P2_8,
	IO_UNUSED_26			= P2_9,
	IO_UNUSED_27			= P2_10,
	IO_UNUSED_28			= P2_11,
	IO_UNUSED_29			= P2_12,
	IO_TEST_LED 			= P2_13,
	IO_UNUSED_30			= P2_14,
	IO_UNUSED_31			= P2_15,
}io_e;


#define GPIOA				((gpio_regdef_t*)GPIO_BASEADDR)
#define GPIOB				((gpio_regdef_t*)0x40010C00)
#define GPIOC				((gpio_regdef_t*)0x40011000)
#define PORTA	0
#define PORTB	1
#define PORTC	2



typedef enum
{
    MODE_IO_DIR_INPUT,
    MODE_IO_DIR_OUTPUT_10MHZ,
	MODE_IO_DIR_OUTPUT_2MHZ,
	MODE_IO_DIR_OUTPUT_50MHZ,
} io_mode_e;

typedef enum
{
	CONFIG_ANALOG_MODE 								= 0,
	CONFIG_FLOATING_STATE 							= 1,
	CONFIG_INPUT_WITH_PULLUP_PULLDOWN 				= 2,
	CONFIG_RESERVED 								= 3,
	CONFIG_GENERAL_PURPOSE_OUTPUT_PUSHPULL 			= 0,
	CONFIG_GENERAL_PURPOSE_OUTPUT_OPENDRAIN 		= 1,
	CONFIG_ALTERNATE_FUNCTION_OUTPUT_PUSHPULL 		= 2,
	CONFIG_ALTERNATE_FUNCTION_OUTPUT_OPENDRAIN 		= 3,

}io_config_e;

typedef enum{
	PULL_DOWN,
	PULL_UP,
	NO_PUPD,
}io_pull_up_down_e;

typedef enum{
	OUT_STATE_LOW,
	OUT_STATE_HIGH,
}io_out_e;

typedef enum{
	IO_TRIGGER_RISING,
	IO_TRIGGER_FALLING,
}io_trigger_e;

typedef struct{
	io_mode_e mode;
	io_config_e config;
	io_pull_up_down_e pupd;
}io_configuration;

typedef enum{
	STATE_DISABLE,
	STATE_ENABLE,
}state;

typedef struct{
	io_e pin;
	irqn_type irqn;
	io_trigger_e trigger_source;
	state en_or_di;
	uint8_t prority;
}exti_configuration;



//EXTI_Config gpio_PA3_interrupt_config = {IO_BUTTON,EXTI4_IRQn,IO_TRIGGER_RISING,ENABLE,0};
//void gpio_init(GPIO_RegDef_t* port,uint8_t pin, uint8_t dir, uint8_t option);
void io_init(void);
void io_config(io_e io, io_configuration *config);
void io_set_io_mode(io_e io, io_mode_e mode, io_config_e config,io_pull_up_down_e pupd);
void io_set_out(io_e io, io_out_e out);
uint16_t io_get_in(io_e io);
void io_interrupt_config(exti_configuration *config); // pass the structure
void io_toggle(io_e io);
void delay();

#endif /* INC_GPIO_H_ */
