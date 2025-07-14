/*
 * common.h
 *
 *  Created on: 30-Jul-2024
 *      Author: Prashant Ajabe
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#include "stm32f103x.h"

void mcu_init(void);
uint32_t round_off(float number);
void print_msg(char* format,...);
void memory_set(void* ptr,int value, int num);
#endif /* INC_COMMON_H_ */
