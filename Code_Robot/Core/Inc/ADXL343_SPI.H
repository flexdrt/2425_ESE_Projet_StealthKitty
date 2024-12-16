#ifndef ADXL343_SPI_H
#define ADXL343_SPI_H


#include "stm32g4xx_hal.h"
#include <string.h>
#include <stdio.h>

#define ADXL_CS_PORT GPIOC
#define ADXL_CS_PIN  GPIO_PIN_4

static const uint8_t ADXL343_DEVICE_ID =  			0xE5;



static const uint8_t ADXL343_REG_DEVICE_ID =     	0x00;
static const uint8_t ADXL343_REG_THRESH_TAP =    	0x1D;// mettre une valeur 20
static const uint8_t ADXL343_REG_OFSX =          	0x1E;
static const uint8_t ADXL343_REG_OFSY =         	0x1F;
static const uint8_t ADXL343_REG_OFSZ =          	0x20;
static const uint8_t ADXL343_REG_DUR =           	0x21;
static const uint8_t ADXL343_REG_LATENT =        	0x22;
static const uint8_t ADXL343_REG_WINDOW =        	0x23;
static const uint8_t ADXL343_REG_THRESH_ACT =    	0x24;
static const uint8_t ADXL343_REG_THRESH_INACT =  	0x25;
static const uint8_t ADXL343_REG_TIME_INACT =    	0x26;
static const uint8_t ADXL343_REG_ACT_INACT_CTL = 	0x27;
static const uint8_t ADXL343_REG_THRESH_FF =     	0x28;
static const uint8_t ADXL343_REG_TIME_FF =       	0x29;
static const uint8_t ADXL343_REG_TAP_AXES =      	0x2A;
static const uint8_t ADXL343_REG_ACT_TAP_STATUS = 	0x2B;
static const uint8_t ADXL343_REG_BW_RATE =      	0x2C;
static const uint8_t ADXL343_REG_POWER_CTL =     	0x2D;
static const uint8_t ADXL343_REG_INT_ENABLE =    	0x2E;
static const uint8_t ADXL343_REG_INT_MAP =       	0x2F;
static const uint8_t ADXL343_REG_INT_SOURCE =    	0x30;
static const uint8_t ADXL343_REG_DATA_FORMAT =   	0x31;
static const uint8_t ADXL343_REG_DATAX0 =        	0x32;
static const uint8_t ADXL343_REG_DATAX1 =        	0x33;
static const uint8_t ADXL343_REG_DATAY0 =        	0x34;
static const uint8_t ADXL343_REG_DATAY1 =        	0x35;
static const uint8_t ADXL343_REG_DATAZ0 =        	0x36;
static const uint8_t ADXL343_REG_DATAZ1 =        	0x37;
static const uint8_t ADXL343_REG_FIFO_CTL =      	0x38;
static const uint8_t ADXL343_REG_FIFO_STATUS =   	0x39;

typedef struct
{

	SPI_HandleTypeDef *hspiHandle;

	// Raw acceleration data X Y Z
	int16_t acc_rawX;
	int16_t acc_rawY;
	int16_t acc_rawZ;

	// Acceleration data X Y Z
	float acc_msp2X;
	float acc_msp2Y;
	float acc_msp2Z;
} ADXL343;

uint8_t ADXL343_initialization(SPI_HandleTypeDef *hspi, ADXL343 *dev);
//void ADXL343_set_offset(ADXL343 *dev);
//void ADXL343_print_norm(ADXL343 *dev, UART_HandleTypeDef *huart);
HAL_StatusTypeDef ADXL343_get_acc_raw(ADXL343 *dev);
HAL_StatusTypeDef ADXL343_get_acc_norm(ADXL343 *dev);
void ADXL343_print_raw(ADXL343 *dev, UART_HandleTypeDef *huart);

void ADXL343_set_tap_parameters(ADXL343 *dev, uint8_t threshold, uint8_t duration, uint8_t latency, uint8_t window);
uint8_t ADXL343_read_register(SPI_HandleTypeDef *hspi, uint8_t regAddr);
//void ADXL343_EnableInterrupts(SPI_HandleTypeDef *hspi);
void ADXL343_EnableTapInterrupts(ADXL343 *dev);
void ADXL343_check_tap(ADXL343 *dev) ;
void ADXL343_set_tap_threshold(ADXL343 *dev, uint8_t threshold);
#endif
