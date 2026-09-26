/*
 * Bsp_LED.c
 *
 *  Created on: 2026年9月20日
 *      Author: andyn
 */

#include <Inc/Bsp_LED.h>

void Bsp_LedInit(void)
{
	Bsp_Led0_Off();
	Bsp_Led1_Off();
}

// LED0 PF9 低电平点亮
void Bsp_Led0_On(void)
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
}
void Bsp_Led0_Off(void)
{
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
}
void Bsp_Led0_Toggle(void)
{
    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
}

// LED1 PF10 低电平点亮
void Bsp_Led1_On(void)
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
}
void Bsp_Led1_Off(void)
{
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
}
void Bsp_Led1_Toggle(void)
{
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
}
