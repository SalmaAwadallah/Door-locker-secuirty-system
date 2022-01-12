/*
 * timer.h
 *
 *  Created on: Mar 16, 2021
 *      Author: salma
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}timer_Clock;

typedef enum
{
	NORMAL,PWM,CTC,FASTPWM
}timer_Mode;

typedef enum
{
	TIMER0,TIMER1,TIMER2
}timer_type;
typedef struct
{
	timer_type timer;
	timer_Clock clock;
	timer_Mode mode;
	uint16 initialValue ;
	uint16 compareValue ;
}timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the TIMER driver
 * 	1. Set the required clock.
 * 	2. Set the required waveform generation mode.
 * 	3. Enable timer0 overflow/compare match Interrupt.
 * 	4. Initialize Timer0 Registers
 */
void timer_init(const timer_ConfigType * Config_Ptr);

/*
 * Description: Function to set the Call Back function address for timer0.
 */
void timer0_setCallBack(void(*a_ptr)(void));
/*
 * Description: Function to set the Call Back function address for timer1.
 */
void timer1_setCallBack(void(*a_ptr)(void));
/*
 * Description: Function to set the Call Back function address for timer2.
 */
void timer2_setCallBack(void(*a_ptr)(void));



#endif /* TIMER_H_ */
