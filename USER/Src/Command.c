/*
 * Command.c
 *
 *  Created on: 2026年9月20日
 *      Author: yufan
 */


#include <Inc/Command.h>
#include "mb.h"

static void InputREG_Init(void);
static void HoldingREG_Init(void);
static void System_PrintStartupInfo(void);

void User_Init(void)
{
	Bsp_ServoInit();
	Bsp_BeepInit();
	Bsp_LedInit();

	InputREG_Init();
	HoldingREG_Init();

	System_PrintStartupInfo();
}

void Command_KeyProcess(void)
{
    uint8_t key;
    static uint16_t Angle = 0;

    key = Bsp_KEY_Scan();

    switch (key)
    {
        case KEY_0:
            Bsp_Led0_Toggle();
            printf("KEY0 Press\r\n");
            break;

        case KEY_1:
            Bsp_Led1_Toggle();
            printf("KEY1 Press\r\n");
            break;

        case KEY_2:
            Bsp_BeepToggle();
            printf("KEY2 Press\r\n");
            break;

        case KEY_WK_UP:
            Angle += 15;

            if (Angle >= 180)
            {
                Angle = 0;
            }

            Bsp_ServoSetAngle(Angle);

            printf("Angle: %u\r\n", Angle);
            break;

        default:

            break;
    }

    HAL_Delay(10);
}

static void InputREG_Init(void)
{
	INPUTREG  -> Version_0 = FIRMWARE_VERSION0;
	INPUTREG  -> Version_1 = FIRMWARE_VERSION1;
	INPUTREG  -> Version_2 = FIRMWARE_VERSION2;
}

static void HoldingREG_Init(void)
{
	HOLDINGREG  -> SystemMode = RUN_MODE_APP;
	HOLDINGREG  -> BaudRate = g_AppConfig.baudRate;
}

static void System_PrintStartupInfo(void)
{
    printf("========================================\r\n");
    printf("        STM32 DEVICE STARTUP\r\n");
    printf("========================================\r\n");
    printf("Firmware : V%0x.%0x.%0x\r\n", FIRMWARE_VERSION0, FIRMWARE_VERSION1, FIRMWARE_VERSION2);
    printf("Hardware : V1.0\r\n");
    printf("MCU      : STM32F407ZGT6\r\n");
    printf("BaudRate : %lu (code 0x%02X)\r\n", (unsigned long)APP_Config_GetBaudRate(), g_AppConfig.baudRate);
    printf("[INFO] System initialization complete.\r\n");
    printf("========================================\r\n");
}

void Modbus_BaudApply(void)
{
	uint16_t code;

	if (g_bBaudChangePending == 0U)
	{
		return;
	}

	g_bBaudChangePending = 0U;

	code = HOLDINGREG->BaudRate;

	/* 非法编码：回退到当前配置，不切换 */
	if (code > BAUD_CODE_230400)
	{
		HOLDINGREG->BaudRate = g_AppConfig.baudRate;
		return;
	}

	/* 没变化 */
	if ((uint8_t)code == g_AppConfig.baudRate)
	{
		return;
	}

	/* 更新配置并保存到 EEPROM */
	g_AppConfig.baudRate = (uint8_t)code;
	APP_Config_Save();

	/* 重新配置 UART 和 FreeModbus（T3.5 帧间隔会按新波特率重算） */
	eMBDisable();
	User_UART2_Init(APP_Config_GetBaudRate());
	eMBInit(MB_RTU, 0x01, 0x01, APP_Config_GetBaudRate(), MB_PAR_EVEN, 1);
	eMBEnable();
}
