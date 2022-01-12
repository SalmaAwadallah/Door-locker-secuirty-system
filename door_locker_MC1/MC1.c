/*
 * MC1.c
 *
 *  Created on: Mar 16, 2021
 *      Author: salma
 */



#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"

#define M1_READY 0x10
#define NEW_PASSWORD 0x01
#define CURRENT_PASSWORD 0x02
#define RE_ENTER_PASSWORD 0x03
#define PASSWORDS_NOT_MATCHED 0x04
#define OPTIONS_MENU 0x06

static uint8 pressed_key;
static uint8 error_flag=0;

void error_timer(void);

int main()
{

	uint8 MC2_RequiredAction;

	uint8 stars_count=0;
	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);


	/* Initialize both the LCD and ICU driver */
	LCD_init();

	USART_ConfigType USART_Config={DISABLE,ONE_BIT,EGIHT,standard_6} ;
	UART_init(&USART_Config);

	UART_sendByte(M1_READY); // MC1 is ready
	while(1)
	{
		MC2_RequiredAction=UART_recieveByte();

		if(MC2_RequiredAction==NEW_PASSWORD)
		{
			stars_count=0;
			LCD_clearScreen();

           do
           {
        	   LCD_displayStringRowColumn(0,0,"PleaseEnterPass:");
        	   pressed_key=KeyPad_getPressedKey();

        	   if(pressed_key !=13)
        	   {
        		   LCD_goToRowColumn(1,stars_count);
        		   LCD_displayCharacter('*');
        	   }
        	    UART_sendByte(pressed_key);
        	    stars_count++;

           }while(pressed_key != 13 );

		}

		//check_for match
		else if(MC2_RequiredAction==RE_ENTER_PASSWORD)
		{
			stars_count=0;
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"Pls.Re-EnterPass:");
			do
			{
				pressed_key=KeyPad_getPressedKey();
				if(pressed_key !=13 )
				{
					LCD_goToRowColumn(1,stars_count);
					LCD_displayCharacter('*');
				}
				UART_sendByte(pressed_key);
				_delay_ms(500);
				stars_count++;
			}while(pressed_key !=13 );
		}
		else if(MC2_RequiredAction==CURRENT_PASSWORD)
		{
			stars_count=0;
			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"CURRENT PASSWORD:");
			do
			{
				pressed_key=KeyPad_getPressedKey();
				if(pressed_key !=13 )
				{
					LCD_goToRowColumn(1,stars_count);
					LCD_displayCharacter('*');
				}
				UART_sendByte(pressed_key);
				_delay_ms(500);
				stars_count++;
			}while(pressed_key !=13 );
		}
		//options menu
		else if(MC2_RequiredAction==OPTIONS_MENU)
		{

			LCD_clearScreen();
			LCD_displayStringRowColumn(0,0,"+: change pass");
			LCD_displayStringRowColumn(1,0,"-: open door");
			pressed_key=KeyPad_getPressedKey();
			UART_sendByte(pressed_key);

		}
		//password doesnt match for 3 times
		else if(MC2_RequiredAction==PASSWORDS_NOT_MATCHED)
		{
			LCD_clearScreen();
			// T_TIMER=1024/1MHZ , 0->58600 60SECS
			timer_ConfigType timer_Config={TIMER1,F_CPU_1024,CTC,0,58600} ;
			timer_init(&timer_Config);
			timer1_setCallBack(error_timer);

			error_flag=0;
			while(error_flag==0)
			{

				LCD_displayStringRowColumn(0,0,"ERROR");
			}

		}
	}

}


void error_timer(void)
{


	/* after 60 secs-> make error_flag=1 */

		error_flag=1;

}
