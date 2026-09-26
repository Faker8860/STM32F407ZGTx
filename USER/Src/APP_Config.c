/*
 * APP_Config.c
 *
 * Created on: 2026年9月25日
 * Author: yufan
 */

#include "Inc/APP_Config.h"


/* =========================================================
 * EEPROM 配置起始地址
 * ========================================================= */

#define EEPROM_CONFIG_ADDR    0x00U


/* =========================================================
 * 全局配置
 * ========================================================= */

APP_Config_t g_AppConfig;


/* =========================================================
 * 私有函数
 * ========================================================= */

/**
 * @brief 检查波特率编码是否合法
 */
static uint8_t APP_Config_BaudRateIsValid(uint8_t baudRate)
{
    if (baudRate <= BAUD_CODE_230400)
    {
        return 1U;
    }

    return 0U;
}


/**
 * @brief 检查当前配置是否合法
 */
static uint8_t APP_Config_Check(void)
{
    /*
     * 检查 MAGIC
     */
    if (g_AppConfig.MAGIC != EEPROM_CONFIG_MAGIC)
    {
        return 0U;
    }

    /*
     * 检查波特率
     */
    if (!APP_Config_BaudRateIsValid(g_AppConfig.baudRate))
    {
        return 0U;
    }

    return 1U;
}


/* =========================================================
 * 默认配置
 * ========================================================= */

/**
 * @brief 设置默认配置
 */
void APP_Config_SetDefault(void)
{
    memset(&g_AppConfig, 0, sizeof(APP_Config_t));

    g_AppConfig.MAGIC      = EEPROM_CONFIG_MAGIC;
    g_AppConfig.baudRate   = DEFAULT_BAUD_CODE;
    g_AppConfig.systemMode = DEFAULT_SYSTEM_MODE;
}


/* =========================================================
 * 保存配置
 * ========================================================= */

/**
 * @brief 将当前配置保存到 EEPROM
 */
HAL_StatusTypeDef APP_Config_Save(void)
{
    EEPROM_Status ret;

    /*
     * 检查配置
     */
    if (!APP_Config_Check())
    {
        return HAL_ERROR;
    }

    /*
     * 将整个配置结构体写入 EEPROM
     *
     * APP_Config_t = 8 Byte
     *
     * EEPROM：
     * 0x00 MAGIC
     * 0x01 baudRate
     * 0x02 systemMode
     * 0x03~0x07 reserved
     */
    ret = Bsp_Eeprom_Write(
            EEPROM_CONFIG_ADDR,
            (uint8_t *)&g_AppConfig,
            sizeof(APP_Config_t));

    if (ret != EEPROM_OK)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}


/* =========================================================
 * 加载配置
 * ========================================================= */

/**
 * @brief 从 EEPROM 加载配置
 */
HAL_StatusTypeDef APP_Config_Load(void)
{
    EEPROM_Status ret;

    /*
     * 从 EEPROM 读取整个配置
     */
    ret = Bsp_Eeprom_Read(
            EEPROM_CONFIG_ADDR,
            (uint8_t *)&g_AppConfig,
            sizeof(APP_Config_t));

    /*
     * EEPROM 通信失败
     */
    if (ret != EEPROM_OK)
    {
        /*
         * EEPROM 读失败时使用默认配置
         */
        APP_Config_SetDefault();

        return HAL_ERROR;
    }

    /*
     * 检查 EEPROM 中的配置是否有效
     */
    if (!APP_Config_Check())
    {
        /*
         * EEPROM 第一次使用
         * 或者配置数据异常
         */
        APP_Config_SetDefault();

        /*
         * 将默认配置保存到 EEPROM
         */
        return APP_Config_Save();
    }

    return HAL_OK;
}


/* =========================================================
 * 获取实际波特率
 * ========================================================= */

/**
 * @brief 根据波特率编码获取实际波特率
 */
uint32_t APP_Config_GetBaudRate(void)
{
    switch (g_AppConfig.baudRate)
    {
        case BAUD_CODE_9600:
            return 9600U;

        case BAUD_CODE_14400:
            return 14400U;

        case BAUD_CODE_19200:
            return 19200U;

        case BAUD_CODE_38400:
            return 38400U;

        case BAUD_CODE_57600:
            return 57600U;

        case BAUD_CODE_115200:
            return 115200U;

        case BAUD_CODE_230400:
            return 230400U;

        default:
            return 115200U;
    }
}


void APP_Config_Init(void)
{
    /* EEPROM 初始化失败 */
    if (Bsp_Eeprom_Init() != EEPROM_OK)
    {
        APP_Config_SetDefault();
        return;
    }

    /* 加载配置 */
    if (APP_Config_Load() != HAL_OK)
    {
        APP_Config_SetDefault();
    }
}

