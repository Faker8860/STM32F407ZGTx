/*
 * Bsp_Servo.c
 *
 *  Created on: 2026年9月20日
 *      Author: andyn
 */


#include <Inc/Bsp_Servo.h>

void Bsp_ServoInit(void)
{
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}

void Bsp_ServoSetAngle(uint8_t angle)
{
    uint16_t pulse;

    // 限制角度范围
    if (angle > 180)
    {
        angle = 180;
    }

    // 0° -> 500us
    // 90° -> 1500us
    // 180° -> 2500us
    pulse = 500 + ((uint32_t)angle * 2000) / 180;

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pulse);
}
