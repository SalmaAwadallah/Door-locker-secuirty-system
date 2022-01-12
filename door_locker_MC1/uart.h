 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Salma Ali
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"



/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	DISABLE,RESERVED,EVEN_PARITY,ODD_PARITY
}parity_mode;

typedef enum
{
	ONE_BIT,TWO_BITS
}stopBit_select;

typedef enum
{
	FIVE,SIX,SEVEN,EGIHT,RESREVED_1,RESREVED_2,RESREVED_3,NINE
}character_size;

typedef enum
{
	standard_0=100,standard_1=200,standard_2=300,standard_3=1200,standard_4=2400,
	standard_5=4800,standard_6=9600,standard_7=19200,standard_8=38400,standard_9=57600,standard_10=115200
}USART_BAUDERATE;

typedef struct
{
	parity_mode parity;
	stopBit_select stopBits;
	character_size ch_size;
	USART_BAUDERATE BAUD_RATE;
}USART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const USART_ConfigType * Config_Ptr);

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
