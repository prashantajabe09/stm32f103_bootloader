

#ifndef SRC_CLK_C_
#define SRC_CLK_C_



#include "Clk.h"

/*
 * Structure of Parameters required for system clock setting
 */
struct domain_clock_setting clock_setting = {HSI,PLL_IN_X_12,PLL_HSI_DIV_2,SYSCLK_NOT_DIV,HCLK_NOT_DIV,HCLK_NOT_DIV};


/*
 * Pointer to system clock parameter structure
 */
struct domain_clock_setting *p = &clock_setting;

/*
 * AHB and APBx prescalar array.
 */
uint16_t ahb_prescaler_1[] = {2,4,8,16,64,128,256,512};
uint16_t apb_prescaler_1[] = {2,4,8,16};

/*
 * PLL Multiplier
 */
uint8_t pll_multiplier[] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,16};

/*****************************************************************************
** Function name:		clock_init
**
** Descriptions:		Set the clock source for SYSCLK
**
** parameters:			Clock Source (enum)
**
** Returned value:	Current Source of the system Clock
**
*****************************************************************************/

void clock_init(void)
{
	ahb_clk_set(p->ahb_prescalar);

	apb1_clk_set(p->apb1_prescalar);

	apb2_clk_set(p->apb2_prescalar);

	pll_clk_config(p->pll_clock_source,p->pll_input_clock_multiplier);

	system_clock_set(p->system_clock_source);

	ahb_peripheral_clock_en(AHB_PERIPHERAL_CLOCK_EN);

	apb2_peripheral_clock_en(APB2_PERIPHERAL_CLOCK_EN);

	apb1_peripheral_clock_en(APB1_PERIPHERAL_CLOCK_EN);

	lsi_clock_en();

}




/*****************************************************************************
** Function name:		System_Clock_Set
**
** Descriptions:		Set the clock source for SYSCLK
**
** parameters:			Clock Source (enum)
**
** Returned value:	Current Source of the system Clock
**
*****************************************************************************/

uint8_t system_clock_set(clock_source_e source)
{
	uint8_t temp = HSI;
	if (source == HSE)
	{
		RCC->CR |= HSE_ON;
		while (!IS_HSE_READY())
		{
			;
		}
		RCC->CFGR &= ~(3 << 0);
		RCC->CFGR |= HSE;
		RCC->CR &= ~(1 << 0);
	}
	else if (source == HSI)
	{
		RCC->CR |= HSI_ON;
		while (!IS_HSI_READY())
		{
			;
		}
		RCC->CFGR &= ~(3 << 0);
		RCC->CFGR |= HSI;
	}
	else if (source == PLL)
	{
		RCC->CR |= PLL_ON;
		while (!IS_PLL_READY())
		{
			;
		}
		//RCC->CFGR &= ~(3 << 0);
		RCC->CFGR |= PLL;
		while ((RCC->CFGR & (3 << 2)) != 8);
		//RCC->CR &= ~(1 << 0);

	}
	return temp;
}
uint8_t get_system_clock(uint8_t target)
{

	return 1;
}


/*****************************************************************************
** Function name:		pll_clk_config
**
** Descriptions:		configure the PLL output Clock.
** 						PLL input CLock ( HSI / 2 or HSE, Multiplication factor must be set before enabling the PLL)
** 						PLL Clock can't exceed the 72 MHz.
** 						If Source * multiplication > 72000000 then return with the error message.
** 						If it is less than the 72 MHz then add that multiplication factor to register.
**
** parameters:			input Clock Source (enum), multiplication factor
**
** Returned value:		status of the function (healthy or error)
**
*****************************************************************************/
uint8_t pll_clk_config(pll_entry_clock_source_e source,pll_input_multiplier_e multiplication_factor)
{
	 RCC->CFGR &= ~(0xF << 18);
	 RCC->CFGR |= (multiplication_factor << 18);
	 RCC->CFGR &= ~(1 << 16);
	 RCC->CFGR |= source;
     return 1;
}

/*****************************************************************************
** Function name:		ahb_clk_set
**
** Descriptions:		configure the prescalar parameter for AHB peripheral
**
** parameters:			AHB prescalar (enum)
**
** Returned value:		void
**
*****************************************************************************/


void ahb_clk_set(ahb_prescalar_option_e factor)
{
	RCC->CFGR &= ~(0xF << AHB_CLOCK_PRESCALAR_OFFSET);
	RCC->CFGR |= (factor << AHB_CLOCK_PRESCALAR_OFFSET);
}



/*****************************************************************************
** Function name:		apb1_clk_set
**
** Descriptions:		configure the prescalar parameter for APB1 peripheral
**
** parameters:			APB prescalar (enum)
**
** Returned value:		void
**
*****************************************************************************/
void apb1_clk_set(apbx_prescalar_options_e factor)
{
	RCC->CFGR &= ~(0x7 << APB1_CLOCK_PRESCALAR_OFFSET);
	RCC->CFGR |= (factor << APB1_CLOCK_PRESCALAR_OFFSET);
}





