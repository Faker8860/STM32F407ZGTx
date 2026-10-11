/**
 ******************************************************************************
 * @file    Bsp_ST480MC.c
 * @brief   ST480MC 三轴磁力计驱动
 * @author  yufan
 ******************************************************************************
 */

#include "Inc/Bsp_ST480MC.h"
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

/* ----------------------- 内部函数声明 ------------------------------------- */

static HAL_StatusTypeDef ST480MC_WaitFlagSet( I2C_TypeDef *i2c, uint32_t flag, uint32_t timeout);

static HAL_StatusTypeDef ST480MC_ReadBytes(I2C_TypeDef *i2c, uint8_t *buffer, uint8_t length);

static int16_t ST480MC_DecodeRaw(uint16_t raw);


/* ----------------------- 等待 I2C 标志 ----------------------------------- */
static HAL_StatusTypeDef ST480MC_WaitFlagSet(I2C_TypeDef *i2c, uint32_t flag, uint32_t timeout)
{
    uint32_t start = HAL_GetTick();

    while ((i2c->SR1 & flag) == 0U)
    {
        if ((i2c->SR1 & I2C_SR1_BERR) != 0U)
        {
            i2c->SR1 &= ~I2C_SR1_BERR;
            return HAL_ERROR;
        }

        if ((i2c->SR1 & I2C_SR1_ARLO) != 0U)
        {
            i2c->SR1 &= ~I2C_SR1_ARLO;
            return HAL_ERROR;
        }

        if ((i2c->SR1 & I2C_SR1_AF) != 0U)
        {
            i2c->SR1 &= ~I2C_SR1_AF;
            return HAL_ERROR;
        }

        if ((HAL_GetTick() - start) >= timeout)
        {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

/* ----------------------- 读取状态 ----------------------------------------- */
HAL_StatusTypeDef Bsp_ST480MC_ReadStatus(uint8_t *status)
{
    I2C_TypeDef *i2c = I2C1;
    HAL_StatusTypeDef ret;

    if (status == NULL)
    {
        return HAL_ERROR;
    }

    if (hi2c1.State != HAL_I2C_STATE_READY)
    {
        return HAL_BUSY;
    }

    if ((i2c->SR2 & I2C_SR2_BUSY) != 0U)
    {
        return HAL_BUSY;
    }

    i2c->CR1 |= I2C_CR1_ACK;

    /* START + 写地址 */
    i2c->CR1 |= I2C_CR1_START;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_SB, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    i2c->DR = (uint8_t)(ST480MC_I2C_ADDR << 1);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_ADDR, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    (void)i2c->SR2;

    /* 发送 EM 命令 */
    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_TXE, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->DR = ST480MC_CMD_EXIT_MODE;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_BTF, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    /* Repeated START + 读地址 */
    i2c->CR1 |= I2C_CR1_START;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_SB, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    i2c->DR = (uint8_t)((ST480MC_I2C_ADDR << 1) | 1U);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_ADDR, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    /* 单字节接收：NACK + STOP */
    i2c->CR1 &= ~I2C_CR1_ACK;
    (void)i2c->SR1;
    (void)i2c->SR2;

    i2c->CR1 |= I2C_CR1_STOP;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_RXNE, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    *status = (uint8_t)i2c->DR;
    i2c->CR1 |= I2C_CR1_ACK;

    return HAL_OK;

error:
    i2c->CR1 |= I2C_CR1_STOP;
    i2c->CR1 |= I2C_CR1_ACK;
    return ret;
}

/* ----------------------- 寄存器读取 --------------------------------------- */
HAL_StatusTypeDef Bsp_ST480MC_ReadRegister(uint8_t reg_addr, uint8_t *status, uint16_t *reg_value)
{
    I2C_TypeDef *i2c = I2C1;
    HAL_StatusTypeDef ret;
    uint8_t data[3];

    if ((status == NULL) || (reg_value == NULL))
        return HAL_ERROR;

    if (reg_addr > 0x09U)
        return HAL_ERROR;

    if (hi2c1.State != HAL_I2C_STATE_READY)
        return HAL_BUSY;

    if ((i2c->SR2 & I2C_SR2_BUSY) != 0U)
        return HAL_BUSY;

    i2c->CR1 |= I2C_CR1_ACK;

    /* START + 写地址 */
    i2c->CR1 |= I2C_CR1_START;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_SB,ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    i2c->DR = (uint8_t)(ST480MC_I2C_ADDR << 1);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_ADDR, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    (void)i2c->SR2;

    /* 发送 RR 命令和寄存器地址 */
    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_TXE, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->DR = ST480MC_CMD_READ_REGISTER;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_TXE, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->DR = (uint8_t)(reg_addr << 2);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_BTF, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    /* Repeated START + 读地址 */
    i2c->CR1 |= I2C_CR1_START;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_SB, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    i2c->DR = (uint8_t)((ST480MC_I2C_ADDR << 1) | 1U);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_ADDR, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    /* 三字节接收：Status + High + Low */
    (void)i2c->SR1;
    (void)i2c->SR2;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_BTF, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->CR1 &= ~I2C_CR1_ACK;
    data[0] = (uint8_t)i2c->DR;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_BTF, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->CR1 |= I2C_CR1_STOP;

    data[1] = (uint8_t)i2c->DR;
    data[2] = (uint8_t)i2c->DR;

    *status = data[0];
    *reg_value = ((uint16_t)data[1] << 8) | data[2];

    i2c->CR1 |= I2C_CR1_ACK;

    return HAL_OK;

error:
    i2c->CR1 |= I2C_CR1_STOP;
    i2c->CR1 |= I2C_CR1_ACK;
    return ret;
}

/* ----------------------- 寄存器写入 --------------------------------------- */
HAL_StatusTypeDef Bsp_ST480MC_WriteRegister(uint8_t reg_addr, uint16_t reg_value, uint8_t *status)
{
    I2C_TypeDef *i2c = I2C1;
    HAL_StatusTypeDef ret;

    if (status == NULL)
        return HAL_ERROR;

    if (reg_addr > 0x09U)
        return HAL_ERROR;

    if (hi2c1.State != HAL_I2C_STATE_READY)
        return HAL_BUSY;

    if ((i2c->SR2 & I2C_SR2_BUSY) != 0U)
        return HAL_BUSY;

    i2c->CR1 |= I2C_CR1_ACK;
    i2c->CR1 |= I2C_CR1_START;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_SB, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    i2c->DR = (uint8_t)(ST480MC_I2C_ADDR << 1);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_ADDR, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    (void)i2c->SR2;

    /* WR 命令 */
    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_TXE, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->DR = ST480MC_CMD_WRITE_REGISTER;

    /* 数据高字节 */
    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_TXE, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->DR = (uint8_t)(reg_value >> 8);

    /* 数据低字节 */
    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_TXE, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->DR = (uint8_t)(reg_value & 0xFFU);

    /* 寄存器地址 */
    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_TXE, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->DR = (uint8_t)(reg_addr << 2);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_BTF, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    /* 读取写入后的状态 */
    i2c->CR1 |= I2C_CR1_START;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_SB, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    i2c->DR = (uint8_t)((ST480MC_I2C_ADDR << 1) | 1U);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_ADDR, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->CR1 &= ~I2C_CR1_ACK;
    (void)i2c->SR1;
    (void)i2c->SR2;
    i2c->CR1 |= I2C_CR1_STOP;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_RXNE, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    *status = (uint8_t)i2c->DR;
    i2c->CR1 |= I2C_CR1_ACK;

    return HAL_OK;

error:
    i2c->CR1 |= I2C_CR1_STOP;
    i2c->CR1 |= I2C_CR1_ACK;
    return ret;
}

/* ----------------------- I2C 多字节接收 ----------------------------------- */
/*
 * 注意：
 * 此函数沿用当前工程已经使用的 STM32F4 I2C 接收时序。
 * 修改接收长度或 I2C 外设配置后，需要重新验证时序。
 */
static HAL_StatusTypeDef ST480MC_ReadBytes(I2C_TypeDef *i2c, uint8_t *buffer, uint8_t length)
{
    HAL_StatusTypeDef ret;
    uint8_t i;

    if ((buffer == NULL) || (length < 3U))
        return HAL_ERROR;

    for (i = 0U; i < (uint8_t)(length - 3U); i++)
    {
        ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_RXNE, ST480MC_TIMEOUT_MS);
        if (ret != HAL_OK)
            goto error;

        buffer[i] = (uint8_t)i2c->DR;
    }

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_BTF, ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->CR1 &= ~I2C_CR1_ACK;

    buffer[length - 3U] = (uint8_t)i2c->DR;

    i2c->CR1 |= I2C_CR1_STOP;

    buffer[length - 2U] = (uint8_t)i2c->DR;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_RXNE,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    buffer[length - 1U] = (uint8_t)i2c->DR;

    i2c->CR1 |= I2C_CR1_ACK;

    return HAL_OK;

error:
    i2c->CR1 |= I2C_CR1_STOP;
    i2c->CR1 |= I2C_CR1_ACK;
    return ret;
}

/* ----------------------- 读取测量结果 ------------------------------------- */
HAL_StatusTypeDef Bsp_ST480MC_ReadMeasurement(uint8_t *status, uint16_t *temperature, uint16_t *mag_x, uint16_t *mag_y, uint16_t *mag_z)
{
    I2C_TypeDef *i2c = I2C1;
    HAL_StatusTypeDef ret;
    uint8_t data[9];

    if ((status == NULL) ||
        (temperature == NULL) ||
        (mag_x == NULL) ||
        (mag_y == NULL) ||
        (mag_z == NULL))
    {
        return HAL_ERROR;
    }

    if (hi2c1.State != HAL_I2C_STATE_READY)
        return HAL_BUSY;

    if ((i2c->SR2 & I2C_SR2_BUSY) != 0U)
        return HAL_BUSY;

    i2c->CR1 |= I2C_CR1_ACK;

    /* START + 写地址 */
    i2c->CR1 |= I2C_CR1_START;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_SB,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    i2c->DR = (uint8_t)(ST480MC_I2C_ADDR << 1);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_ADDR,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    (void)i2c->SR2;

    /* 发送 RM 命令 */
    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_TXE,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->DR = ST480MC_CMD_READ_MEASUREMENT;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_BTF,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    /* Repeated START + 读地址 */
    i2c->CR1 |= I2C_CR1_START;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_SB,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    i2c->DR = (uint8_t)((ST480MC_I2C_ADDR << 1) | 1U);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_ADDR,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    (void)i2c->SR2;

    /* Status + 温度 + X + Y + Z，共9字节 */
    ret = ST480MC_ReadBytes(i2c, data, 9U);
    if (ret != HAL_OK)
        goto error;

    *status = data[0];

    *temperature = ((uint16_t)data[1] << 8) | data[2];
    *mag_x = ((uint16_t)data[3] << 8) | data[4];
    *mag_y = ((uint16_t)data[5] << 8) | data[6];
    *mag_z = ((uint16_t)data[7] << 8) | data[8];

    return HAL_OK;

error:
    i2c->CR1 |= I2C_CR1_STOP;
    i2c->CR1 |= I2C_CR1_ACK;
    return ret;
}

/* ----------------------- 启动单次测量 ------------------------------------- */
HAL_StatusTypeDef Bsp_ST480MC_StartMeasurement(uint8_t *status)
{
    I2C_TypeDef *i2c = I2C1;
    HAL_StatusTypeDef ret;

    if (status == NULL)
        return HAL_ERROR;

    if (hi2c1.State != HAL_I2C_STATE_READY)
        return HAL_BUSY;

    if ((i2c->SR2 & I2C_SR2_BUSY) != 0U)
        return HAL_BUSY;

    i2c->CR1 |= I2C_CR1_ACK;

    /* START + 写地址 */
    i2c->CR1 |= I2C_CR1_START;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_SB,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    i2c->DR = (uint8_t)(ST480MC_I2C_ADDR << 1);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_ADDR,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    (void)i2c->SR2;

    /* 发送 SM 命令 */
    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_TXE,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    i2c->DR = ST480MC_CMD_START_MEASUREMENT;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_BTF,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    /* Repeated START + 读地址 */
    i2c->CR1 |= I2C_CR1_START;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_SB,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    (void)i2c->SR1;
    i2c->DR = (uint8_t)((ST480MC_I2C_ADDR << 1) | 1U);

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_ADDR,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    /* 单字节接收状态 */
    i2c->CR1 &= ~I2C_CR1_ACK;
    (void)i2c->SR1;
    (void)i2c->SR2;
    i2c->CR1 |= I2C_CR1_STOP;

    ret = ST480MC_WaitFlagSet(i2c, I2C_SR1_RXNE,
                              ST480MC_TIMEOUT_MS);
    if (ret != HAL_OK)
        goto error;

    *status = (uint8_t)i2c->DR;
    i2c->CR1 |= I2C_CR1_ACK;

    return HAL_OK;

error:
    i2c->CR1 |= I2C_CR1_STOP;
    i2c->CR1 |= I2C_CR1_ACK;
    return ret;
}

/* ----------------------- 数据解码与磁场换算 ------------------------------- */
/**
 * @brief 16位二进制补码解码
 */
static int16_t ST480MC_DecodeRaw(uint16_t raw)
{
    return (int16_t)raw;
}

/**
 * @brief 将原始磁场数据转换为mG
 *
 * X/Y：1.5 mG/LSB
 * Z：  2.5 mG/LSB
 *
 * 1 mG = 0.1 uT
 */
void Bsp_ST480MC_ConvertMagneticField(
    uint16_t raw_x,
    uint16_t raw_y,
    uint16_t raw_z,
    float *bx_mG,
    float *by_mG,
    float *bz_mG)
{
    int16_t x;
    int16_t y;
    int16_t z;

    if ((bx_mG == NULL) ||
        (by_mG == NULL) ||
        (bz_mG == NULL))
    {
        return;
    }

    x = ST480MC_DecodeRaw(raw_x);
    y = ST480MC_DecodeRaw(raw_y);
    z = ST480MC_DecodeRaw(raw_z);

    *bx_mG = (float)x * 1.5f;
    *by_mG = (float)y * 1.5f;
    *bz_mG = (float)z * 2.5f;
}

/* ----------------------- 寄存器地址测试 ----------------------------------- */

void ST480MC_AddressTest(void)
{
    uint8_t status = 0U;
    uint16_t reg_value = 0U;
    HAL_StatusTypeDef ret;
    uint8_t reg;

    for (reg = 0U; reg <= 2U; reg++)
    {
        ret = Bsp_ST480MC_ReadRegister(reg, &status, &reg_value);

        if (ret == HAL_OK)
        {
            printf("REG[0x%02X] = 0x%04X, STATUS = 0x%02X\r\n",
                   (unsigned int)reg,
                   (unsigned int)reg_value,
                   (unsigned int)status);
        }
        else
        {
            printf("REG[0x%02X] read failed: %d\r\n",
                   (unsigned int)reg,
                   (int)ret);
        }

        HAL_Delay(100U);
    }
}

/* ----------------------- 连续测量测试 ------------------------------------- */

void TESt0(void)
{
    uint8_t i;

    for (i = 0U; i < 20U; i++)
    {
        uint8_t sm_status = 0U;
        uint8_t rm_status = 0U;

        uint16_t temperature = 0U;
        uint16_t mag_x = 0U;
        uint16_t mag_y = 0U;
        uint16_t mag_z = 0U;

        int16_t signed_temperature;
        int16_t signed_x;
        int16_t signed_y;
        int16_t signed_z;

        float bx_mG = 0.0f;
        float by_mG = 0.0f;
        float bz_mG = 0.0f;

        HAL_StatusTypeDef ret;

        /* 1. 启动单次测量 */
        ret = Bsp_ST480MC_StartMeasurement(&sm_status);

        if ((ret != HAL_OK) ||
            ((sm_status & 0x30U) != 0x20U))
        {
            printf("[%u] SM failed: ret=%d, status=0x%02X\r\n",
                   (unsigned int)i,
                   (int)ret,
                   (unsigned int)sm_status);

            HAL_Delay(300U);
            continue;
        }

        /* 保留已经验证成功的等待时间 */
        HAL_Delay(300U);

        /* 2. 读取测量结果 */
        ret = Bsp_ST480MC_ReadMeasurement(
            &rm_status,
            &temperature,
            &mag_x,
            &mag_y,
            &mag_z);

        if (ret != HAL_OK)
        {
            printf("[%u] RM read failed: %d\r\n",
                   (unsigned int)i,
                   (int)ret);
            continue;
        }

        if ((rm_status & 0x10U) != 0U)
        {
            printf("[%u] RM device error: status=0x%02X\r\n",
                   (unsigned int)i,
                   (unsigned int)rm_status);
            continue;
        }

        if ((rm_status & 0x03U) != 0x03U)
        {
            printf("[%u] RM data length unexpected: status=0x%02X\r\n",
                   (unsigned int)i,
                   (unsigned int)rm_status);
            continue;
        }

        /* 3. 解码有符号值 */
        signed_temperature = ST480MC_DecodeRaw(temperature);
        signed_x = ST480MC_DecodeRaw(mag_x);
        signed_y = ST480MC_DecodeRaw(mag_y);
        signed_z = ST480MC_DecodeRaw(mag_z);

        /* 4. 输出原始值 */
        printf("[%u] RAW T=%04X X=%04X Y=%04X Z=%04X status=0x%02X\r\n",
               (unsigned int)i,
               (unsigned int)temperature,
               (unsigned int)mag_x,
               (unsigned int)mag_y,
               (unsigned int)mag_z,
               (unsigned int)rm_status);

        /* 5. 输出补码解码结果 */
        printf("    Signed: T=%d X=%d Y=%d Z=%d\r\n",
               (int)signed_temperature,
               (int)signed_x,
               (int)signed_y,
               (int)signed_z);

        /* 6. 输出各通道高字节bit7 */
        printf("    Sign: T=%u X=%u Y=%u Z=%u\r\n",
               (unsigned int)((temperature >> 15) & 1U),
               (unsigned int)((mag_x >> 15) & 1U),
               (unsigned int)((mag_y >> 15) & 1U),
               (unsigned int)((mag_z >> 15) & 1U));

        /* 7. 换算磁场，单位mG */
        Bsp_ST480MC_ConvertMagneticField(
            mag_x,
            mag_y,
            mag_z,
            &bx_mG,
            &by_mG,
            &bz_mG);

        /* 8. 输出磁场，单位mG和uT */
        printf("    Magnetic field:\r\n");

        printf("        X = %+.2f mG (%+.2f uT)\r\n",
               (double)bx_mG,
               (double)(bx_mG * 0.1f));

        printf("        Y = %+.2f mG (%+.2f uT)\r\n",
               (double)by_mG,
               (double)(by_mG * 0.1f));

        printf("        Z = %+.2f mG (%+.2f uT)\r\n",
               (double)bz_mG,
               (double)(bz_mG * 0.1f));

        printf("\r\n");
    }
}

/**
 * @brief ST480MC连续采集任务
 * @note  非阻塞状态机，每次调用只执行一个状态
 *        串口输出CSV格式，方便XCOM保存数据
 */
void Bsp_ST480MC_CollectTask(void)
{
    typedef enum
    {
        COLLECT_START = 0,
        COLLECT_WAIT,
        COLLECT_READ,
        COLLECT_ERROR_WAIT
    } CollectState_t;

    static CollectState_t state = COLLECT_START;
    static uint32_t state_tick = 0U;
    static uint32_t sample_count = 0U;
    static uint32_t last_sample_tick = 0U;
    static uint8_t first_sample = 1U;
    static uint8_t csv_header_printed = 0U;

    uint8_t sm_status = 0U;
    uint8_t rm_status = 0U;

    uint16_t temperature = 0U;
    uint16_t mag_x = 0U;
    uint16_t mag_y = 0U;
    uint16_t mag_z = 0U;

    int16_t signed_x;
    int16_t signed_y;
    int16_t signed_z;

    uint32_t current_tick;
    uint32_t interval_ms;

    HAL_StatusTypeDef ret;

    current_tick = HAL_GetTick();

    switch (state)
    {
        case COLLECT_START:

            ret = Bsp_ST480MC_StartMeasurement(&sm_status);

            if ((ret != HAL_OK) ||
                ((sm_status & 0x30U) != 0x20U))
            {
                printf("SM_ERROR,%lu,%d,0x%02X\r\n",
                       (unsigned long)sample_count,
                       (int)ret,
                       (unsigned int)sm_status);

                state_tick = current_tick;
                state = COLLECT_ERROR_WAIT;
                break;
            }

            state_tick = current_tick;
            state = COLLECT_WAIT;
            break;

        case COLLECT_WAIT:

            if ((uint32_t)(current_tick - state_tick) >= 300U)
            {
                state = COLLECT_READ;
            }

            break;

        case COLLECT_READ:

            ret = Bsp_ST480MC_ReadMeasurement(&rm_status,
                                              &temperature,
                                              &mag_x,
                                              &mag_y,
                                              &mag_z);

            if (ret != HAL_OK)
            {
                printf("RM_ERROR,%lu,%d\r\n",
                       (unsigned long)sample_count,
                       (int)ret);

                state_tick = HAL_GetTick();
                state = COLLECT_ERROR_WAIT;
                break;
            }

            if (((rm_status & 0x10U) != 0U) ||
                ((rm_status & 0x03U) != 0x03U))
            {
                printf("STATUS_ERROR,%lu,0x%02X\r\n",
                       (unsigned long)sample_count,
                       (unsigned int)rm_status);

                state_tick = HAL_GetTick();
                state = COLLECT_ERROR_WAIT;
                break;
            }

            signed_x = ST480MC_DecodeRaw(mag_x);
            signed_y = ST480MC_DecodeRaw(mag_y);
            signed_z = ST480MC_DecodeRaw(mag_z);

            current_tick = HAL_GetTick();

            if (first_sample != 0U)
            {
                interval_ms = 0U;
                first_sample = 0U;
            }
            else
            {
                interval_ms = current_tick - last_sample_tick;
            }

            last_sample_tick = current_tick;

            if (csv_header_printed == 0U)
            {
                printf("Sample,Tick_ms,Interval_ms,RawT,X,Y,Z,Status\r\n");
                csv_header_printed = 1U;
            }

            printf("%lu,%lu,%lu,%u,%d,%d,%d,%u\r\n",
                   (unsigned long)sample_count,
                   (unsigned long)current_tick,
                   (unsigned long)interval_ms,
                   (unsigned int)temperature,
                   (int)signed_x,
                   (int)signed_y,
                   (int)signed_z,
                   (unsigned int)rm_status);

            sample_count++;
            state = COLLECT_START;
            break;

        case COLLECT_ERROR_WAIT:

            if ((uint32_t)(current_tick - state_tick) >= 100U)
            {
                state = COLLECT_START;
            }

            break;

        default:

            state = COLLECT_START;
            break;
    }
}
