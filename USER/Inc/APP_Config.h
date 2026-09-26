/*
 * APP_Config.h
 *
 * Created on: 2026年9月25日
 * Author: yufan
 */

#ifndef INC_APP_CONFIG_H_
#define INC_APP_CONFIG_H_

#include "main.h"
#include <stdint.h>
#include <Inc/Bsp_AT24C02.h>
#include <string.h>
#include "stdio.h"
/* ================= 系统配置 ================= */

#define RUN_MODE_APP        0xAA

#define FIRMWARE_VERSION0   0x2026
#define FIRMWARE_VERSION1   0x0925
#define FIRMWARE_VERSION2   0x1505


/* ================= 波特率配置 ================= */

typedef enum
{
    BAUD_CODE_9600   = 0x00,
    BAUD_CODE_14400  = 0x01,
    BAUD_CODE_19200  = 0x02,
    BAUD_CODE_38400  = 0x03,
    BAUD_CODE_57600  = 0x04,
    BAUD_CODE_115200 = 0x05,
    BAUD_CODE_230400 = 0x06,

} BaudRateCode;


/* ================= EEPROM配置 ================= */

#define EEPROM_CONFIG_MAGIC       0xA5U

#define DEFAULT_BAUD_CODE         BAUD_CODE_115200
#define DEFAULT_SYSTEM_MODE       RUN_MODE_APP


/* ================= EEPROM配置结构体 ================= */

typedef struct
{
    uint8_t MAGIC;
    uint8_t baudRate;
    uint8_t systemMode;
    uint8_t reserved[5];

} APP_Config_t;


/* ================= 全局配置 ================= */

extern APP_Config_t g_AppConfig;


/* ================= 配置接口 ================= */

void APP_Config_SetDefault(void);
HAL_StatusTypeDef APP_Config_Load(void);
HAL_StatusTypeDef APP_Config_Save(void);
uint32_t APP_Config_GetBaudRate(void);
void APP_Config_Init(void);

#endif /* INC_APP_CONFIG_H_ */
