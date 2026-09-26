/*
 * FreeModbus Libary: MSP430 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "main.h"
#include <Inc/Bsp_RS485.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "usart.h"
/* ----------------------- Defines ------------------------------------------*/

extern UART_HandleTypeDef huart2;


/* ----------------------- Start implementation -----------------------------*/

/**
 * RS485 是否正处于发送模式。
 *
 * vMBPortSerialEnable(TRUE, FALSE) 会在两种场景被调用：
 *   1. 启动时单纯开启接收（此时并没有在发送）
 *   2. 一帧响应发完后，要把 DE 切回接收
 *
 * 这两种场景处理方式不同：场景2必须等 TC 中断（最后一字节真正
 * 发完）才能切 DE，否则会削掉停止位；场景1则应直接开接收。
 * 用这个标志区分两者。
 */
static volatile BOOL bRs485TxActive = FALSE;

/**
 * @brief  Enable/Disable serial receiver/transmitter
 *
 * @param  xRxEnable  接收使能
 * @param  xTxEnable 发送使能
 */
void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    if (xTxEnable)
    {
        /* RS485进入发送模式 */
        Bsp_RS485_SetTransmit();

        /* 发送期间关接收，避免收到自己发出的数据 */
        __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
        __HAL_UART_DISABLE_IT(&huart2, UART_IT_TC);

        /*
         * 用TXE中断启动发送。
         *
         * 注意：不能直接开TC中断！
         * TC标志只有在完整发出过一个字节之后才会置位，
         * 第一帧（以及上一帧TC标志被ISR清掉之后）TC=0，
         * 使能TC中断永远不会触发，第一个字节发不出去。
         *
         * TXE在发送器空闲时天然为1，使能后立即触发中断，
         * 由USART2_IRQHandler驱动发送。
         */
        __HAL_UART_ENABLE_IT(&huart2, UART_IT_TXE);

        bRs485TxActive = TRUE;
    }
    else
    {
        /* 停止发送 */
        __HAL_UART_DISABLE_IT(&huart2, UART_IT_TXE);

        if (xRxEnable)
        {
            if (bRs485TxActive)
            {
                /*
                 * 刚发完一帧：不能立刻把RS485切回接收，
                 * 最后一字节还在移位寄存器里发送，立刻切方向
                 * 会削掉停止位，主机收到 framing error。
                 *
                 * 这里只开TC中断，等最后一字节真正发完，
                 * 由serialTxCompleteISR切回接收。
                 */
                bRs485TxActive = FALSE;
                __HAL_UART_ENABLE_IT(&huart2, UART_IT_TC);
            }
            else
            {
                /*
                 * 启动时单纯开启接收：并没有在发送，
                 * 直接切回接收并开 RXNE 中断。
                 */
                Bsp_RS485_SetReceive();
                __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
            }
        }
        else
        {
            /* 完全停止收发 */
            bRs485TxActive = FALSE;
            __HAL_UART_DISABLE_IT(&huart2, UART_IT_TC);
            __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
        }
    }
}


/**
 * @brief  FreeModbus串口初始化
 *
 * @param  ucPort      串口号
 * @param  ulBaudRate 波特率
 * @param  ucDataBits 数据位
 * @param  eParity    奇偶校验
 *
 * @retval TRUE  初始化成功
 * @retval FALSE 初始化失败
 */
BOOL xMBPortSerialInit(UCHAR ucPort,
                       ULONG ulBaudRate,
                       UCHAR ucDataBits,
                       eMBParity eParity,
                       UCHAR ucStopBits)
{
    BOOL bInitialized = TRUE;

    UNUSED(ucPort);
    UNUSED(ulBaudRate);
    UNUSED(ucDataBits);
    UNUSED(eParity);
    UNUSED(ucStopBits);

    Bsp_RS485_SetReceive();
    bRs485TxActive = FALSE;

    __HAL_UART_DISABLE_IT(&huart2, UART_IT_RXNE);
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_TC);

    return bInitialized;
}


/**
 * @brief  FreeModbus发送一个字节
 *
 * @param  ucByte 要发送的数据
 *
 * @retval TRUE  发送成功
 */
BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    /*
     * USART2的发送数据寄存器
     *
     * F407使用DR寄存器。
     */
	Bsp_RS485_WriteByte((uint8_t)ucByte);

    return TRUE;
}


/**
 * @brief  FreeModbus读取一个字节
 *
 * @param  pucByte 接收数据
 *
 * @retval TRUE
 */
BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    /*
     * 读取USART2接收数据寄存器
     */
	*pucByte = (CHAR)Bsp_RS485_ReadByte();

    return TRUE;
}


/**
 * @brief  USART2接收到一个字节
 */
void serialReceiveOneByteISR(void)
{
    pxMBFrameCBByteReceived();
}


/**
 * @brief  USART2发送完成
 */
void serialSentOneByteISR(void)
{
    pxMBFrameCBTransmitterEmpty();
}


/**
 * @brief  USART2发送完成（TC中断，最后一字节已全部移出）
 */
void serialTxCompleteISR(void)
{
    /* 清TC标志 */
    __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_TC);

    /* 关闭TC中断 */
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_TC);

    /* RS485切回接收 */
    Bsp_RS485_SetReceive();

    /* 开启接收中断 */
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
}


/**
 * @brief  进入临界区
 */
void EnterCriticalSection(void)
{
    __disable_irq();
}


/**
 * @brief  退出临界区
 */
void ExitCriticalSection(void)
{
    __enable_irq();
}
