/*
 * Clk.h
 *
 *  Created on: 22-Jul-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_CLK_H_
#define INC_CLK_H_

#include "stm32f103x.h"


#define HSI_INPUT 8000000
#define HSE_INPUT 8000000


#define APB1 1
#define APB2 2





/*
 * CSR Bit Position
 */
#define LSI_CLOCK_EN_BIT_POSITION_CSR 	 0
#define LSI_CLOCK_READY_BIT_POSITION_CSR 1

#define IS_LSI_CLOCK_READY (1 << LSI_CLOCK_READY_BIT_POSITION_CSR)

/*
 * APB2 Peripheral Clock Enable macros
 */
#define AFIO_CLOCK_EN 				(1 << 0)
#define PORTA_CLOCK_EN				(1 << 2)
#define PORTB_CLOCK_EN				(1 << 3)
#define PORTC_CLOCK_EN				(1 << 4)
#define TIMER1_CLOCK_EN				(1 << 11)
#define SPI1_CLOCK_EN				(1 << 12)
#define ADC1_CLOCK_EN				(1 << 9)
/*
 * APB1 Peripheral Clock Enable Macros
 */
#define USART2_CLOCK_EN 			(1 << 17)
#define I2C2_CLOCK_EN				(1 << 22)
#define CAN_CLOCK_EN				(1 << 25)

/*
 * AHB Peripheral Clock Enable Macro
 */
#define AHB_PERIPHERAL_CLOCK_EN (1 << 0)

/*
 * APB2 Macro to be passed to function to enable the peripheral clock
 */
#define APB2_PERIPHERAL_CLOCK_EN (ADC1_CLOCK_EN | AFIO_CLOCK_EN | PORTA_CLOCK_EN | PORTB_CLOCK_EN | PORTC_CLOCK_EN | TIMER1_CLOCK_EN | SPI1_CLOCK_EN)

/*
 * APB1 Macro to be passed to function to enable the peripheral Clock
 */
#define APB1_PERIPHERAL_CLOCK_EN (USART2_CLOCK_EN | I2C2_CLOCK_EN | CAN_CLOCK_EN)



/*
 * Enum for Clock Sources
 */

typedef enum{
	HSI        = 0,
	HSE        = 1,
	PLL        = 2,
	NO_ALLOWED = 3,
}clock_source_e;


/*
 * Enum for PLL Options
 * The PLL Output Frequency must not exceed 72 MHz
 */
typedef enum{
	PLL_IN_X_2   = 0,
	PLL_IN_X_3   = 1,
	PLL_IN_X_4	 = 2,
	PLL_IN_X_5   = 3,
	PLL_IN_X_6   = 4,
	PLL_IN_X_7   = 5,
	PLL_IN_X_8   = 6,
	PLL_IN_X_9   = 7,
	PLL_IN_X_10  = 8,
	PLL_IN_X_11  = 9,
	PLL_IN_X_12  = 10,
	PLL_IN_X_13  = 11,
	PLL_IN_X_14  = 12,
	PLL_IN_X_15  = 13,
	PLL_IN_X_16  = 14,
}pll_input_multiplier_e;



/*
 * HSE Divider for PLL Entry
 */
typedef enum{
	HSE_DIV_1,
	HSE_DIV_2,
}hse_div_pll_e;

/*
 * PLL Entry Clock source
 */
typedef enum{
	PLL_HSI_DIV_2,
	PLL_HSE,
}pll_entry_clock_source_e;


/*
 * AHB Prescalar Options
 */

typedef enum{
	SYSCLK_NOT_DIV = 0,
	SYSCLK_DIV_2   = 8,
	SYSCLK_DIV_4   = 9,
	SYSCLK_DIV_8   = 10,
	SYSCLK_DIV_16  = 11,
	SYSCLK_DIV_64  = 12,
	SYSCLK_DIV_128 = 13,
	SYSCLK_DIV_256 = 14,
	SYSCLK_DIV_512 = 15,
}ahb_prescalar_option_e;

/*
 * APBx PRescalar Options
 */
typedef enum{
	HCLK_NOT_DIV = 0,
	HCLK_DIV_2 	 =  4,
	HCLK_DIV_4   =  5,
	HCLK_DIV_8   =  6,
	HCLK_DIV_16  = 7,
}apbx_prescalar_options_e;

typedef enum{
	MCO_SYSCLK    = 4,
	MCO_HSI       = 5,
	MCO_HSE		  = 6,
	MCO_PLL_DIV_2 = 7,
}mco_source_e;


#define HSE_ON 		   					(1 << 16)
#define HSE_READY 	   					(1 << 17)
#define IS_HSE_READY() 					(RCC->CR & (HSE_READY))
#define HSI_ON 		   					(1 << 0)
#define HSI_READY 	   					(1 << 1)
#define IS_HSI_READY() 					(RCC->CR & (HSI_READY))
#define PLL_ON		   					(1 << 24)
#define PLL_READY      					(1 << 25)
#define IS_PLL_READY() 					(RCC->CR & (PLL_READY))
#define AHB_CLOCK_PRESCALAR_OFFSET	 	4
#define APB1_CLOCK_PRESCALAR_OFFSET 	8
#define APB2_CLOCK_PRESCALAR_OFFSET 	11
#define MAX_SYSTEM_CLOCK 				72000000

/*
 * System Clock Source Macros
 */
#define SYSCLK_SOURCE_HSI				0
#define SYSCLK_SOURCE_HSE				1
#define SYSCLK_SOURCE_PLL				2
#define SYSCLK_SOURCE_PLL_R				3

/*
 * Clock setting Structure
 */
struct domain_clock_setting{
	clock_source_e system_clock_source;
	pll_input_multiplier_e pll_input_clock_multiplier;
	pll_entry_clock_source_e pll_clock_source;
	ahb_prescalar_option_e ahb_prescalar;
	apbx_prescalar_options_e apb1_prescalar;
	apbx_prescalar_options_e apb2_prescalar;
};

void clock_init(void);
uint8_t system_clock_set(clock_source_e source);
uint8_t get_system_clock(uint8_t target);
uint8_t pll_clk_config(pll_entry_clock_source_e source,pll_input_multiplier_e multiplication_factor);
void ahb_clk_set(ahb_prescalar_option_e factor);
void apb1_clk_set(apbx_prescalar_options_e factor);
void apb2_clk_set(apbx_prescalar_options_e factor);
void ahb_peripheral_clock_en(uint32_t clock_en_parameter);
void apb2_peripheral_clock_en(uint32_t clock_en_parameter);
void apb1_peripheral_clock_en(uint32_t clock_en_parameter);
uint32_t apb_get_clock(uint8_t apb);
uint32_t get_pll_clock(void);
void lsi_clock_en(void);

#endif /* INC_CLK_H_ */
