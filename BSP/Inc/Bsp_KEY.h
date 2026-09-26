/*
 * Bsp_KEY.h
 *
 *  Created on: 2026年9月20日
 *      Author: andyn
 */

#ifndef INC_BSP_KEY_H_
#define INC_BSP_KEY_H_

#include "gpio.h"
#include "main.h"

/* 按键定义 */
#define KEY_NONE    0
#define KEY_0       1
#define KEY_1       2
#define KEY_2       3
#define KEY_WK_UP   4

uint8_t Bsp_KEY_Scan(void);


#endif /* INC_BSP_KEY_H_ */
