/*
 * Command.h
 *
 *  Created on: 2026年9月20日
 *      Author: yufan
 */

#ifndef INC_COMMAND_H_
#define INC_COMMAND_H_

#include <Inc/APP_Config.h>
#include <Inc/Bsp_Header.h>

#include "stdio.h"
#include "main.h"

void Command_KeyProcess(void);
void Modbus_BaudApply(void);
void User_Init(void);

extern volatile uint8_t g_bBaudChangePending;


#endif /* INC_COMMAND_H_ */
