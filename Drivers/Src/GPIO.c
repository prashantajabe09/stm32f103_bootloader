/*
 * GPIO.c
 *
 *  Created on: 13-May-2024
 *      Author: Prashant Ajabe
 */


#include "GPIO.h"
uint8_t flag_led_on = 0;

uint8_t pin;
uint8_t port_number;
#define PORTS 		   					3
#define PINS_PER_PORT 					16
#define ARRAY_SIZE(array) 				(sizeof(array) / sizeof(array[0]))
#define PIN_MODE_MASK 					(pin*4)
#define PIN_CONFIG_MASK 				((pin*4) + 2)
#define IO_PORT_OFFSET 					(4u)
#define IO_PORT_MASK 					(0x3u << IO_PORT_OFFSET)
#define IO_PIN_MASK 					(0xFu)
#define UNUSED_PIN_CONFIG 				{MODE_IO_DIR_OUTPUT_10MHZ,CONFIG_GENERAL_PURPOSE_OUTPUT_PUSHPULL,NO_PUPD}


#define LED_ON() 			io_set_out(IO_TEST_LED, OUT_STATE_LOW);
#define LED_OFF() 			io_set_out(IO_TEST_LED, OUT_STATE_HIGH);

static uint8_t io_port(io_e io)
{
    return (io & IO_PORT_MASK) >> IO_PORT_OFFSET;
}

static uint8_t io_pin_bit(gpio_pins io)
{
    return io & IO_PIN_MASK;
}

void delay()
{
	uint32_t i = 0;
	for (i = 0; i <10000; i++);

}
// TO DO: Configure the pins as you progress the code. Next configue for the UART then I2C.
io_configuration io_initial_config[PORTS * PINS_PER_PORT] = {
													  [P0_0] = {MODE_IO_DIR_INPUT,CONFIG_ANALOG_MODE,NO_PUPD},
													  [P0_2] = {MODE_IO_DIR_OUTPUT_50MHZ,CONFIG_ALTERNATE_FUNCTION_OUTPUT_PUSHPULL,NO_PUPD},
													  [P0_3] = {MODE_IO_DIR_INPUT,CONFIG_FLOATING_STATE,NO_PUPD},
													  [P0_8] = {MODE_IO_DIR_OUTPUT_50MHZ,CONFIG_ALTERNATE_FUNCTION_OUTPUT_PUSHPULL,NO_PUPD},
													  [P1_0]= {MODE_IO_DIR_INPUT,CONFIG_INPUT_WITH_PULLUP_PULLDOWN,PULL_UP},
													  [P1_6] = {MODE_IO_DIR_INPUT,CONFIG_INPUT_WITH_PULLUP_PULLDOWN,PULL_UP},
													  [P1_7] = {MODE_IO_DIR_INPUT,CONFIG_INPUT_WITH_PULLUP_PULLDOWN,PULL_UP},
													  [P1_8] = {MODE_IO_DIR_INPUT,CONFIG_INPUT_WITH_PULLUP_PULLDOWN,PULL_UP},
													  [P1_9] = {MODE_IO_DIR_INPUT,CONFIG_INPUT_WITH_PULLUP_PULLDOWN,PULL_UP},
													  [P1_10] = {MODE_IO_DIR_OUTPUT_50MHZ,CONFIG_ALTERNATE_FUNCTION_OUTPUT_OPENDRAIN},
													  [P1_11] = {MODE_IO_DIR_OUTPUT_50MHZ,CONFIG_ALTERNATE_FUNCTION_OUTPUT_OPENDRAIN},
													  [P1_12] = {MODE_IO_DIR_INPUT,CONFIG_INPUT_WITH_PULLUP_PULLDOWN,PULL_UP},
													  [P1_13] = {MODE_IO_DIR_INPUT,CONFIG_INPUT_WITH_PULLUP_PULLDOWN,PULL_UP},
													  // Output
													  [P1_14] = {MODE_IO_DIR_OUTPUT_10MHZ,CONFIG_GENERAL_PURPOSE_OUTPUT_PUSHPULL},
													  [P1_15] = {MODE_IO_DIR_OUTPUT_10MHZ,CONFIG_GENERAL_PURPOSE_OUTPUT_PUSHPULL},
													  [P2_13] = {MODE_IO_DIR_OUTPUT_10MHZ,CONFIG_GENERAL_PURPOSE_OUTPUT_PUSHPULL},
													  [P0_4] = {MODE_IO_DIR_OUTPUT_10MHZ,CONFIG_GENERAL_PURPOSE_OUTPUT_PUSHPULL},
													  [SPI1_SCK] = {MODE_IO_DIR_OUTPUT_50MHZ,CONFIG_ALTERNATE_FUNCTION_OUTPUT_PUSHPULL},
													  [SPI1_MOSI] = {MODE_IO_DIR_OUTPUT_50MHZ,CONFIG_ALTERNATE_FUNCTION_OUTPUT_PUSHPULL},
													  [SPI1_MISO] = {MODE_IO_DIR_INPUT,CONFIG_INPUT_WITH_PULLUP_PULLDOWN,PULL_UP},
													 //[SPI1_NSS] = {MODE_IO_DIR_OUTPUT_50MHZ,CONFIG_ALTERNATE_FUNCTION_OUTPUT_PUSHPULL},
													  //Unused Pins
														[IO_SENSOR_IN_TEMP] = {MODE_IO_DIR_INPUT,CONFIG_INPUT_WITH_PULLUP_PULLDOWN,PULL_UP},
														[IO_UNUSED_5] = UNUSED_PIN_CONFIG,
														[CAN_RX] = {MODE_IO_DIR_INPUT,CONFIG_INPUT_WITH_PULLUP_PULLDOWN,PULL_UP},
														[CAN_TX] = {MODE_IO_DIR_OUTPUT_50MHZ,CONFIG_ALTERNATE_FUNCTION_OUTPUT_PUSHPULL},
														[IO_UNUSED_9] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_10] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_8] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_12] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_13] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_14] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_15] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_16] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_17] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_18] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_19] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_20] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_21] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_22] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_23] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_24] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_25] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_26] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_27] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_28] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_29] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_30] = UNUSED_PIN_CONFIG,
														[IO_UNUSED_30] = UNUSED_PIN_CONFIG,
};


