/*
 * app.c
 *
 *  Created on: Jul 9, 2025
 *      Author: PRASHANT AJABE
 */

#ifndef APP_C_
#define APP_C_

#include "stm32f103x.h"

#define USER_APPLICATION_BASE_ADDR	0x08004000
#define BUFF_SIZE					200

/*
 * Boot loader Commands
 */

#define BL_GET_VER					0x51
#define BL_GET_HELP					0x52
#define BL_GET_CID					0x53
#define BL_GET_RDP_STATUS			0x54
#define BL_GO_TO_ADDR				0x55
#define BL_FLASH_ERASE				0x56
#define BL_MEM_WRITE				0x57
#define BL_EN_R_W_PROTECT			0x58
#define BL_MEM_READ					0x59
#define BL_READ_SECTOR_STATUS		0x5A
#define BL_OTP_READ					0x5B
#define BL_DIS_R_W_PROTECT			0x5C


#define BL_ACK						0xA5
#define BL_NACK						0x7F

#define BL_VERSION					0x10
extern uint8_t buffer[BUFF_SIZE];
void jump_to_user_app(void);
void set_msp(uint32_t msp_value);
void bootloader_read_uart_data(void);
uint8_t get_bl_version(void);
void bootloader_send_nack(void);
void bootloader_send_ack(uint32_t length);
uint8_t bootloader_bl_get_ver_cmd(uint8_t* buffer);
#endif /* APP_C_ */
