/*
 * timer.c
 *
 *  Created on: Mar 16, 2021
 *      Author: salma
 */


#include "timer.h"
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_timer0_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer1_callBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer2_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_timer0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer0_callBackPtr)();
	}
}
ISR(TIMER0_COMP_vect)
{
	if(g_timer0_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer0_callBackPtr)();
	}
}
ISR(TIMER1_OVF_vect)
{
	if(g_timer1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer1_callBackPtr)();
	}
}
ISR(TIMER1_COMPA_vect)
{
	if(g_timer1_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer1_callBackPtr)();
	}
}

ISR(TIMER2_OVF_vect)
{
	if(g_timer2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer2_callBackPtr)();
	}
}
ISR(TIMER2_COMP_vect)
{
	if(g_timer2_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_timer2_callBackPtr)();
	}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description : Function to call  the Timer_init() function based on timer type
 */
void timer_init(const timer_ConfigType * Config_Ptr)
{
	if(Config_Ptr->timer==0)
		timer0_init(Config_Ptr);
	else if(Config_Ptr->timer==1)
		timer1_init(Config_Ptr);
	else if(Config_Ptr->timer==2)
		timer2_init(Config_Ptr);
}

/*
 * Description : Function to initialize THE TIMER driver
 * 	1. Set the required clock.
 * 	2. Set the required waveform generation mode.
 * 	3. Enable timer0 overflow/compare match Interrupt.
 * 	4. Initialize Timer0 Registers
 */

void timer0_init(const timer_ConfigType * Config_Ptr)
{
	/* Timer0 always operates in a non-pwm mode*/
    TCCR0|=(1<<FOC0);
    /*
     * insert the required clock value in the first three bits (CS02, CS01 and CS00)
     * of TCCR0 Register
     */
    TCCR0= ( TCCR0 & 0xF8) | (Config_Ptr->clock);
    /*
     * insert the required waveform generation mode in bit no. 6 (WGM00) bit &
     * bit no.3 (WGM01) bit in TCCR0 Register
     */
    TCCR0= ( TCCR0 & 0xBF) | ((Config_Ptr->mode&0x01)<<6);
    TCCR0= ( TCCR0 & 0xF7) | ((Config_Ptr->mode&0x02)<<2);

    /* Initial Value for Timer0 */
    TCNT0 =Config_Ptr->initialValue ;
    /* Compare Value for Timer0 */
    OCR0=Config_Ptr->compareValue ;
    /*  Timer0 overflow interrupt enable*/
    if(Config_Ptr->mode==0)
    TIMSK|=(1<<TOIE0);
    /*  Timer0 compare match interrupt enable*/
    else if(Config_Ptr->mode==2)
    TIMSK|=(1<<OCIE0);


}
/*
 * Description: Function to set the Call Back function address.
 */
void timer0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_timer0_callBackPtr= a_ptr;
}
void timer1_init(const timer_ConfigType * Config_Ptr)
{
	/* Timer1 always operates in a non-pwm mode*/
	TCCR1A=(1<<FOC1A)|(1<<FOC1B);

    /*
     * insert the required clock value in the first three bits (CS12, CS11 and CS10)
     * of TCCR1B Register
     */
    TCCR1B= (TCCR1B & 0xF8) | Config_Ptr->clock;
    /*
     * insert the required waveform generation mode in bit no. 4 (WFGM13) bit &
     * bit no.3 (WGM12) bit in TCCR1B Register
     * and bit no.1 (WGM11)& bit no.0(WGM10)in TCCR1A Register
     */
    if(Config_Ptr->mode==2)
    TCCR1B|= (1<<WGM12);


    /* Initial Value for Timer1 */
    TCNT1 =Config_Ptr->initialValue ;
    /* Compare Value for Timer1 */
    OCR1A=Config_Ptr->compareValue ;
    /*  Timer1 overflow interrupt enable*/
    if(Config_Ptr->mode==0)
    TIMSK|=(1<<TOIE1);
    /*  Timer1 compare match interrupt enable for CHANNEL A*/
    else if(Config_Ptr->mode==2)
    TIMSK=(1<<OCIE1A);


}
/*
 * Description: Function to set the Call Back function address.
 */
void timer1_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_timer1_callBackPtr= a_ptr;
}
void timer2_init(const timer_ConfigType * Config_Ptr)
{
	/* Timer2 always operates in a non-pwm mode*/
    TCCR2|=(1<<FOC2);
    /*
     * insert the required clock value in the first three bits (CS22, CS21 and CS20)
     * of TCCR2 Register
     */
    TCCR2= ( TCCR2 & 0xF8) | (Config_Ptr->clock);
    /*
     * insert the required waveform generation mode in bit no. 6 (WGM20) bit &
     * bit no.3 (WGM21) bit in TCCR2 Register
     */
    TCCR2= ( TCCR2 & 0xBF) | ((Config_Ptr->mode&0x01)<<6);
    TCCR2= ( TCCR2 & 0xF7) | ((Config_Ptr->mode&0x02)<<2);

    /* Initial Value for Timer0 */
    TCNT2 =Config_Ptr->initialValue ;
    /* Compare Value for Timer0 */
    OCR2=Config_Ptr->compareValue ;
    /*  Timer0 overflow interrupt enable*/
    if(Config_Ptr->mode==0)
    TIMSK|=(1<<TOIE2);
    /*  Timer0 compare match interrupt enable*/
    else if(Config_Ptr->mode==2)
    TIMSK|=(1<<OCIE2);


}
/*
 * Description: Function to set the Call Back function address.
 */
void timer2_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_timer2_callBackPtr= a_ptr;
}
