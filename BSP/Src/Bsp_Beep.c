/*
 * Bsp_Beep.c
 *
 *  Created on: 2026年9月20日
 *      Author: andyn
 */

#include <Inc/Bsp_Beep.h>

void Bsp_BeepInit(void)
{
	Bsp_BeepOff();
}

/**
 * @brief 蜂鸣器打开
 */
void Bsp_BeepOn(void)
{
    HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_SET);
}

/**
 * @brief 蜂鸣器关闭
 */
void Bsp_BeepOff(void)
{
    HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_RESET);
}

/**
 * @brief 蜂鸣器翻转
 */
void Bsp_BeepToggle(void)
{
    HAL_GPIO_TogglePin(Beep_GPIO_Port, Beep_Pin);
}