/*****************************************************************************
** Function name:		apb2_clk_set
**
** Descriptions:		configure the prescalar parameter for APB2 peripheral
**
** parameters:			APB prescalar (enum)
**
** Returned value:		void
**
*****************************************************************************/
void apb2_clk_set(apbx_prescalar_options_e factor)
{
	RCC->CFGR &= ~(0xF << APB2_CLOCK_PRESCALAR_OFFSET);
	RCC->CFGR |= (factor << APB2_CLOCK_PRESCALAR_OFFSET);
}

/*****************************************************************************
** Function name:		apb2_peripheral_clock_en
**
** Descriptions:		Enables the clock to APB2 peripheral
**
** parameters:			macro created for respective peripheral clock enable bit
**
** Returned value:		void
**
*****************************************************************************/
void apb2_peripheral_clock_en(uint32_t clock_en_parameter)
{
	RCC->APB2ENR |= clock_en_parameter;
}





/*****************************************************************************
** Function name:		apb1_peripheral_clock_en
**
** Descriptions:		Enables the clock to APB1 peripheral
**
** parameters:			macro created for respective peripheral clock enable bit
**
** Returned value:		void
**
*****************************************************************************/
void apb1_peripheral_clock_en(uint32_t clock_en_parameter)
{
	RCC->APB1ENR |= clock_en_parameter;
}


void ahb_peripheral_clock_en(uint32_t clock_en_parameter)
{
	RCC->AHBENR |= clock_en_parameter;
}



/*****************************************************************************
** Function name:		apb_get_clock
**
** Descriptions:		Give the Clock Frequency on APBx
**
** parameters:			APBx
**
** Returned value:		Clock Frequency
**
*****************************************************************************/
uint32_t apb_get_clock(uint8_t apb)
{
	uint8_t clock_source;
	uint32_t sysclk, temp,temp2, ahbp, apbp;

	clock_source = ((RCC->CFGR >> 2) & 0x03);
	if (clock_source == SYSCLK_SOURCE_HSI)
	{
		sysclk = 8000000;
	}
	else if (clock_source == SYSCLK_SOURCE_HSE)
	{
		sysclk = 8000000;
	}
	else if (clock_source == SYSCLK_SOURCE_PLL)
	{
		sysclk = get_pll_clock();
	}
	else if (clock_source == SYSCLK_SOURCE_PLL_R)
	{
		sysclk = 1; // study this and write the function for it.
	}

	/*
	 * Get AHB Prescalar Value
	 */
	temp = (RCC->CFGR >> 4) & 0xF;
	if (temp < 8)
	{
		ahbp = 1;
	}
	else
	{
		ahbp = ahb_prescaler_1[temp - 8];
	}

	/*
	 * Get APB1 Prescalar Value
	 */
	if (apb == APB1)
	{
		temp = ( (RCC->CFGR >> 8) & 0x7);
		if (temp < 4)
		{
			apbp = 1;
		}
		else
		{
			apbp = apb_prescaler_1[temp-4];
		}
	}

	/*
	 * get APB2 Prescalar Value
	 */
	if (apb == APB2)
	{
		temp = ((RCC->CFGR >> 11) & 0x7);
		if (temp < 4)
		{
			apbp = 1;
		}
		else
		{
			apbp = apb_prescaler_1[temp-4];
		}
	}
	return (sysclk / ahbp / apbp);

}





uint32_t get_pll_clock(void)
{
	// PLL input Clock source
	// If PLL input clock source is HSE then check PLLXTRE in Register
	uint8_t clock_source = ( (RCC->CFGR >> 16) & 0x01);
	uint32_t multipliation_factor = ( (RCC->CFGR >> 18) & 0x0F);
	uint32_t hse_pll_divider = ( (RCC->CFGR >> 17) & 0x01);
	uint32_t pll_out_clk = 0;
	if (clock_source == PLL_HSI_DIV_2)
	{
		pll_out_clk =  ( (HSI_INPUT / 2) * pll_multiplier[multipliation_factor]);
	}
	else if (clock_source == PLL_HSE)
	{
		pll_out_clk =  ( ( (HSE_INPUT) / (hse_pll_divider + 1) ) * pll_multiplier[multipliation_factor]);
	}
	return pll_out_clk;
}

void lsi_clock_en(void)
{
	RCC->CSR |= (1 << LSI_CLOCK_EN_BIT_POSITION_CSR);
	while(!(RCC->CSR & IS_LSI_CLOCK_READY)){;}
}

#endif /* SRC_CLK_C_ */
