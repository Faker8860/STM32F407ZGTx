/*
 * Bsp_RS485.h
 *
 *  Created on: 2026年9月25日
 *      Author: andyn
 */

#ifndef INC_BSP_RS485_H_
#define INC_BSP_RS485_H_

#include "gpio.h"
#include "main.h"
#include "usart.h"

void Bsp_RS485_SetTransmit(void);
void Bsp_RS485_SetReceive(void);
void Bsp_RS485_WriteByte(uint8_t data);
uint8_t Bsp_RS485_ReadByte(void);

#endif /* INC_BSP_RS485_H_ */
