/*
 * Bsp_ST480MC.h
 *
 *  Created on: 2026年9月27日
 *      Author: yufan
 */

#ifndef INC_BSP_ST480MC_H_
#define INC_BSP_ST480MC_H_

#include "main.h"
#include <stdint.h>
#include <stdio.h>
#include <i2c.h>

/* ----------------------- 配置 --------------------------------------------- */

#define ST480MC_I2C_ADDR             0x0CU
#define ST480MC_CMD_EXIT_MODE        0x80U
#define ST480MC_CMD_READ_REGISTER    0x50U
#define ST480MC_CMD_WRITE_REGISTER   0x60U
#define ST480MC_CMD_READ_MEASUREMENT 0x4FU
#define ST480MC_CMD_START_MEASUREMENT 0x3FU
#define ST480MC_TIMEOUT_MS           100U

extern I2C_HandleTypeDef hi2c1;

#define ST480MC_STATUS_ERROR(status) 		(((status) & 0x10U) != 0U)

/* ST480MC采集间隔 */
#define ST480MC_SAMPLE_INTERVAL_MS    300U

HAL_StatusTypeDef Bsp_ST480MC_WriteRegister(uint8_t reg_addr, uint16_t reg_value, uint8_t *status);
void ST480MC_AddressTest(void);
HAL_StatusTypeDef Bsp_ST480MC_ReadStatus(uint8_t *status);
HAL_StatusTypeDef Bsp_ST480MC_ReadRegister(uint8_t reg_addr, uint8_t *status, uint16_t *reg_value);
void TESt0(void);
HAL_StatusTypeDef Bsp_ST480MC_StartMeasurement(uint8_t *status);
HAL_StatusTypeDef Bsp_ST480MC_ReadMeasurement(uint8_t *status, uint16_t *temperature, uint16_t *mag_x, uint16_t *mag_y, uint16_t *mag_z);
void Bsp_ST480MC_CollectTask(void);


#ifdef __cplusplus
}
#endif


#endif /* INC_BSP_ST480MC_H_ */
