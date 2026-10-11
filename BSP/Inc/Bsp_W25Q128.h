/*
 * Bsp_W25Q128.h
 *
 *  Created on: 2026年10月11日
 *      Author: yufan
 */

#ifndef INC_BSP_W25Q128_H_
#define INC_BSP_W25Q128_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "main.h"
/* ======================== 基本参数 ======================== */

#define W25Q128_FLASH_SIZE          0x01000000UL  /* 16 MB */
#define W25Q128_PAGE_SIZE           256U
#define W25Q128_SECTOR_SIZE         4096U
#define W25Q128_BLOCK_SIZE           65536U

#define W25Q128_MANUFACTURER_ID     0xEFU
#define W25Q128_MEMORY_TYPE         0x40U
#define W25Q128_CAPACITY_ID         0x18U

#define W25Q128_TIMEOUT             5000U

/* ======================== SPI 指令 ======================== */

#define W25Q128_CMD_WRITE_ENABLE    0x06U
#define W25Q128_CMD_WRITE_DISABLE   0x04U
#define W25Q128_CMD_READ_STATUS1    0x05U
#define W25Q128_CMD_READ_STATUS2    0x35U
#define W25Q128_CMD_READ_STATUS3    0x15U

#define W25Q128_CMD_READ_DATA       0x03U
#define W25Q128_CMD_FAST_READ       0x0BU
#define W25Q128_CMD_PAGE_PROGRAM    0x02U

#define W25Q128_CMD_SECTOR_ERASE    0x20U
#define W25Q128_CMD_BLOCK_ERASE     0xD8U
#define W25Q128_CMD_CHIP_ERASE      0xC7U

#define W25Q128_CMD_JEDEC_ID        0x9FU
#define W25Q128_CMD_DEVICE_ID       0x90U
#define W25Q128_CMD_POWER_DOWN      0xB9U
#define W25Q128_CMD_RELEASE_POWER   0xABU

/* ======================== 返回状态 ======================== */

typedef enum
{
    W25Q128_OK = 0,
    W25Q128_ERROR,
    W25Q128_BUSY,
    W25Q128_TIMEOUT_ERROR,
    W25Q128_PARAM_ERROR,
    W25Q128_ID_ERROR
} W25Q128_StatusTypeDef;

/* ======================== 数据结构 ======================== */

typedef struct
{
    uint8_t manufacturer_id;
    uint8_t memory_type;
    uint8_t capacity_id;
} W25Q128_IDTypeDef;

/* ======================== 对外接口 ======================== */

W25Q128_StatusTypeDef Bsp_W25Q128_Init(void);

W25Q128_StatusTypeDef
Bsp_W25Q128_ReadID(W25Q128_IDTypeDef *id);

W25Q128_StatusTypeDef
Bsp_W25Q128_ReadStatus(uint8_t *status);

W25Q128_StatusTypeDef
Bsp_W25Q128_WaitBusy(uint32_t timeout);

W25Q128_StatusTypeDef
Bsp_W25Q128_Read(uint32_t address,
                 uint8_t *buffer,
                 uint32_t length);

W25Q128_StatusTypeDef
Bsp_W25Q128_Write(uint32_t address,
                  const uint8_t *buffer,
                  uint32_t length);

W25Q128_StatusTypeDef
Bsp_W25Q128_EraseSector(uint32_t address);

W25Q128_StatusTypeDef
Bsp_W25Q128_EraseBlock(uint32_t address);

W25Q128_StatusTypeDef
Bsp_W25Q128_EraseChip(void);

W25Q128_StatusTypeDef
Bsp_W25Q128_PowerDown(void);

W25Q128_StatusTypeDef
Bsp_W25Q128_WakeUp(void);

void W25Q128_PowerWriteTest(void);
void W25Q128_PowerReadTest(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_BSP_W25Q128_H_ */
