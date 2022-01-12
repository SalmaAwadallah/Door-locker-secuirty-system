/*
 * MC2.c
 *
 *  Created on: Mar 16, 2021
 *      Author: salma
 */


#include "uart.h"
#include "timer.h"
#include "i2c.h"

#define M1_READY 0x10
#define NEW_PASSWORD 0x01
#define CURRENT_PASSWORD 0x02
#define RE_ENTER_PASSWORD 0x03
#define PASSWORDS_NOT_MATCHED 0x04
#define OPEN_DOOR 0x05
#define OPTIONS_MENU 0x06

static uint8 pressed_key;
static uint8 flag_Clockwise=0;
static uint8 clockwise_count=0;
static uint8 flag_antiClockwise=0;
static uint8 anticlockwise_count=0;
static uint8 flag_stop=0;
static uint8 buzzer_count;
static uint8 buzzer_flag=0 ;

void get_pass(uint16 start_address);
uint8 check_pass(uint16 start_address);
void password(uint16 start_address);
void clockwise (void);
void stop (void);
void anti_clockwise(void);
void buzzer(void);
uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data);
uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data);

int main()
{


	uint16 mem_address=0x0311;
    uint8 count=0;
    uint8 flag=0;

	/*configure PD2,PD3 in PORTD as output pins (motor INT1 & INT2 pins)*/
    DDRD|=(1<<PD2)|(1<<PD3);

   /*configure PA4 in PORTA as output pin (buzzer pin)*/
    DDRA|=(1<<PA4);

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Initialize both the UART and TWI driver */
	USART_ConfigType USART_Config={DISABLE,ONE_BIT,EGIHT,standard_6} ;
	UART_init(&USART_Config);

	I2C_ConfigType i2c_Config={TWPS_ZERO,ONE,0x01,FAST_MODE} ;
	TWI_init(&i2c_Config);


	while(UART_recieveByte() != M1_READY){} // wait until MC1 is ready
	 UART_sendByte(NEW_PASSWORD);
	 password(mem_address);

	 while(1)
	{


		 /* step2 (getting password process is done and we have a saved password in eeprom*/
		 UART_sendByte(OPTIONS_MENU);
		 pressed_key=UART_recieveByte();

		 if(pressed_key == '+')
		 {
			 /*check for pass*/
			 UART_sendByte(CURRENT_PASSWORD);
			 flag=check_pass(mem_address);

			 /* if match (go to step 1) */

			 if (flag==0)
			 {

				 UART_sendByte(NEW_PASSWORD);
				 password(mem_address);
			 }
			 /* if not match
			      check for mismatch three times then trigger buzzer for 1 minute and error message
			  */
			 else
			 {
				 count=0;
				 do
				 {
					 UART_sendByte(CURRENT_PASSWORD);
					 flag=check_pass(mem_address);
					 count++;
				 }while (!((flag ==0) || (count ==3)));

				 if (flag==0)
				 {
					 UART_sendByte(NEW_PASSWORD);
					 password(mem_address);

				 }

				 else
				 {
					 UART_sendByte(PASSWORDS_NOT_MATCHED);

					 /* trigger buzzer for 1 minute*/
					 timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,62536} ;
					 timer_init(&timer_Config);
					 timer1_setCallBack(buzzer);
					 buzzer_flag=0;
					 while(buzzer_flag==0)
					 {
						 PORTA|=(1<<PA4);
					 }
					 PORTA &=(~(1<<PA4));

				 }
			 }

		 }
		 else if (pressed_key =='-')
		 {
			 /* check for pass*/
			 UART_sendByte(CURRENT_PASSWORD);
			 flag=check_pass(mem_address);

			 /* if match
					 -rotate motor clockwise for 15 secs
					 -stop motor for 3 secs
					 -rotate motor anti clock wise for 15 secs
			  */

			 if (flag==0)
			 {

				 timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,62536} ;
				 timer_init(&timer_Config);
				 timer1_setCallBack(clockwise);
				 flag_Clockwise=0;
				 while(flag_Clockwise==0)
				 {
					 PORTD&=(~(1<<PD2));
					 PORTD |=(1<<PD3);

				 }
				 flag_stop=0;
				 while(flag_stop==0)
				 {
					 PORTD &=(~(1<<PD2));
					 PORTD &=(~(1<<PD3));

				 }
				 flag_antiClockwise=0;
				 while(flag_antiClockwise==0)
				 {
					 PORTD|=(1<<PD2);
					 PORTD &=(~(1<<PD3));
				 }

				 PORTD &=(~(1<<PD2));
				 PORTD &=(~(1<<PD3));

			 }

			 /* if not match
				   check for mismatch three times then trigger buzzer for 1 minute and error message
			  */

			 else
			 {
				 count=0;
				 do
				 {
					 UART_sendByte(CURRENT_PASSWORD);
					 flag=check_pass(mem_address);
					 count++;
				 }while (!((flag ==0) || (count ==3)));

				 if (flag==0)
				 {

					 timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,62536} ;
					 timer_init(&timer_Config);
					 timer1_setCallBack(clockwise);
					 flag_Clockwise=0;
					 while(flag_Clockwise==0)
					 {
						 PORTD|=(1<<PD2);
						 PORTD &=(~(1<<PD3));
					 }
					 flag_stop=0;
					 while(flag_stop==0)
					 {
						 PORTD &=(~(1<<PD2));
						 PORTD &=(~(1<<PD3));

					 }

					 flag_antiClockwise=0;
					 while(flag_antiClockwise==0)
					 {
						 PORTD&=(~(1<<PD2));
						 PORTD |=(1<<PD3);
					 }

					 PORTD &=(~(1<<PD2));
					 PORTD &=(~(1<<PD3));

				 }

				 else
				 {
					 /* error message */
					 UART_sendByte(PASSWORDS_NOT_MATCHED);

					 /* trigger buzzer for 1 minute*/
					 timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,62536} ;
					 timer_init(&timer_Config);
					 timer1_setCallBack(buzzer);
					 buzzer_flag=0;
					 while(buzzer_flag==0)
					 {
						 PORTA|=(1<<PA4);
					 }
					 PORTA &=(~(1<<PA4));

				 }
			 }
		 }


	}
}
void password (uint16 start_address)
{
	/*step1*/
	uint8 match_flag=1;
	while(match_flag==1)
	{
		/*take pass for first time*/

		get_pass(start_address);

		/*take pass for second time and check if 2 pass match*/
		UART_sendByte(RE_ENTER_PASSWORD);
		match_flag=check_pass(start_address);
	}

}
void get_pass(uint16 start_address)
{

	do
	{
		pressed_key=UART_recieveByte();
		EEPROM_writeByte(start_address, pressed_key);
		_delay_ms(10);
		start_address++;


	}while(pressed_key != 13);


}

