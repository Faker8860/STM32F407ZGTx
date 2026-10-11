/*
 * Bsp_W25Q128.c
 *
 *  Created on: 2026年10月11日
 *      Author: yufan
 */

#include "Inc/Bsp_W25Q128.h"

/* ======================== 硬件配置 ======================== */

/*
 * SPI1 由 CubeMX 初始化。
 * PB14：W25Q128_CS，低电平选中芯片。
 */
extern SPI_HandleTypeDef hspi1;

#define W25Q128_CS_LOW() 		 	HAL_GPIO_WritePin(W25Q128_CS_GPIO_Port, W25Q128_CS_Pin, GPIO_PIN_RESET)

#define W25Q128_CS_HIGH() 			HAL_GPIO_WritePin(W25Q128_CS_GPIO_Port, W25Q128_CS_Pin, GPIO_PIN_SET)

/* ======================== 内部函数 ======================== */

/**
 * @brief  检查地址和长度是否在 Flash 范围内
 */
static uint8_t W25Q128_IsRangeValid(uint32_t address,
                                    uint32_t length)
{
    if (address >= W25Q128_FLASH_SIZE)
    {
        return 0U;
    }

    if (length == 0U)
    {
        return 1U;
    }

    if (length > (W25Q128_FLASH_SIZE - address))
    {
        return 0U;
    }

    return 1U;
}


/**
 * @brief  发送单字节指令
 */
static W25Q128_StatusTypeDef
W25Q128_SendCommand(uint8_t command)
{
    HAL_StatusTypeDef ret;

    W25Q128_CS_LOW();

    ret = HAL_SPI_Transmit(&hspi1,
                           &command,
                           1U,
                           W25Q128_TIMEOUT);

    W25Q128_CS_HIGH();

    return (ret == HAL_OK) ? W25Q128_OK : W25Q128_ERROR;
}


/**
 * @brief  写使能
 */
static W25Q128_StatusTypeDef W25Q128_WriteEnable(void)
{
    uint8_t status;

    if (W25Q128_SendCommand(W25Q128_CMD_WRITE_ENABLE) != W25Q128_OK)
    {
        return W25Q128_ERROR;
    }

    if (Bsp_W25Q128_ReadStatus(&status) != W25Q128_OK)
    {
        return W25Q128_ERROR;
    }

    /* Status Register 1 的 WEL 位为 bit1 */
    if ((status & 0x02U) == 0U)
    {
        return W25Q128_ERROR;
    }

    return W25Q128_OK;
}


/**
 * @brief  读取 JEDEC ID，不执行外部参数检查
 */
static W25Q128_StatusTypeDef
W25Q128_ReadJedecID(uint8_t id[3])
{
    uint8_t command = W25Q128_CMD_JEDEC_ID;
    HAL_StatusTypeDef ret;

    W25Q128_CS_LOW();

    ret = HAL_SPI_Transmit(&hspi1,
                           &command,
                           1U,
                           W25Q128_TIMEOUT);

    if (ret == HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi1,
                              id,
                              3U,
                              W25Q128_TIMEOUT);
    }

    W25Q128_CS_HIGH();

    return (ret == HAL_OK) ? W25Q128_OK : W25Q128_ERROR;
}


/* ======================== 对外接口 ======================== */

/**
 * @brief  初始化 W25Q128
 *
 * 注意：SPI1 和 PB14 GPIO 必须已经由 CubeMX 初始化。
 */
W25Q128_StatusTypeDef Bsp_W25Q128_Init(void)
{
    W25Q128_IDTypeDef id;

    W25Q128_CS_HIGH();

    if (Bsp_W25Q128_ReadID(&id) != W25Q128_OK)
    {
        return W25Q128_ERROR;
    }

    if ((id.manufacturer_id != W25Q128_MANUFACTURER_ID) ||
        (id.memory_type != W25Q128_MEMORY_TYPE) ||
        (id.capacity_id != W25Q128_CAPACITY_ID))
    {
        return W25Q128_ID_ERROR;
    }

    return W25Q128_OK;
}


/**
 * @brief  读取 JEDEC ID
 *
 * W25Q128JV 常见返回值：
 * EF 40 18
 */
W25Q128_StatusTypeDef
Bsp_W25Q128_ReadID(W25Q128_IDTypeDef *id)
{
    uint8_t id_buffer[3];

    if (id == NULL)
    {
        return W25Q128_PARAM_ERROR;
    }

    if (W25Q128_ReadJedecID(id_buffer) != W25Q128_OK)
    {
        return W25Q128_ERROR;
    }

    id->manufacturer_id = id_buffer[0];
    id->memory_type     = id_buffer[1];
    id->capacity_id     = id_buffer[2];

    return W25Q128_OK;
}


