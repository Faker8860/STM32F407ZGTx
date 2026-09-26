/*
 * Bsp_RS485.c
 *
 *  Created on: 2026年9月25日
 *      Author: andyn
 */

#include <Inc/Bsp_RS485.h>

/**
 * @brief  RS485进入发送模式
 */
void Bsp_RS485_SetTransmit(void)
{
    HAL_GPIO_WritePin(RS485_DE_GPIO_Port,  RS485_DE_Pin, GPIO_PIN_SET);
}

/**
 * @brief  RS485进入接收模式
 */
void Bsp_RS485_SetReceive(void)
{
    HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET);
}

void Bsp_RS485_WriteByte(uint8_t data)
{
    huart2.Instance->DR = data;
}

uint8_t Bsp_RS485_ReadByte(void)
{
    return (uint8_t)(huart2.Instance->DR & 0xFFU);
}