uint8 check_pass(uint16 start_address)
{

	uint8 saved_key;
	uint8 flag=0;
	do
	{
		pressed_key=UART_recieveByte();
		EEPROM_readByte(start_address, &saved_key);
		start_address++;
		if(pressed_key != saved_key)
			flag=1;

	}while(pressed_key != 13);
	return flag;

}


void clockwise (void)
{

	/* Create configuration structure for timer1 driver*/

	clockwise_count++;

	if (clockwise_count ==1)
	{
		/*T_TIMER=1024/8MHZ, 0->54719=7 SECS
		 */
		timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,54719} ;
		timer_init(&timer_Config);
		timer1_setCallBack(clockwise);
	}
	else
	{
		/* after 15 secs-> make flag_Clockwise=1, call the stop function*/

		flag_Clockwise=1;
		clockwise_count=0;
		timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,23451} ;
		timer_init(&timer_Config);
		timer1_setCallBack(stop);


	}



}
void stop(void)
{

	/* Create configuration structure for timer1 driver
	 *T_TIMER=1024/8MHZ, 0->23451 =3 SECS
	 */
	flag_stop=1;
	timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,62536} ;
	timer_init(&timer_Config);
	timer1_setCallBack(anti_clockwise);
}
void anti_clockwise(void)
{

	/* Create configuration structure for timer1 driver*/

		/*T_TIMER=1024/8MHZ, 0->62536 =8 SECS
		 */

		anticlockwise_count++;


	 if (anticlockwise_count ==1)
	{

		/*T_TIMER=1024/8MHZ, 0->54719=7 SECS
		 */
		timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,54719} ;
		timer_init(&timer_Config);
		timer1_setCallBack(anti_clockwise);
	}
	else
	{
		/* after 60 secs-> make flag=1, stop the timer */
		anticlockwise_count=0;
		flag_antiClockwise=1;

	}

}
void buzzer(void)
{

	buzzer_count++;
	/* count 8secs 7 times-> (8*7)=56 */
	if(buzzer_count <=6)
	{

		/* Create configuration structure for timer1 driver
		 *T_TIMER=1024/8MHZ, 0->62536  =8 SECS
		 */
		timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,62536} ;
									timer_init(&timer_Config);
									timer1_setCallBack(buzzer);
	}
	/* count 4 secs 1 time-> total time 56+4= 60secs=1 minute*/
	else if(buzzer_count==7)
	{
		/* Create configuration structure for timer1 driver
		 *F_TIMER=1024/8MHZ, 0->31268 =4SECS
		 */
		timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,31268} ;
		timer_init(&timer_Config);
		timer1_setCallBack(buzzer);
	}
	else
	{
		/* after 60 secs-> make flag=1, stop the timer */

		buzzer_count=0;
		buzzer_flag=1;
	}

}

uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_write((uint8)(0xA0 | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address */
    TWI_write((uint8)(u16addr));
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;

    /* write byte to eeprom */
    TWI_write(u8data);
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}

uint8 EEPROM_readByte(uint16 u16addr, uint8 *u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;

    /* Send the required memory location address */
    TWI_write((uint8)(u16addr));
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;

    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_REP_START)
        return ERROR;

    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TW_MT_SLA_R_ACK)
        return ERROR;

    /* Read Byte from Memory without send ACK */
    *u8data = TWI_readWithNACK();
    if (TWI_getStatus() != TW_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();
    return SUCCESS;
}