/**
 * @brief  读取状态寄存器 1
 */
W25Q128_StatusTypeDef
Bsp_W25Q128_ReadStatus(uint8_t *status)
{
    uint8_t command = W25Q128_CMD_READ_STATUS1;
    HAL_StatusTypeDef ret;

    if (status == NULL)
    {
        return W25Q128_PARAM_ERROR;
    }

    W25Q128_CS_LOW();

    ret = HAL_SPI_Transmit(&hspi1,
                           &command,
                           1U,
                           W25Q128_TIMEOUT);

    if (ret == HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi1,
                              status,
                              1U,
                              W25Q128_TIMEOUT);
    }

    W25Q128_CS_HIGH();

    return (ret == HAL_OK) ? W25Q128_OK : W25Q128_ERROR;
}


/**
 * @brief  等待 Flash 内部操作结束
 *
 * SR1 bit0：BUSY
 */
W25Q128_StatusTypeDef
Bsp_W25Q128_WaitBusy(uint32_t timeout)
{
    uint32_t start_tick = HAL_GetTick();
    uint8_t status;

    do
    {
        if (Bsp_W25Q128_ReadStatus(&status) != W25Q128_OK)
        {
            return W25Q128_ERROR;
        }

        if ((status & 0x01U) == 0U)
        {
            return W25Q128_OK;
        }

    } while ((uint32_t)(HAL_GetTick() - start_tick) < timeout);

    return W25Q128_TIMEOUT_ERROR;
}


/**
 * @brief  连续读取 Flash
 */
W25Q128_StatusTypeDef
Bsp_W25Q128_Read(uint32_t address,
                 uint8_t *buffer,
                 uint32_t length)
{
    uint8_t command[4];
    HAL_StatusTypeDef ret;

    if ((buffer == NULL) && (length != 0U))
    {
        return W25Q128_PARAM_ERROR;
    }

    if (length == 0U)
    {
        return W25Q128_OK;
    }

    if (W25Q128_IsRangeValid(address, length) == 0U)
    {
        return W25Q128_PARAM_ERROR;
    }

    command[0] = W25Q128_CMD_READ_DATA;
    command[1] = (uint8_t)(address >> 16);
    command[2] = (uint8_t)(address >> 8);
    command[3] = (uint8_t)address;

    W25Q128_CS_LOW();

    ret = HAL_SPI_Transmit(&hspi1,
                           command,
                           sizeof(command),
                           W25Q128_TIMEOUT);

    if (ret == HAL_OK)
    {
        ret = HAL_SPI_Receive(&hspi1,
                              buffer,
                              length,
                              W25Q128_TIMEOUT);
    }

    W25Q128_CS_HIGH();

    return (ret == HAL_OK) ? W25Q128_OK : W25Q128_ERROR;
}


/**
 * @brief  连续写入 Flash
 *
 * 自动按 256 字节 Page 分段。
 *
 * 注意：
 * 1. 不自动擦除扇区。
 * 2. 写入前应确保目标区域已经擦除。
 * 3. 普通 NOR Flash 只能将 bit 从 1 写成 0；
 *    需要从 0 恢复到 1 时必须先擦除。
 */
W25Q128_StatusTypeDef
Bsp_W25Q128_Write(uint32_t address,
                  const uint8_t *buffer,
                  uint32_t length)
{
    uint8_t command[4];
    uint32_t page_remain;
    uint32_t write_length;
    HAL_StatusTypeDef ret;

    if ((buffer == NULL) && (length != 0U))
    {
        return W25Q128_PARAM_ERROR;
    }

    if (length == 0U)
    {
        return W25Q128_OK;
    }

    if (W25Q128_IsRangeValid(address, length) == 0U)
    {
        return W25Q128_PARAM_ERROR;
    }

    while (length > 0U)
    {
        /* 当前页剩余空间 */
        page_remain = W25Q128_PAGE_SIZE -
                      (address % W25Q128_PAGE_SIZE);

        write_length = (length < page_remain) ?
                       length : page_remain;

        if (Bsp_W25Q128_WaitBusy(W25Q128_TIMEOUT) != W25Q128_OK)
        {
            return W25Q128_TIMEOUT_ERROR;
        }

        if (W25Q128_WriteEnable() != W25Q128_OK)
        {
            return W25Q128_ERROR;
        }

        command[0] = W25Q128_CMD_PAGE_PROGRAM;
        command[1] = (uint8_t)(address >> 16);
        command[2] = (uint8_t)(address >> 8);
        command[3] = (uint8_t)address;

        W25Q128_CS_LOW();

        ret = HAL_SPI_Transmit(&hspi1,
                               command,
                               sizeof(command),
                               W25Q128_TIMEOUT);

        if (ret == HAL_OK)
        {
            ret = HAL_SPI_Transmit(&hspi1,
                                   (uint8_t *)buffer,
                                   write_length,
                                   W25Q128_TIMEOUT);
        }

        W25Q128_CS_HIGH();

        if (ret != HAL_OK)
        {
            return W25Q128_ERROR;
        }

        if (Bsp_W25Q128_WaitBusy(W25Q128_TIMEOUT) != W25Q128_OK)
        {
            return W25Q128_TIMEOUT_ERROR;
        }

        address += write_length;
        buffer  += write_length;
        length  -= write_length;
    }

    return W25Q128_OK;
}


