/*
 * Bsp_Beep.h
 *
 *  Created on: 2026年9月20日
 *      Author: andyn
 */

#ifndef INC_BSP_BEEP_H_
#define INC_BSP_BEEP_H_

#include "gpio.h"
#include "main.h"

void Bsp_BeepInit(void);
void Bsp_BeepOn(void);
void Bsp_BeepOff(void);
void Bsp_BeepToggle(void);

#endif /* INC_BSP_BEEP_H_ */
