 /******************************************************************************
 *
 * Module: I2C(TWI)
 *
 * File Name: i2c.h
 *
 * Description: Header file for the I2C(TWI) AVR driver
 *
 * Author: Salma Ali
 *
 *******************************************************************************/ 


#ifndef I2C_H_
#define I2C_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 // start has been sent
#define TW_REP_START     0x10 // repeated start 
#define TW_MT_SLA_W_ACK  0x18 // Master transmit ( slave address + Write request ) to slave + Ack received from slave
#define TW_MT_SLA_R_ACK  0x40 // Master transmit ( slave address + Read request ) to slave + Ack received from slave
#define TW_MT_DATA_ACK   0x28 // Master transmit data and ACK has been received from Slave.
#define TW_MR_DATA_ACK   0x50 // Master received data and send ACK to slave
#define TW_MR_DATA_NACK  0x58 // Master received data but doesn't send ACK to slave
#define ERROR 0
#define SUCCESS 1
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	TWPS_ZERO,TWPS_ONE,TWPS_TW0,TWPS_THREE
}TWPS;

typedef enum
{
	ONE=1,FOUR=4,SIXTEEN=16,SIXTY_FOUR=64
}prescaler_value;
typedef enum
{
	NORMAL_MODE=100000,FAST_MODE=400000,FAST_MODE_PLUS=1000000,HIGH_SPEED_MODE=3400000
}speed_modes;


typedef struct
{
	TWPS TWPS_BITS;
	prescaler_value PRESCALER;
	uint8 Slave_address;
	speed_modes SCL_FREQ;

}I2C_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(const I2C_ConfigType * Config_ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_write(uint8 data);
uint8 TWI_readWithACK(void); //read with send Ack
uint8 TWI_readWithNACK(void); //read without send Ack
uint8 TWI_getStatus(void);


#endif /* I2C_H_ */
