/*
 * Bsp_LED.h
 *
 *  Created on: 2026年9月20日
 *      Author: andyn
 */

#ifndef INC_BSP_LED_H_
#define INC_BSP_LED_H_

#include "gpio.h"
#include "main.h"

void Bsp_Led0_On(void);
void Bsp_Led0_Off(void);
void Bsp_Led0_Toggle(void);

void Bsp_Led1_On(void);
void Bsp_Led1_Off(void);
void Bsp_Led1_Toggle(void);

void Bsp_LedInit(void);

#endif /* INC_BSP_LED_H_ */
