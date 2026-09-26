/*
 * port.c
 *
 *  Created on: 2026年9月25日
 *      Author: andyn
 */

/* ----------------------- Includes -----------------------------------------*/

#include "stdint.h"
#include "mb.h"
#include "mbport.h"

#include "main.h"

/*
 * 波特率修改请求标志：
 * eMBRegHoldingCB 检测到写波特率寄存器后置位，
 * 主循环里的 Modbus_BaudApply() 消费。
 */
volatile uint8_t g_bBaudChangePending = 0U;
/* ----------------------- Defines ------------------------------------------*/


#define HOLDINGREG_COUNT 	(sizeof(HoldingREG) / sizeof(uint16_t))

#define INPUTREG_COUNT  	(sizeof(InputREG) / sizeof(uint16_t))


/*
 * 线圈
 *
 * 功能码：
 *     01
 *     05
 *     15(0x0F)
 */
#define REG_COILS_SIZE      10U

uint16_t REG_HOLD_BUF[REG_HOLD_SIZE];
uint16_t REG_INPUT_BUF[REG_INPUT_SIZE];

/*
 * 离散输入
 *
 * 功能码：
 *     02
 */
#define REG_DISC_SIZE       10U


/* ----------------------- Register buffers ---------------------------------*/

/*
 * 线圈
 *
 * 地址：
 *     1 ~ 10
 *
 * 这里按照文档的示例，
 * 一个uint8_t保存一个线圈状态。
 */
static uint8_t REG_COILS_BUF[REG_COILS_SIZE] =
{
    1, 1, 1, 1, 0,
    0, 0, 0, 1, 1
};


/*
 * 离散输入
 *
 * 地址：
 *     1 ~ 10
 */
static uint8_t REG_DISC_BUF[REG_DISC_SIZE] =
{
    1, 1, 1, 1, 0,
    0, 0, 0, 1, 1
};


/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer,
                           USHORT usAddress,
                           USHORT usNRegs)
{
    USHORT i;

    if (pucRegBuffer == NULL)
    {
        return MB_EINVAL;
    }

    if (usAddress == 0 || usNRegs == 0)
    {
        return MB_EINVAL;
    }

    if (usAddress > REG_INPUT_SIZE)
    {
        return MB_ENOREG;
    }

    if (usNRegs > (REG_INPUT_SIZE - usAddress + 1U))
    {
        return MB_ENOREG;
    }

    for (i = 0; i < usNRegs; i++)
    {
        *pucRegBuffer++ =
            (UCHAR)(REG_INPUT_BUF[usAddress - 1U + i] >> 8);

        *pucRegBuffer++ =
            (UCHAR)(REG_INPUT_BUF[usAddress - 1U + i] & 0xFF);
    }

    return MB_ENOERR;
}

/**
 * @brief  保持寄存器读写
 *
 * Function Code:
 *
 *     03 读取
 *     06 写单个
 *     16(0x10) 写多个
 */
eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer,
                             USHORT usAddress,
                             USHORT usNRegs,
                             eMBRegisterMode eMode)
{
    USHORT i;

    if (pucRegBuffer == NULL)
    {
        return MB_EINVAL;
    }

    if (usAddress == 0 || usNRegs == 0)
    {
        return MB_EINVAL;
    }

    if (usAddress > REG_HOLD_SIZE)
    {
        return MB_ENOREG;
    }

    if (usNRegs > (REG_HOLD_SIZE - usAddress + 1U))
    {
        return MB_ENOREG;
    }

    if (eMode == MB_REG_READ)
    {
        for (i = 0; i < usNRegs; i++)
        {
            *pucRegBuffer++ =
                (UCHAR)(REG_HOLD_BUF[usAddress - 1U + i] >> 8);

            *pucRegBuffer++ =
                (UCHAR)(REG_HOLD_BUF[usAddress - 1U + i] & 0xFF);
        }
    }
    else if (eMode == MB_REG_WRITE)
    {
        for (i = 0; i < usNRegs; i++)
        {
            REG_HOLD_BUF[usAddress - 1U + i] =
                ((USHORT)pucRegBuffer[0] << 8) |
                pucRegBuffer[1];

            pucRegBuffer += 2;
        }

        /*
         * 检测波特率寄存器被写。
         *
         * HOLDINGREG->BaudRate 位于 REG_HOLD_BUF[1]，
         * 对应 Modbus 地址 2。
         */
        if ((usAddress <= 2U) && ((usAddress + usNRegs - 1U) >= 2U))
        {
            g_bBaudChangePending = 1U;
        }
    }
    else
    {
        return MB_EINVAL;
    }

    return MB_ENOERR;
}


