/*
 * Bsp_AT24C02.c
 *
 *  Created on: 2026年9月25日
 *      Author: andyn
 */

#include <Inc/Bsp_AT24C02.h>

/*
 * CubeMX 生成的 I2C 句柄
 *
 * 如果你的 I2C 不是 I2C1，
 * 这里改成对应的 hi2cX。
 */
extern I2C_HandleTypeDef hi2c1;


/**
 * @brief  EEPROM 初始化
 *
 * @note   CubeMX 已经负责 I2C 外设初始化，
 *         这里主要用于检测 EEPROM 是否在线。
 */
EEPROM_Status Bsp_Eeprom_Init(void)
{
    return Bsp_Eeprom_IsReady();
}


/**
 * @brief  检测 EEPROM 是否在线
 *
 * @note
 * AT24C02 内部写周期期间不会响应设备地址，
 * 因此 HAL_I2C_IsDeviceReady() 也可以用于 ACK Polling。
 */
EEPROM_Status Bsp_Eeprom_IsReady(void)
{
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDR, 3, EEPROM_TIMEOUT);

    if (ret == HAL_OK)
    {
        return EEPROM_OK;
    }

    if (ret == HAL_TIMEOUT)
    {
        return EEPROM_TIMEOUT_ERROR;
    }

    return EEPROM_ERROR;
}


/**
 * @brief  等待 EEPROM 内部写周期完成
 *
 * @note
 * AT24C02 写入后需要进行内部 EEPROM 编程，
 * 数据手册给出的最大写周期约为 5ms。
 */
static EEPROM_Status Bsp_Eeprom_WaitReady(void)
{
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_IsDeviceReady(&hi2c1, EEPROM_I2C_ADDR, 20, EEPROM_TIMEOUT);

    if (ret == HAL_OK)
    {
        return EEPROM_OK;
    }

    if (ret == HAL_TIMEOUT)
    {
        return EEPROM_TIMEOUT_ERROR;
    }

    return EEPROM_ERROR;
}


/**
 * @brief  写一个字节
 */
EEPROM_Status Bsp_Eeprom_WriteByte(uint8_t mem_addr,
                                   uint8_t data)
{
    HAL_StatusTypeDef ret;

    ret = HAL_I2C_Mem_Write(&hi2c1, EEPROM_I2C_ADDR, mem_addr, I2C_MEMADD_SIZE_8BIT, &data,1, EEPROM_TIMEOUT);

    if (ret != HAL_OK)
    {
        return EEPROM_ERROR;
    }

    /*
     * 等待 EEPROM 内部写周期结束
     */
    return Bsp_Eeprom_WaitReady();
}


/**
 * @brief  读取一个字节
 */
EEPROM_Status Bsp_Eeprom_ReadByte(uint8_t mem_addr,  uint8_t *data)
{
    HAL_StatusTypeDef ret;

    if (data == NULL)
    {
        return EEPROM_ERROR;
    }

    ret = HAL_I2C_Mem_Read(&hi2c1, EEPROM_I2C_ADDR, mem_addr, I2C_MEMADD_SIZE_8BIT, data, 1, EEPROM_TIMEOUT);

    if (ret != HAL_OK)
    {
        return EEPROM_ERROR;
    }

    return EEPROM_OK;
}


/**
 * @brief  写多个字节
 *
 * @note
 * AT24C02 页大小为 8 Byte。
 *
 * 如果一次写入跨越页边界，
 * 驱动会自动拆分成多次页写。
 */
EEPROM_Status Bsp_Eeprom_Write(uint8_t mem_addr, uint8_t *pData, uint16_t size)
{
    HAL_StatusTypeDef ret;

    uint16_t remain;
    uint16_t page_remain;
    uint16_t write_size;

    if (pData == NULL || size == 0)
    {
        return EEPROM_ERROR;
    }

    /*
     * 检查地址范围
     */
    if ((uint16_t)mem_addr + size > EEPROM_SIZE)
    {
        return EEPROM_ERROR;
    }

    remain = size;

    while (remain > 0)
    {
        /*
         * 当前地址距离本页结束还剩多少空间
         *
         * 例如：
         *
         * mem_addr = 0x02
         *
         * 0x02 0x03 0x04 0x05 0x06 0x07
         *
         * 剩余 6 Byte
         */
        page_remain = EEPROM_PAGE_SIZE - (mem_addr % EEPROM_PAGE_SIZE);

        /*
         * 本次实际写入长度
         */
        write_size = (remain < page_remain) ? remain : page_remain;

        /*
         * 执行页写
         */
        ret = HAL_I2C_Mem_Write(&hi2c1, EEPROM_I2C_ADDR, mem_addr, I2C_MEMADD_SIZE_8BIT, pData, write_size, EEPROM_TIMEOUT);

        if (ret != HAL_OK)
        {
            return EEPROM_ERROR;
        }

        /*
         * 等待内部 EEPROM 写周期完成
         */
        if (Bsp_Eeprom_WaitReady() != EEPROM_OK)
        {
            return EEPROM_TIMEOUT_ERROR;
        }

        /*
         * 更新地址
         */
        mem_addr += write_size;

        /*
         * 更新数据指针
         */
        pData += write_size;

        /*
         * 更新剩余数据
         */
        remain -= write_size;
    }

    return EEPROM_OK;
}


/**
 * @brief  连续读取多个字节
 *
 * @note
 * AT24C02 顺序读取会自动增加内部地址。
 */
EEPROM_Status Bsp_Eeprom_Read(uint8_t mem_addr, uint8_t *pData, uint16_t size)
{
    HAL_StatusTypeDef ret;

    if (pData == NULL || size == 0)
    {
        return EEPROM_ERROR;
    }

    /*
     * 检查地址范围
     */
    if ((uint16_t)mem_addr + size > EEPROM_SIZE)
    {
        return EEPROM_ERROR;
    }

    ret = HAL_I2C_Mem_Read(&hi2c1, EEPROM_I2C_ADDR, mem_addr,  I2C_MEMADD_SIZE_8BIT, pData, size, EEPROM_TIMEOUT);

    if (ret != HAL_OK)
    {
        return EEPROM_ERROR;
    }

    return EEPROM_OK;
}
