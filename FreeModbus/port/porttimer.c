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
#include "tim.h"

/* ----------------------- Modbus includes ----------------------------------*/

#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/

/*
 * TIM7固定50us产生一次Update中断
 */
#define MODBUS_TIMER_BASE_US    50U


/* ----------------------- Static variables ---------------------------------*/

/*
 * FreeModbus要求的超时时间
 *
 * 单位：50us
 *
 * 例如：
 *     70 -> 70 × 50us = 3.5ms
 */
static USHORT gusTim1Timeout50us = 0;

/*
 * 当前已经经过多少个50us
 */
static USHORT gusTim1Count50us = 0;


/* ----------------------- Static functions ---------------------------------*/

static void prvvTIMERExpiredISR(void);


/* ----------------------- Start implementation -----------------------------*/

/**
 * @brief  初始化Modbus定时器
 *
 * @param  usTim1Timeout50us
 *         超时时间，单位：50us
 *
 * @retval TRUE  初始化成功
 */
BOOL xMBPortTimersInit(USHORT usTim1Timeout50us)
{
    /*
     * 保存FreeModbus计算出的超时时间
     */
    gusTim1Timeout50us = usTim1Timeout50us;

    /*
     * 停止TIM7
     */
    HAL_TIM_Base_Stop_IT(&htim7);

    /*
     * 清零TIM7计数器
     */
    __HAL_TIM_SET_COUNTER(&htim7, 0);

    /*
     * 清除Update中断标志
     */
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);

    /*
     * 清零软件计数器
     */
    gusTim1Count50us = 0;

    return TRUE;
}


/**
 * @brief  启动Modbus超时定时器
 *
 * 每接收到一个Modbus字符，
 * FreeModbus都会重新调用这个函数。
 *
 * 相当于重新开始计算帧间超时时间。
 */
void vMBPortTimersEnable(void)
{
    /*
     * 软件计数器清零
     */
    gusTim1Count50us = 0;

    /*
     * TIM7计数器清零
     */
    __HAL_TIM_SET_COUNTER(&htim7, 0);

    /*
     * 清除可能残留的Update标志
     */
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);

    /*
     * 启动TIM7 + Update中断
     */
    HAL_TIM_Base_Start_IT(&htim7);
}


/**
 * @brief  关闭Modbus超时定时器
 *
 * 一旦检测到帧结束，或者收到新数据，
 * FreeModbus会根据状态关闭/重新启动定时器。
 */
void vMBPortTimersDisable(void)
{
    /*
     * 停止TIM7
     */
    HAL_TIM_Base_Stop_IT(&htim7);

    /*
     * 清零TIM7
     */
    __HAL_TIM_SET_COUNTER(&htim7, 0);

    /*
     * 清零软件计数
     */
    gusTim1Count50us = 0;

    /*
     * 清除Update标志
     */
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
}


/**
 * @brief  Modbus延时函数
 *
 * 当前RTU移植中暂时不使用。
 */
void vMBPortTimersDelay(USHORT usTimeOutMS)
{
    UNUSED(usTimeOutMS);
}


/**
 * @brief  TIM7周期到达回调
 *
 * TIM7每50us进入一次。
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /*
     * 确认是TIM7
     */
    if (htim->Instance == TIM7)
    {
        /*
         * 经过一个50us
         */
        gusTim1Count50us++;

        /*
         * 达到FreeModbus要求的超时时间
         */
        if (gusTim1Count50us >= gusTim1Timeout50us)
        {
            /*
             * 停止TIM7
             *
             * Modbus RTU这里属于一次性超时。
             */
            HAL_TIM_Base_Stop_IT(&htim7);

            /*
             * 软件计数清零
             */
            gusTim1Count50us = 0;

            /*
             * 通知FreeModbus：
             *
             * T3.5已经超时
             */
            prvvTIMERExpiredISR();
        }
    }
}


/**
 * @brief  Modbus定时器超时ISR
 */
static void prvvTIMERExpiredISR(void)
{
    /*
     * 通知FreeModbus定时器超时
     */
    (void)pxMBPortCBTimerExpired();
}

