/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#pragma pack(push, 1)
typedef struct
{
    uint16_t SystemMode;       	//[0] 0xAA = APP,	0xFF = BOOT

    /* 波特率选择 (初始化值: 0x05 = 115200) */
    uint16_t BaudRate;			//[1]

} HoldingREG;
_Static_assert(sizeof(HoldingREG) == 2 * sizeof(uint16_t), "HoldingREG Size ERROR");
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
	uint16_t Version_0;		// [0] 固件版本寄存器0，年

	uint16_t Version_1;		// [1] 固件版本寄存器1，月日

	uint16_t Version_2;		// [2] 固件版本寄存器2，时分
} InputREG;
_Static_assert(sizeof(InputREG) == 3 * sizeof(uint16_t),"InputREG Size ERROR");
#pragma pack(pop)



/* =========================================================
 * Global Register Pointer
 * ========================================================= */
extern HoldingREG *const HOLDINGREG;
extern InputREG   *const INPUTREG;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY2_Pin GPIO_PIN_2
#define KEY2_GPIO_Port GPIOE
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE
#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE
#define Light_Sensor_Pin GPIO_PIN_7
#define Light_Sensor_GPIO_Port GPIOF
#define Beep_Pin GPIO_PIN_8
#define Beep_GPIO_Port GPIOF
#define LED0_Pin GPIO_PIN_9
#define LED0_GPIO_Port GPIOF
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOF
#define WK_UP_Pin GPIO_PIN_0
#define WK_UP_GPIO_Port GPIOA
#define Servo_Pin GPIO_PIN_5
#define Servo_GPIO_Port GPIOA
#define RS485_DE_Pin GPIO_PIN_8
#define RS485_DE_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
