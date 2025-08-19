/*
 * app.c
 *
 *  Created on: Jul 9, 2025
 *      Author: PRASHANT AJABE
 */

#ifndef APP_C_
#define APP_C_

#include "stm32f103x.h"
#include <stddef.h>

extern uint8_t usart_return;
#define USER_APP_FLASH_BASE_ADDR	0x08004000
#define USER_APP_RAM_BASE_ADDR		0x20001400
#define BUFF_SIZE					200

/*
 * Boot loader Commands
 */

#define BL_GET						0x00
#define BL_GET_VER					0x01
#define BL_GET_CID					0x02
#define BL_READ_MEM	 				0x11
#define BL_GO_TO_ADDR				0x21
#define BL_FLASH_MEM_WRITE			0x31
#define BL_FLASH_ERASE				0x43
#define BL_EXTENTED_ERASE			0x44
#define BL_SPECIAL					0x50
#define BL_EXTENDED_SPECIAL			0x51
#define BL_WRITE_PROTECT			0x63
#define BL_WRITE_UNPROTECT			0x73
#define BL_READOUT_PROTECT			0x82
#define BL_READOUT_UNPROTECT		0x92
#define BL_GET_CHECKSUM				0xA1

#define BL_ACK						0x79
#define BL_NACK						0x1F

#define VALID_CMD					0x01
#define INVALID_CMD					0x00

#define MEMORY_SUPPORTED			1
#define MEMORY_NOT_SUPPORTED		0



#define BL_VERSION					0x60
extern uint8_t buffer[BUFF_SIZE];

#define RAM_BASE_ADDRESS 			0x20000000
#define RAM_END_ADDRESS				0x20005000


#define MAX_COMMAND_COUNT			15

typedef enum {
    BL_OK = 0,
    BL_INVALID_CMD,
	BL_PROTECTED_MEMORY,
	BL_CHECKSUM_FAILED,
	BL_ERROR_RX,
    BL_ERROR_TX,
	BL_FLASH_WRITE_FAILED,
	BL_RAM_WRITE_FAILED,
	BL_MEMORY_NOT_SUPPORTED,
	CHECKSUM_NOT_MATCHED = 0,
	CHECKSUM_MATCHED = 1,
	NULL_POINTER,

} bl_status_t;

void jump_to_user_app(uint32_t user_app_base_address);
void set_msp(uint32_t msp_value);
void bl_read_uart_data(void);
uint8_t get_bl_version(void);
void bl_send_nack(void);
void bl_send_ack(uint32_t length);
uint8_t bl_check_crc(uint8_t* p_data,uint32_t len,uint32_t host_crc);
uint8_t execute_flash_mem_write(uint8_t* buffer,uint32_t base_addr,uint32_t payload_len);
uint8_t execute_ram_mem_write(uint8_t* buffer,uint32_t base_addr,uint32_t payload_len);
bl_status_t bl_get_ver_cmd(uint8_t* buffer);
bl_status_t bl_get(uint8_t* buffer);
bl_status_t bl_go_to_addr(uint8_t* buffer);
uint8_t bl_flash_erase(uint8_t* buffer);
bl_status_t bl_get_cid_cmd(uint8_t* buffer);
void bl_get_flash_rdp(uint8_t* buffer);
bl_status_t bl_mem_write(uint8_t* buffer);
bl_status_t bl_verify_checksum(uint8_t* buffer,uint8_t len);
uint8_t is_valid_cmd(uint8_t* rec_cmd);
uint8_t is_protection_active(void);
uint8_t is_supported_memory(uint32_t base_addr);

#endif /* APP_C_ */
