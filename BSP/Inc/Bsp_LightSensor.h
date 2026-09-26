/*
 * Bsp_LightSensor.h
 *
 *  Created on: 2026年9月25日
 *      Author: andyn
 */

#ifndef INC_BSP_LIGHTSENSOR_H_
#define INC_BSP_LIGHTSENSOR_H_

#include "gpio.h"
#include "main.h"

/* 光感状态 */
typedef enum
{
    LIGHT_SENSOR_NOT_TRIGGERED = 0,     // 未触发
    LIGHT_SENSOR_TRIGGERED              // 已触发
} LightSensorState;

/* 获取光感当前状态 */
LightSensorState Bsp_LightSensor_GetState(void);

/* 判断光感是否触发 */
uint8_t Bsp_LightSensor_IsTriggered(void);

#endif /* INC_BSP_LIGHTSENSOR_H_ */
