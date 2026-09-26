/*
 * Bsp_Servo.h
 *
 *  Created on: 2026年9月20日
 *      Author: andyn
 */

#ifndef INC_BSP_SERVO_H_
#define INC_BSP_SERVO_H_

#include <stdint.h>
#include "tim.h"
#include "main.h"

void Bsp_ServoInit(void);
void Bsp_ServoSetAngle(uint8_t angle);


#endif /* INC_BSP_SERVO_H_ */
