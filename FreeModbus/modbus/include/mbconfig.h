/* 
 * FreeModbus库：用于Modbus ASCII/RTU的可移植实现。
 * 版权所有（c）2006-2018 Christian Walter <cwalter@embedded-solutions.at>
 * 保留所有权利。
 *
 * 允许在源代码和二进制形式中重新分发和使用，无论是否进行了
 * 修改，只要满足以下条件：
 * 1. 必须保留上述版权声明、条件列表和以下免责声明。
 * 2. 在分发的文档和/或其他提供的材料中必须重现上述版权声明、条件列表和以下免责声明。
 * 3. 未经特定书面许可，不得使用作者的名称来认可或推广与本软件派生的产品。
 *
 * 本软件是按原样提供的，任何明示或暗示的保证，
 * 包括但不限于适销性和适用于特定目的的暗示保证，
 * 都是被拒绝的。
 * 在任何情况下，作者均不对任何直接、间接、
 * 偶然、特殊、典型或间接损害（包括但不限于
 * 替代品或服务的采购；使用、数据或利润的损失；
 * 或业务中断）负责，无论是基于合同、严格责任还是侵权
 * （包括疏忽或其他原因），即使事先已被告知有此种可能性。
 *
 */

#ifndef _MB_CONFIG_H
#define _MB_CONFIG_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
/* ----------------------- 定义 ------------------------------------------*/
/*! \defgroup modbus_cfg Modbus 配置
 *
 * 协议栈中的大多数模块是完全可选的，可以排除。
 * 如果目标资源非常有限且需要节省程序存储空间，
 * 则这点尤为重要。<br>
 *
 * 所有这些设置都在文件 <code>mbconfig.h</code> 中可用。
 */
/*! \addtogroup modbus_cfg
 *  @{
 */
/*! \brief 启用 Modbus ASCII 支持。 */
#define MB_ASCII_ENABLED                        (  0 )

/*! \brief 启用 Modbus RTU 支持。 */
#define MB_RTU_ENABLED                          (  1 )

/*! \brief 启用 Modbus TCP 支持。 */
#define MB_TCP_ENABLED                          (  0 )

/*! \brief Modbus ASCII 的字符超时值。
 *
 * 字符超时值对于 Modbus ASCII 不是固定的，因此是一个配置选项。
 * 它应该设置为网络的最大预期延迟时间。
 */
#define MB_ASCII_TIMEOUT_SEC                    (  1 )

/*! \brief 在启用 ASCII 之前等待启用串行发送的超时时间。
 *
 * 如果定义了该值，则函数调用 vMBPortSerialDelay，
 * 参数为 MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS，
 * 以允许在启用串行发送之前延迟一段时间。
 * 这是必需的，因为某些目标速度非常快，
 * 在接收和发送帧之间没有时间间隔。
 * 如果主站在启用其接收器时太慢，
 * 那么它将无法正确接收响应。
 */
#ifndef MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS
#define MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS    ( 0 )
#endif

/*! \brief 在启用 RTU 之前等待启用串行发送的超时时间。
 *
 * 同 ASCII 超时，某些目标在接收和发送帧之间需要延迟。
 * 0 表示不延迟。
 */
#ifndef MB_RTU_TIMEOUT_WAIT_BEFORE_SEND_MS
#define MB_RTU_TIMEOUT_WAIT_BEFORE_SEND_MS    ( 0 )
#endif

/*! \brief 协议栈应支持的最大 Modbus 功能码数量。
 *
 * 支持的最大 Modbus 功能码数量必须大于此文件中所有启用的功能的总和和自定义功能处理程序的总和。
 * 如果设置得太小，添加更多功能将失败。
 */
#define MB_FUNC_HANDLERS_MAX                    ( 16 )

/*! \brief 为 <em>报告从机 ID </em>命令分配的字节数。
 *
 * 此数字限制了报告从机 ID 函数中附加段的最大大小。
 * 有关如何设置此值的更多信息，请参见 eMBSetSlaveID(  )。
 * 仅在 MB_FUNC_OTHER_REP_SLAVEID_ENABLED 设置为 <code>1</code> 时使用。
 */
#define MB_FUNC_OTHER_REP_SLAVEID_BUF           ( 32 )

/*! \brief 是否启用 <em>报告从机 ID</em> 功能。 */
#define MB_FUNC_OTHER_REP_SLAVEID_ENABLED       (  1 )

/*! \brief 是否启用 <em>读输入寄存器</em> 功能。 */
#define MB_FUNC_READ_INPUT_ENABLED              (  1 )

/*! \brief 是否启用 <em>读保持寄存器</em> 功能。 */
#define MB_FUNC_READ_HOLDING_ENABLED            (  1 )

/*! \brief 是否启用 <em>写单个寄存器</em> 功能。 */
#define MB_FUNC_WRITE_HOLDING_ENABLED           (  1 )

/*! \brief 是否启用 <em>写多个寄存器</em> 功能。 */
#define MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  (  1 )

/*! \brief 是否启用 <em>读线圈</em> 功能。 */
#define MB_FUNC_READ_COILS_ENABLED              (  1 )

/*! \brief 是否启用 <em>写线圈</em> 功能。 */
#define MB_FUNC_WRITE_COIL_ENABLED              (  1 )

/*! \brief 是否启用 <em>写多个线圈</em> 功能。 */
#define MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED    (  1 )

/*! \brief 是否启用 <em>读离散输入</em> 功能。 */
#define MB_FUNC_READ_DISCRETE_INPUTS_ENABLED    (  1 )

/*! \brief 是否启用 <em>读/写多个寄存器</em> 功能。 */
#define MB_FUNC_READWRITE_HOLDING_ENABLED       (  1 )

/*! @} */
#ifdef __cplusplus
    PR_END_EXTERN_C
#endif
#endif

