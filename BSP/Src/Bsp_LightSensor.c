/*
 * Bsp_LightSensor.c
 *
 *  Created on: 2026年9月25日
 *      Author: andyn
 */

#include <Inc/Bsp_LightSensor.h>

/**
 * @brief  获取光感状态
 * @note   硬件电路为上拉输入，低电平表示触发
 *
 * @retval LIGHT_SENSOR_TRIGGERED      光感触发
 * @retval LIGHT_SENSOR_NOT_TRIGGERED  光感未触发
 */
LightSensorState Bsp_LightSensor_GetState(void)
{
    if (HAL_GPIO_ReadPin(Light_Sensor_GPIO_Port, Light_Sensor_Pin) == GPIO_PIN_RESET)
    {
        return LIGHT_SENSOR_TRIGGERED;
    }

    return LIGHT_SENSOR_NOT_TRIGGERED;
}


/**
 * @brief  判断光感是否触发
 *
 * @retval 1：触发
 * @retval 0：未触发
 */
uint8_t Bsp_LightSensor_IsTriggered(void)
{
    return (HAL_GPIO_ReadPin(Light_Sensor_GPIO_Port, Light_Sensor_Pin) == GPIO_PIN_RESET);
}

