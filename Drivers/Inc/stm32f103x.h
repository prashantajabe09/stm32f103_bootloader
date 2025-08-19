/*
 * stm32f103x.h
 *
 *  Created on: 17-May-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_STM32F103X_H_
#define INC_STM32F103X_H_


#include <stdint.h>
#include <stdarg.h>




#define APB1_BASEADDR						0x40000000U
#define APB2_BASEADDR						0x40010000U
#define GPIO_BASEADDR					 	0x40010800U
#define RCC_BASEADDR						0x40021000U
#define USART1_BASEADDR						(APB2_BASEADDR + 0x3800)
#define USART2_BASEADDR						(APB1_BASEADDR + 0x4400)
#define USART3_BASEADDR						(APB1_BASEADDR + 0x4800)
#define UART4_BASEADDR						(APB1_BASEADDR + 0x4C00)
#define UART5_BASEADDR						(APB1_BASEADDR + 0x5000)
#define EXTI_BASEADDR						(APB2_BASEADDR + 0x0400)
#define I2C1_BASEADDR						(0x40005400)
#define I2C2_BASEADDR						(0x40005800)
#define SPI1_BASEADDR						(0x40013000)
#define SPI2_BASEADDR						(0x40003800)
#define SPI3_BASEADDR						(0x40003C00)
#define ADC1_BASEADDR						(0x40012400)
//#define EXTI_BASEADDR						(0x40010400)
#define DMA1_BASEADDR						0x40020000
#define DMA2_BASEADDR						0x40020400
#define NVIC_BASEADDR						0xE000E100
#define IWDG_BASEADDR						0x40003000
#define SYSTICK_BASEADDR					0xE000E010
#define CAN1_BASEADDR						0x40006400
#define CAN2_BASEADDR						0x40006800
#define SCB_BASEADDR						0xE000ED00
#define CRC_BASEADDR						0x40023000

#define FLASH_BASEADDR						0x08000000
#define FLASH_PAGE_SIZE						0x400
#define FLASH_SIZE							0x1FFFF
#define FLASH_END							(FLASH_BASEADDR + FLASH_SIZE)
#define EN	  1
#define DI	  0


#define NO_OF_TX_MAILBOX 			3
#define NO_OF_RX_MAILBOX			2
typedef struct {
	uint32_t CR;
	uint32_t CFGR;
	uint32_t CIR;
	uint32_t APB2RSTR;
	uint32_t APB1RSTR;
	uint32_t AHBENR;
	uint32_t APB2ENR;
	uint32_t APB1ENR;
	uint32_t BDCR;
	uint32_t CSR;
}rcc_regdef_t;

//typedef struct{


//}NVIC_RegDef_t;


typedef struct{
	uint32_t IMR;
	uint32_t EMR;
	uint32_t RTSR;
	uint32_t FTSR;
	uint32_t SWIER;
	uint32_t PR;
}exti_regdef_t;

typedef struct
{
	uint32_t CRL;
	uint32_t CRH;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t BRR;
	uint32_t LCKR;
}gpio_regdef_t;

typedef struct
{
	uint32_t SR;
	uint32_t DR;
	uint32_t BRR;
	uint32_t CR1;
	uint32_t CR2;
	uint32_t CR3;
	uint32_t GTPR;
}usartx_regdef_t;

typedef struct
{
	uint32_t CR1;
	uint32_t CR2;
	uint32_t SMCR;
	uint32_t DIER;
	uint32_t SR;
	uint32_t EGR;
	uint32_t CCMR[2];
	uint32_t CCER;
	uint32_t CNT;
	uint32_t PSC;
	uint32_t ARR;
	uint32_t RCR;
	uint32_t CCR[4];
	uint32_t BDTR;
	uint32_t DCR;
	uint32_t DMAR;
}timerx_regdef_t;

typedef struct
{
	uint32_t CR1;
	uint32_t CR2;
	uint32_t OAR1;
	uint32_t OAR2;
	uint32_t DR;
	uint32_t volatile SR1;
	uint32_t volatile SR2;
	uint32_t CCR;
	uint32_t TRISE;
}i2cx_regdef_t;

typedef struct
{
	uint32_t ISR;
	uint32_t IFCR;
	uint32_t CCR1;
	uint32_t CNDTR1;
	uint32_t CPAR1;
	uint32_t CMAR1;
	uint32_t RESERVED1;
	uint32_t CCR2;
	uint32_t CNDTR2;
	uint32_t CPAR2;
	uint32_t CMAR2;
	uint32_t RESERVED2;
	uint32_t CCR3;
	uint32_t CNDTR3;
	uint32_t CPAR3;
	uint32_t CMAR3;
	uint32_t RESERVED3;
	uint32_t CCR4;
	uint32_t CNDTR4;
	uint32_t CPAR4;
	uint32_t CMAR4;
	uint32_t RESERVED4;
	uint32_t CCR5;
	uint32_t CNDTR5;
	uint32_t CPAR5;
	uint32_t CMAR5;
	uint32_t RESERVED5;
	uint32_t CCR6;
	uint32_t CNDTR6;
	uint32_t CPAR6;
	uint32_t CMAR6;
	uint32_t RESERVED6;
	uint32_t CCR7;
	uint32_t CNDTR7;
	uint32_t CPAR7;
	uint32_t CMAR7;
	uint32_t RESERVED7;
}dmax_regdef_dummy_t;


typedef struct{
	uint32_t CCR;
	uint32_t CNDTR;
	uint32_t CPAR;
	uint32_t CMAR;
	uint32_t RESERVED;
}dma1_channel_x_regdef;

typedef struct{
	uint32_t ISR;
	uint32_t IFCR;
	dma1_channel_x_regdef dma_channel[7];
}dma1_regdef_t;

typedef struct{
	uint32_t SR;
	uint32_t CR1;
	uint32_t CR2;
	uint32_t SMPR[2];
	uint32_t JOFR[4];
	uint32_t HTR;
	uint32_t LTR;
	uint32_t SQR[3];
	uint32_t JSQR;
	uint32_t JDR[4];
	uint32_t DR;
}adc_x_regdef_t;

typedef enum IRQn{
	EXTI0_IRQn     				= 6,
	EXTI1_IRQn     				= 7,
	EXTI2_IRQn     				= 8,
	EXTI3_IRQn     				= 9,
	EXTI4_IRQn     				= 10,
	DMA_CHANNEL_2_GLOBAL_IRQn	= 12,
	DMA_CHANNEL_3_GLOBAL_IRQn	= 13,
	DMA_CHANNEL_4_GLOBAL_IRQn   = 14,
	DMA_CHANNEL_5_GLOBAL_IRQn 	= 15,
	DMA_CHANNEL_6_GLOBAL_IRQn   = 16,
	DMA_CHANNEL_7_GLOBAL_IRQn 	= 17,
	ADC_1_2_IRQn				= 18,
	CAN1_TX_IRQn				= 19,
	CAN1_RX_0_IRQn				= 20,
	CAN1_RX_1_IRQn				= 21,
	CAN1_SCE_IRQn				= 22,
	EXTI9_5_IRQn   				= 23,
	I2C2_EV_IRQn				= 33,
	SPI1_GLOBAL_IRQn			= 35,
	EXTI10_15_IRQn 				= 40,
	USART2_Global_IRQn 			= 38,
	TIM1_CC_IRQn     			= 27,
}irqn_type;


typedef struct{
	uint32_t CR1;
	uint32_t CR2;
	uint32_t SR;
	uint32_t DR;
	uint32_t CRCPR;
	uint32_t RXCRCR;
	uint32_t TXCRCR;
	uint32_t I2SCFGR;
	uint32_t I2SPR;
}spix_regdef_t;


typedef struct{
	uint32_t iwdg_kr;
	uint32_t iwdg_pr;
	uint32_t iwdg_rlr;
	uint32_t iwdg_sr;
}iwdg_regdef_t;

typedef struct{
	uint32_t ISER[8];
}nvic_regdef_t;


typedef struct{
	uint32_t CSR;
	uint32_t RVR;
	uint32_t CVR;
	uint32_t CALIB;
}systick_regdef_t;

typedef struct{
	uint32_t MCR;
	uint32_t MSR;
	uint32_t TSR;
	uint32_t RFxR[2];
	uint32_t IER;
	uint32_t ESR;
	uint32_t BTR;
	uint32_t RESERVED_1[88];
	struct{
		uint32_t TIxR;
		uint32_t TDTxR;
		uint32_t TDLxR;
		uint32_t TDHxR;
	}canx_tx_mailbox[NO_OF_TX_MAILBOX];
	struct{
		uint32_t RIxR;
		uint32_t RDTxR;
		uint32_t RDLxR;
		uint32_t RDHxR;
	}can_rx_mailbox[NO_OF_RX_MAILBOX];

	uint32_t  RESERVED_2[12];
	uint32_t FMR;
	uint32_t FM1R;
	uint32_t RESERVED_3;
	uint32_t FS1R;
	uint32_t RESERVED_4;
	uint32_t FFA1R;
	uint32_t RESERVED_5;
	uint32_t FA1R;
	uint32_t RESERVED_6[8];
	union{
		uint32_t u32;
		uint16_t u16[2];
	}FxRi[28];


}can_regdef_t;

typedef struct {
    volatile uint32_t ACTLR;  // Offset: 0x000 - Auxiliary Control Register
} SCnSCB_Type;

#define SCnSCB ((SCnSCB_Type *)0xE000E008UL)

typedef struct{
	uint32_t CPUID;
	uint32_t ICSR;
	uint32_t VTOR;
	uint32_t AIRCR;
	uint32_t SCR;
	uint32_t CCR;
	uint32_t SHPR1;
	uint32_t SHPR2;
	uint32_t SHPR3;
	uint32_t SHCRS;
	uint32_t CFSR;
	//uint32_t MMSR;
	//uint32_t BFSR;
	uint32_t RESERVED;
	uint32_t HFSR;
	uint32_t MMAR;
	uint32_t BFAR;
	uint32_t AFSR;
}scb_regdef_t;

typedef struct{
	uint32_t DR;
	uint32_t IDR;
	uint32_t CR;
}crc_regdef_t;

typedef struct{
	uint32_t ACR;
	uint32_t KEYR;
	uint32_t OPTKEYR;
	uint32_t SR;
	uint32_t CR;
	uint32_t AR;
	uint32_t reserved1;
	uint32_t OBR;
	uint32_t WRPR;

}flash_regdef_t;
#define CRC					((crc_regdef_t*)CRC_BASEADDR)
#define SCB  				((scb_regdef_t*)SCB_BASEADDR)
#define RCC					((rcc_regdef_t*)RCC_BASEADDR)
#define GPIOA				((gpio_regdef_t*)GPIO_BASEADDR)
#define GPIOB				((gpio_regdef_t*)0x40010C00)
#define GPIOC				((gpio_regdef_t*)0x40011000)
#define USART1				((usartx_regdef_t *)USART1_BASEADDR)
#define USART2				((usartx_regdef_t *)USART2_BASEADDR)
#define USART3				((usartx_regdef_t *)USART3_BASEADDR)
#define UART4				((usartx_regdef_t *)UART4_BASEADDR)
#define UART5				((usartx_regdef_t *)UART4_BASEADDR)
#define I2C1				((i2cx_regdef_t *)I2C1_BASEADDR)
#define I2C2				((i2cx_regdef_t *)I2C2_BASEADDR)
#define SPI1				((spix_regdef_t *)SPI1_BASEADDR)
#define SPI2				((spix_regdef_t *)SPI2_BASEADDR)
#define SPI3				((spix_regdef_t *)SPI3_BASEADDR)
#define TIMER1				((timerx_regdef_t *)0x40012C00)
#define EXTI				((exti_regdef_t *)EXTI_BASEADDR)
#define DMA1                ((dma1_regdef_t*)DMA1_BASEADDR)
#define ADC1				((adc_x_regdef_t*)ADC1_BASEADDR)
#define CAN1				((can_regdef_t*)CAN1_BASEADDR)
#define CAN2				((can_regdef_t*)CAN2_BASEADDR)
//#define DMA2				((dmax_regdef_t*)DMA2_BASEADDR)
#define IWDG				((iwdg_regdef_t*)IWDG_BASEADDR)
#define SYSTICK				((systick_regdef_t*)SYSTICK_BASEADDR)
#define NVIC				((nvic_regdef_t*)NVIC_BASEADDR)
#define NVIC_ISER0			((uint32_t*)0xE000E100)
#define NVIC_ISER1			((uint32_t*)(0xE000E100 + 0x04))
#define NVIC_ISER2			((uint32_t*)(0xE000E100 + 0x08))
#define NVIC_ISER3			((uint32_t*)(0xE000E100 + 0x0C))
#define NVIC_ISER4			((uint32_t*)(0xE000E100 + 0x10))
#define NVIC_ISER5			((uint32_t*)(0xE000E100 + 0x14))
#define NVIC_ICER0			((uint32_t*)(0xE000E100 + 0x18))
#define NVIC_ICER1			((uint32_t*)(0xE000E100 + 0x1C))
#define NVIC_ICER2			((uint32_t*)(0xE000E100 + 0x80))
#define FLASH_MEMORY_INTERFACE ((flash_regdef_t*)(0x40022000))

#define DBGMCU_IDCODE	    *((uint32_t*)( 0xE0042000))

#define GPIOA_CLK		(1 << 2)
#define GPIOC_CLK		(1 << 4)
#define TIMER1_CLK		(1 << 11)
#define GPIOA_CLK_EN()		(RCC->APB2ENR |= GPIOA_CLK)
#define GPIOC_CLK_EN()		(RCC->APB2ENR |= GPIOC_CLK)
#define GPIOC_CLK_DI()		(RCC->APB2ENR &= ~GPIOC_CLK)
#define TIMER1_CLK_EN()		(RCC->APB2ENR |=  TIMER1_CLK)
#define TIMER1_CLK_DI()		(RCC->APB2ENR &= ~TIMER1_CLK)


#include <Global.h>
#include <System.h>
#include <GPIO.h>
#include <UART.h>
#include <Clk.h>
#include <common.h>
#include <crc.h>
#include <flash.h>
#endif /* INC_STM32F103X_H_ */