void io_interrupt_config(exti_configuration *config)
{
	uint8_t trigger = config->trigger_source;
	if (config->en_or_di == STATE_ENABLE)
	{

		switch (trigger)
		{
			case IO_TRIGGER_RISING:
			{
				EXTI->RTSR |= (1 << config->pin);
				break;
			}
			case IO_TRIGGER_FALLING:
			{
				EXTI->FTSR |= (1 << config->pin);
				break;
			}
		}
		EXTI->IMR |= (1 << config->pin); //Enable the interrupt on EXTI
		nvic_en_irq(config->irqn);
	}
}


void io_init(void)
{
	for (io_e io = P0_2; io <= ARRAY_SIZE(io_initial_config);io++)
	{
		io_config(io,&io_initial_config[io]);
	}
}

void io_config(io_e io, io_configuration *config)
{
	io_set_io_mode(io,config->mode,config->config,config->pupd);
}

void io_set_io_mode(io_e io, io_mode_e mode, io_config_e config,io_pull_up_down_e pupd)
{
	pin = io_pin_bit(io);
	gpio_regdef_t *Port = (gpio_regdef_t*)(GPIO_BASEADDR + (io_port(io)*0x400));
	if(config == CONFIG_INPUT_WITH_PULLUP_PULLDOWN)
	{
		if (pupd == PULL_UP)
		{
			Port->ODR |= (1 << pin);
		}
		else if (pupd == PULL_DOWN)
		{
			Port->ODR &= ~(1 << pin);
		}
	}
	if (pin < 8)
	{
		Port->CRL &= ~(0xF << PIN_MODE_MASK);
		Port->CRL |= (mode << PIN_MODE_MASK) | (config << (PIN_MODE_MASK + 2));
	}
	else
	{
		pin %= 8;
		Port->CRH &= ~(0xF << PIN_MODE_MASK);
		Port->CRH |= (mode << PIN_MODE_MASK) | (config << (PIN_MODE_MASK+2));
	}
}



void io_set_out(io_e io, io_out_e out)
{
	gpio_regdef_t *Port = (gpio_regdef_t*)(GPIO_BASEADDR + ( io_port(io) *0x400));
	switch(out){
	case OUT_STATE_HIGH:
		Port->ODR &= ~(1 << io_pin_bit(io));
		break;
	case OUT_STATE_LOW:
		Port->ODR |= (1 << io_pin_bit(io));
		break;
	}
}

void io_toggle(io_e io)
{
	gpio_regdef_t *Port = (gpio_regdef_t*)(GPIO_BASEADDR + ( io_port(io) *0x400));
	Port->ODR ^= (1 << 13);
}

uint16_t io_get_in(io_e io)
{
	gpio_regdef_t *Port = (gpio_regdef_t*)(GPIO_BASEADDR + ( io_port(io) *0x400));
	return (Port->IDR & (1 << io_pin_bit(io)));
}


void EXTI4_IRQHandler(void)
{
	if (EXTI->PR & (1 << 4))
	{
		EXTI->PR |= (1 << 4);
		if (!flag_led_on)
		{
			LED_ON();
			flag_led_on = 1;
		}
		else
		{
			LED_OFF();
			flag_led_on = 0;
		}
	}
}
