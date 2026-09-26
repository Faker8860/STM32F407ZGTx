/*
 * Bsp_AT24C02.h
 *
 *  Created on: 2026年9月25日
 *      Author: andyn
 */

#ifndef INC_BSP_AT24C02_H_
#define INC_BSP_AT24C02_H_

#include "gpio.h"
#include "main.h"

#include "main.h"
#include <stdint.h>

/* AT24C02 参数 */
#define EEPROM_SIZE             256U
#define EEPROM_PAGE_SIZE        8U

/* A0、A1、A2 全部接 GND */
#define EEPROM_I2C_ADDR         (0x50 << 1)

/* 超时时间 */
#define EEPROM_TIMEOUT          100U

typedef enum
{
    EEPROM_OK = 0,
    EEPROM_ERROR,
    EEPROM_TIMEOUT_ERROR,
    EEPROM_BUSY
} EEPROM_Status;

/* 初始化 / 检测 EEPROM */
EEPROM_Status Bsp_Eeprom_Init(void);

/* 检测 EEPROM 是否在线 */
EEPROM_Status Bsp_Eeprom_IsReady(void);

/* 写 1 个字节 */
EEPROM_Status Bsp_Eeprom_WriteByte(uint8_t mem_addr, uint8_t data);

/* 读 1 个字节 */
EEPROM_Status Bsp_Eeprom_ReadByte(uint8_t mem_addr, uint8_t *data);

/* 写多个字节 */
EEPROM_Status Bsp_Eeprom_Write(uint8_t mem_addr, uint8_t *pData, uint16_t size);

/* 读多个字节 */
EEPROM_Status Bsp_Eeprom_Read(uint8_t mem_addr, uint8_t *pData, uint16_t size);


#endif /* INC_BSP_AT24C02_H_ */
