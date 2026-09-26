/*
 * FreeModbus Libary: MSP430 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */



#ifndef _PORT_H
#define _PORT_H

/* ----------------------- Platform includes --------------------------------*/

#include "main.h"

#define REG_HOLD_SIZE   300
#define REG_INPUT_SIZE  2600

extern uint16_t REG_HOLD_BUF[REG_HOLD_SIZE];
extern uint16_t REG_INPUT_BUF[REG_INPUT_SIZE];

/* ----------------------- Defines ------------------------------------------*/
#define PR_BEGIN_EXTERN_C           extern "C" {
#define	PR_END_EXTERN_C             }

#define ENTER_CRITICAL_SECTION( )   EnterCriticalSection( )//这里没有使用RTOS，如果使用FreeRTOS可以直接替换为 taskENTER_CRITICAL()和 taskEXIT_CRITICAL()即可
#define EXIT_CRITICAL_SECTION( )   ExitCriticalSection( )
#ifdef assert
#undef assert
#endif

#define assert(expr)

typedef char    BOOL; //如果你使用了FreeRTOS，那么添加FreeRTOS的头文件就行了。

typedef unsigned char UCHAR;

typedef char    CHAR;

typedef unsigned short USHORT;
typedef short   SHORT;

typedef unsigned long ULONG;
typedef long    LONG;

#ifndef TRUE
#define TRUE            1
#endif

#ifndef FALSE
#define FALSE           0
#endif

void            EnterCriticalSection( void );
void            ExitCriticalSection( void );

void            serialReceiveOneByteISR( void );
void            serialSentOneByteISR( void );

#endif


