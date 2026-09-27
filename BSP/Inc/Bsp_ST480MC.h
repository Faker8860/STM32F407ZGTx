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

/* =========================================================
 * ST480MC I2C Configuration
 * ========================================================= */

/*
 * ST480MC A0 = GND
 * 7-bit address = 0x0C
 *
 * HAL uses 8-bit address:
 * 0x0C << 1 = 0x18
 */
#define ST480MC_I2C_ADDR             (0x0CU << 1)

#define ST480MC_I2C_TIMEOUT          100U


/* =========================================================
 * ST480MC Command
 * ========================================================= */

/* Start Burst Mode */
#define ST480MC_CMD_SB_BASE          0x10U

/* Start Wake-up Change */
#define ST480MC_CMD_SW_BASE          0x20U

/* Start Single Measure */
#define ST480MC_CMD_SM_BASE          0x30U

/* Read Measurement */
#define ST480MC_CMD_RM_BASE          0x40U

/* Read Register */
#define ST480MC_CMD_RR               0x50U

/* Write Register */
#define ST480MC_CMD_WR               0x60U

/* Exit Mode */
#define ST480MC_CMD_EM               0x80U

/* Memory Recall */
#define ST480MC_CMD_HR               0xD0U

/* Memory Store */
#define ST480MC_CMD_HS               0xE0U

/* Reset */
#define ST480MC_CMD_RT               0xF0U


/* =========================================================
 * Measurement Command
 * ========================================================= */

/*
 * ZYXT = 1110
 *
 * Z = 1
 * Y = 1
 * X = 1
 * Temperature = 0
 */
#define ST480MC_CMD_SM_XYZ           0x3EU
#define ST480MC_CMD_RM_XYZ           0x4EU


/* =========================================================
 * Status Register
 * ========================================================= */

#define ST480MC_STATUS_D_MASK        0x03U
#define ST480MC_STATUS_RS            0x04U
#define ST480MC_STATUS_SED           0x08U
#define ST480MC_STATUS_ERROR         0x10U
#define ST480MC_STATUS_SM            0x20U
#define ST480MC_STATUS_WOC           0x40U
#define ST480MC_STATUS_BURST         0x80U


/* =========================================================
 * ST480MC Registers
 * ========================================================= */

#define ST480MC_REG_00               0x00U
#define ST480MC_REG_01               0x01U
#define ST480MC_REG_02               0x02U
#define ST480MC_REG_03               0x03U
#define ST480MC_REG_OFFSET_X         0x04U
#define ST480MC_REG_OFFSET_Y         0x05U
#define ST480MC_REG_OFFSET_Z         0x06U
#define ST480MC_REG_WOXY_THRESHOLD   0x07U
#define ST480MC_REG_WOZ_THRESHOLD    0x08U
#define ST480MC_REG_WOT_THRESHOLD    0x09U


/* =========================================================
 * Return Status
 * ========================================================= */

typedef enum
{
    ST480MC_OK = 0,
    ST480MC_ERROR,
    ST480MC_TIMEOUT,
    ST480MC_BUSY
} ST480MC_Status_t;


/* =========================================================
 * Raw Measurement Data
 * ========================================================= */

typedef struct
{
    int16_t X;
    int16_t Y;
    int16_t Z;
} ST480MC_RawData_t;


/* =========================================================
 * Driver State
 * ========================================================= */

typedef enum
{
    ST480MC_STATE_IDLE = 0,
    ST480MC_STATE_TX,
    ST480MC_STATE_RX,
    ST480MC_STATE_DONE,
    ST480MC_STATE_ERROR
} ST480MC_State_t;


/* =========================================================
 * Basic Command
 * ========================================================= */

/**
 * @brief Send a single command to ST480MC
 *
 * @param cmd Command byte
 *
 * @return ST480MC_Status_t
 */
ST480MC_Status_t Bsp_ST480MC_SendCommand(uint8_t cmd);


/**
 * @brief Command write transaction
 *
 * @param pData Data to transmit
 * @param size  Data size
 *
 * @return ST480MC_Status_t
 */
ST480MC_Status_t Bsp_ST480MC_CommandWrite(
    uint8_t *pData,
    uint16_t size);


/**
 * @brief Command read transaction
 *
 * @param pData Transmit command/data
 * @param txSize Transmit size
 * @param pRxData Receive buffer
 * @param rxSize Receive size
 *
 * @return ST480MC_Status_t
 */
ST480MC_Status_t Bsp_ST480MC_CommandRead(
    uint8_t *pData,
    uint16_t txSize,
    uint8_t *pRxData,
    uint16_t rxSize);


/* =========================================================
 * Generic I2C Transaction
 * ========================================================= */

/**
 * @brief
 * TX -> Repeated START -> RX
 *
 * This is the basic communication primitive of ST480MC.
 */
ST480MC_Status_t Bsp_ST480MC_TransmitReceive(
    uint8_t *pTxData,
    uint16_t txSize,
    uint8_t *pRxData,
    uint16_t rxSize);


/* =========================================================
 * Register Access
 * ========================================================= */

/**
 * @brief Read ST480MC register
 *
 * Protocol:
 *
 * START
 * Slave + W
 * 0x50
 * Register Address
 * REPEATED START
 * Slave + R
 * Status
 * Register High
 * Register Low
 * STOP
 */
ST480MC_Status_t Bsp_ST480MC_ReadReg(
    uint8_t reg,
    uint16_t *data);


/**
 * @brief Write ST480MC register
 *
 * Protocol:
 *
 * START
 * Slave + W
 * 0x60
 * High
 * Low
 * Register Address
 * STOP
 */
ST480MC_Status_t Bsp_ST480MC_WriteReg(
    uint8_t reg,
    uint16_t data);


/* =========================================================
 * Measurement
 * ========================================================= */

/**
 * @brief Start single XYZ measurement
 */
ST480MC_Status_t Bsp_ST480MC_StartMeasurement(void);


/**
 * @brief Read XYZ measurement
 */
ST480MC_Status_t Bsp_ST480MC_ReadMeasurement(
    ST480MC_RawData_t *data);


/**
 * @brief Start measurement and read XYZ
 */
ST480MC_Status_t Bsp_ST480MC_MeasureXYZ(
    ST480MC_RawData_t *data);


/* =========================================================
 * Status
 * ========================================================= */

/**
 * @brief Get last ST480MC status byte
 */
uint8_t Bsp_ST480MC_GetLastStatus(void);


/**
 * @brief Check whether ST480MC is busy
 */
uint8_t Bsp_ST480MC_IsBusy(void);


/**
 * @brief Initialize ST480MC driver
 *
 * Note:
 * I2C1 itself is initialized by CubeMX/HAL.
 */
ST480MC_Status_t Bsp_ST480MC_Init(void);

#ifdef __cplusplus
}
#endif


#endif /* INC_BSP_ST480MC_H_ */