/**
 * @brief  线圈读写
 *
 * Function Code:
 *
 *     01 读取线圈
 *     05 写单个线圈
 *     15(0x0F) 写多个线圈
 */
eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer,
                           USHORT usAddress,
                           USHORT usNCoils,
                           eMBRegisterMode eMode)
{
    USHORT usRegIndex;

    UCHAR ucBits;
    UCHAR ucState;
    UCHAR ucLoops;


    /* 参数检查 */

    if (pucRegBuffer == NULL)
    {
        return MB_EINVAL;
    }

    /*
     * 地址0非法
     */
    if (usAddress == 0U)
    {
        return MB_ENOREG;
    }

    /*
     * 数量0非法
     */
    if (usNCoils == 0U)
    {
        return MB_ENOREG;
    }

    /*
     * 起始地址越界
     */
    if (usAddress > REG_COILS_SIZE)
    {
        return MB_ENOREG;
    }

    /*
     * 请求数量越界
     */
    if (usNCoils > (REG_COILS_SIZE - usAddress + 1U))
    {
        return MB_ENOREG;
    }


    /*
     * 转换成数组下标
     */
    usRegIndex = usAddress - 1U;


    /*
     * 需要多少个字节
     *
     * 例如：
     *
     * 1 ~ 8   -> 1字节
     * 1 ~ 9   -> 2字节
     */
    ucLoops = (UCHAR)((usNCoils - 1U) / 8U + 1U);


    /*
     * ==============================
     * 写线圈
     * ==============================
     */
    if (eMode == MB_REG_WRITE)
    {
        while (ucLoops != 0U)
        {
            ucState = *pucRegBuffer++;

            ucBits = 0U;

            while ((usNCoils != 0U) && (ucBits < 8U))
            {
                REG_COILS_BUF[usRegIndex] =
                    (uint8_t)((ucState >> ucBits) & 0x01U);

                usRegIndex++;
                usNCoils--;
                ucBits++;
            }

            ucLoops--;
        }
    }


    /*
     * ==============================
     * 读线圈
     * ==============================
     */
    else if (eMode == MB_REG_READ)
    {
        while (ucLoops != 0U)
        {
            ucState = 0U;

            ucBits = 0U;

            while ((usNCoils != 0U) && (ucBits < 8U))
            {
                if (REG_COILS_BUF[usRegIndex] != 0U)
                {
                    ucState |= (UCHAR)(1U << ucBits);
                }

                usRegIndex++;
                usNCoils--;
                ucBits++;
            }

            *pucRegBuffer++ = ucState;

            ucLoops--;
        }
    }


    /*
     * 非法操作模式
     */
    else
    {
        return MB_EINVAL;
    }


    return MB_ENOERR;
}


/**
 * @brief  读取离散输入
 *
 * Function Code:
 *
 *     02
 */
eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer,
                              USHORT usAddress,
                              USHORT usNDiscrete)
{
    USHORT usRegIndex;

    UCHAR ucBits;
    UCHAR ucState;
    UCHAR ucLoops;


    /* 参数检查 */

    if (pucRegBuffer == NULL)
    {
        return MB_EINVAL;
    }

    /*
     * 地址0非法
     */
    if (usAddress == 0U)
    {
        return MB_ENOREG;
    }

    /*
     * 数量0非法
     */
    if (usNDiscrete == 0U)
    {
        return MB_ENOREG;
    }

    /*
     * 起始地址越界
     */
    if (usAddress > REG_DISC_SIZE)
    {
        return MB_ENOREG;
    }

    /*
     * 请求数量越界
     */
    if (usNDiscrete > (REG_DISC_SIZE - usAddress + 1U))
    {
        return MB_ENOREG;
    }


    /*
     * 1-based -> 0-based
     */
    usRegIndex = usAddress - 1U;


    /*
     * 计算需要返回多少字节
     */
    ucLoops = (UCHAR)((usNDiscrete - 1U) / 8U + 1U);


    /*
     * 离散输入只允许读取
     */
    while (ucLoops != 0U)
    {
        ucState = 0U;

        ucBits = 0U;

        while ((usNDiscrete != 0U) && (ucBits < 8U))
        {
            if (REG_DISC_BUF[usRegIndex] != 0U)
            {
                ucState |= (UCHAR)(1U << ucBits);
            }

            usRegIndex++;
            usNDiscrete--;
            ucBits++;
        }

        /*
         * Modbus：
         *
         * 一个字节保存8个离散输入
         * bit0对应第一个输入
         */
        *pucRegBuffer++ = ucState;

        ucLoops--;
    }


    return MB_ENOERR;
}

