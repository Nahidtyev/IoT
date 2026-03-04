#ifndef UART_H_
#define UART_H_

#include <msp430.h>
#include "../utils/Typedefs.h"

void UART_Init();
void UART_TX(char byte);
void SerialSendByte(char B);
void SerialWrite(char *p);
void SerialWriteln(char *p);

unsigned int AToI();
char* IToA(long int val, int BASE);
char* FToA(double val, unsigned char numbers);

void UART_changeBDR(char *rateCH);

#endif /* UART_H_ */
