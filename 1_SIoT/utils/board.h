#ifndef BOARD_H_
#define BOARD_H_

#include <msp430.h>
#include <intrinsics.h>
#include "adc.h"

void board_init();

void delay_ms( unsigned int ms);
void delay_us( unsigned int us);
void wait();

void SPI_Init();
void SPI_Send(char B);
void SPI_Read(char* ptr);




#endif /* BOARD_H_ */
