/*
 * crc.h
 *
 *  Created on: Jul 14, 2025
 *      Author: PRASHANT AJABE
 */

#ifndef INC_CRC_H_
#define INC_CRC_H_

#include "stm32f103x.h"




/*
 * CRC
 * AHB bus
 * base addr:0x40023000
 */

void crc_reset(crc_regdef_t* p_crc);
void crc_clock_en(void);
void crc_clock_di(void);
uint32_t crc_cal(crc_regdef_t* p_crc, uint32_t* p_data, uint32_t len);
#endif /* INC_CRC_H_ */