/**
 * @brief  擦除包含 address 的 4 KB 扇区
 *
 * address 不要求 4 KB 对齐。
 */
W25Q128_StatusTypeDef
Bsp_W25Q128_EraseSector(uint32_t address)
{
    uint8_t command[4];
    HAL_StatusTypeDef ret;

    if (address >= W25Q128_FLASH_SIZE)
    {
        return W25Q128_PARAM_ERROR;
    }

    address &= ~(W25Q128_SECTOR_SIZE - 1U);

    if (Bsp_W25Q128_WaitBusy(W25Q128_TIMEOUT) != W25Q128_OK)
    {
        return W25Q128_TIMEOUT_ERROR;
    }

    if (W25Q128_WriteEnable() != W25Q128_OK)
    {
        return W25Q128_ERROR;
    }

    command[0] = W25Q128_CMD_SECTOR_ERASE;
    command[1] = (uint8_t)(address >> 16);
    command[2] = (uint8_t)(address >> 8);
    command[3] = (uint8_t)address;

    W25Q128_CS_LOW();

    ret = HAL_SPI_Transmit(&hspi1,
                           command,
                           sizeof(command),
                           W25Q128_TIMEOUT);

    W25Q128_CS_HIGH();

    if (ret != HAL_OK)
    {
        return W25Q128_ERROR;
    }

    return Bsp_W25Q128_WaitBusy(W25Q128_TIMEOUT);
}


/**
 * @brief  擦除包含 address 的 64 KB 块
 *
 * address 不要求 64 KB 对齐。
 */
W25Q128_StatusTypeDef
Bsp_W25Q128_EraseBlock(uint32_t address)
{
    uint8_t command[4];
    HAL_StatusTypeDef ret;

    if (address >= W25Q128_FLASH_SIZE)
    {
        return W25Q128_PARAM_ERROR;
    }

    address &= ~(W25Q128_BLOCK_SIZE - 1U);

    if (Bsp_W25Q128_WaitBusy(W25Q128_TIMEOUT) != W25Q128_OK)
    {
        return W25Q128_TIMEOUT_ERROR;
    }

    if (W25Q128_WriteEnable() != W25Q128_OK)
    {
        return W25Q128_ERROR;
    }

    command[0] = W25Q128_CMD_BLOCK_ERASE;
    command[1] = (uint8_t)(address >> 16);
    command[2] = (uint8_t)(address >> 8);
    command[3] = (uint8_t)address;

    W25Q128_CS_LOW();

    ret = HAL_SPI_Transmit(&hspi1,
                           command,
                           sizeof(command),
                           W25Q128_TIMEOUT);

    W25Q128_CS_HIGH();

    if (ret != HAL_OK)
    {
        return W25Q128_ERROR;
    }

    return Bsp_W25Q128_WaitBusy(W25Q128_TIMEOUT);
}


/**
 * @brief  整片擦除
 *
 * 警告：此操作会删除整片 Flash 的所有数据。
 */
W25Q128_StatusTypeDef Bsp_W25Q128_EraseChip(void)
{
    if (Bsp_W25Q128_WaitBusy(W25Q128_TIMEOUT) != W25Q128_OK)
    {
        return W25Q128_TIMEOUT_ERROR;
    }

    if (W25Q128_WriteEnable() != W25Q128_OK)
    {
        return W25Q128_ERROR;
    }

    if (W25Q128_SendCommand(W25Q128_CMD_CHIP_ERASE) != W25Q128_OK)
    {
        return W25Q128_ERROR;
    }

    /*
     * 整片擦除时间较长，不能使用普通短超时。
     * 这里采用 180 秒作为软件等待上限。
     */
    return Bsp_W25Q128_WaitBusy(180000U);
}


/**
 * @brief  进入掉电模式
 */
W25Q128_StatusTypeDef Bsp_W25Q128_PowerDown(void)
{
    return W25Q128_SendCommand(W25Q128_CMD_POWER_DOWN);
}


/**
 * @brief  退出掉电模式
 */
W25Q128_StatusTypeDef Bsp_W25Q128_WakeUp(void)
{
    uint8_t command = W25Q128_CMD_RELEASE_POWER;
    HAL_StatusTypeDef ret;

    W25Q128_CS_LOW();

    ret = HAL_SPI_Transmit(&hspi1,
                           &command,
                           1U,
                           W25Q128_TIMEOUT);

    W25Q128_CS_HIGH();

    if (ret != HAL_OK)
    {
        return W25Q128_ERROR;
    }

    HAL_Delay(1U);

    return W25Q128_OK;
}

#define W25Q128_POWER_TEST_ADDR    0x00030000UL

void W25Q128_PowerWriteTest(void)
{
    W25Q128_StatusTypeDef ret;
    uint8_t status = 0xFF;

    static const uint8_t test_data[] =
    {
        0x12, 0x34, 0x56, 0x78,
        0xAA, 0x55, 0xA5, 0x5A
    };

    uint8_t read_data[sizeof(test_data)] = {0};

    const uint32_t addr = 0x00030000UL;

    printf("\r\n===== W25Q128 Power Write Test =====\r\n");

    /* 1. 擦除扇区 */
    ret = Bsp_W25Q128_EraseSector(addr);
    printf("Erase ret: %d\r\n", (int)ret);

    if (ret != W25Q128_OK)
    {
        return;
    }

    /* 2. 读取擦除后的数据 */
    ret = Bsp_W25Q128_Read(addr, read_data, sizeof(read_data));

    printf("Read after erase ret: %d\r\n", (int)ret);
    printf("After erase: ");

    for (uint32_t i = 0; i < sizeof(read_data); i++)
    {
        printf("%02X ", read_data[i]);
    }
    printf("\r\n");

    /* 3. 写入数据 */
    ret = Bsp_W25Q128_Write(addr, test_data, sizeof(test_data));
    printf("Write ret: %d\r\n", (int)ret);

    if (ret != W25Q128_OK)
    {
        return;
    }

    /* 4. 读取状态寄存器 */
    ret = Bsp_W25Q128_ReadStatus(&status);

    printf("Read Status ret: %d\r\n", (int)ret);
    printf("Status Register: 0x%02X\r\n", status);

    /* 5. 立即读回 */
    ret = Bsp_W25Q128_Read(addr, read_data, sizeof(read_data));

    printf("Read back ret: %d\r\n", (int)ret);
    printf("Read back: ");

    for (uint32_t i = 0; i < sizeof(read_data); i++)
    {
        printf("%02X ", read_data[i]);
    }
    printf("\r\n");

    if (ret == W25Q128_OK &&
        memcmp(test_data, read_data, sizeof(test_data)) == 0)
    {
        printf("Immediate Verify: PASS\r\n");
        printf("Now power off and restart MCU.\r\n");
    }
    else
    {
        printf("Immediate Verify: FAIL\r\n");
    }
}

void W25Q128_PowerReadTest(void)
{
    W25Q128_StatusTypeDef ret;

    static const uint8_t expected_data[] =
    {
        0x12, 0x34, 0x56, 0x78,
        0xAA, 0x55, 0xA5, 0x5A
    };

    uint8_t read_data[sizeof(expected_data)] = {0};

    printf("\r\n===== W25Q128 Power Test: READ =====\r\n");

    ret = Bsp_W25Q128_Read(
        W25Q128_POWER_TEST_ADDR,
        read_data,
        sizeof(read_data)
    );

    if (ret != W25Q128_OK)
    {
        printf("Read failed: %d\r\n", (int)ret);
        return;
    }

    printf("Read: ");
    for (uint32_t i = 0; i < sizeof(read_data); i++)
    {
        printf("%02X ", read_data[i]);
    }
    printf("\r\n");

    if (memcmp(expected_data, read_data, sizeof(read_data)) == 0)
    {
        printf("Power Retention Test: PASS\r\n");
    }
    else
    {
        printf("Power Retention Test: FAIL\r\n");
    }
}

