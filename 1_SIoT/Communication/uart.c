#include "uart.h"
const char _buflen = 16;
extern char UART_RxBuf[16];
extern char UART_TxBuf[17];
extern __Status_t Status;

void UART_Init()
{
    P3SEL |= 0x30;
    UCA0CTL1 = UCSSEL_2;
    UCA0BR0 = (833 & 0xFF);
    UCA0BR1 = (833 & 0xFF00) >> 8;
    UCA0MCTL = UCBRS_2;
    IE2 = 0x01;
    UCA0CTL1 &= ~UCSWRST;
    Status.UART_RX_cnt = 0;
    Status.UART_RX_cnt_LAST = Status.UART_RX_cnt-1;
    Status.UART_RX = 0;
    for(char i=0; i<8; i++)
        UART_RxBuf[i] = 0;
    UART_TxBuf[16] = 0;
 }

unsigned int AToI()
{
    union{
        unsigned char B;
        struct{
            unsigned char __unused  :5;
            unsigned char cnt       :3;
        };
    }IDx;
    IDx.cnt = Status.UART_RX_cnt-1;
    unsigned int tmp = 0;
    unsigned int mult = 1;
    do{
        tmp = tmp + (UART_RxBuf[IDx.cnt] - 48)*mult;
        IDx.cnt = IDx.cnt-1;
        mult *= 10;
    }while(IDx.cnt != Status.UART_RX_cnt_LAST);
    Status.UART_RX_cnt_LAST = Status.UART_RX_cnt-1;
    return tmp;
}

char* IToA(long int val, int BASE)
{
    if(val==0) return "0";
    int tmp = val;
    int i=16;
    if(val<0 && BASE==10) val = val*(-1);

    for(;val && i; --i, val/=BASE)
        UART_TxBuf[i] = "0123456789ABCDEF"[val % BASE];
    if(BASE == 16 && i>2)
    {
        UART_TxBuf[--i] = 'x';
        UART_TxBuf[--i] = '0';
    }
    if(tmp<0 && BASE==10 && i)
        UART_TxBuf[--i] = '-';
    return &UART_TxBuf[i+1];
}

void UART_TX(char byte)
{
    while(UCA0STAT & UCBUSY);
    UCA0TXBUF = byte;
    while(UCA0STAT & UCBUSY);
}

void SerialSendByte(char B)
{
    while(UCA0STAT & UCBUSY);
   UCA0TXBUF = B;
}

void SerialWrite(char *p)
{
    while(*p)
    {
        while(UCA0STAT & UCBUSY);
        UCA0TXBUF = *p;
        p++;
    }
}

void SerialWriteln(char *p)
{
    while(*p)
    {
        while(UCA0STAT & UCBUSY);
        UCA0TXBUF = *p;
        p++;
    }
    while(UCA0STAT & UCBUSY);
    UCA0TXBUF = '\n';
    while(UCA0STAT & UCBUSY);
    UCA0TXBUF = '\r';
}


char* FToA(double val, unsigned char numbers)
{
    if(val!=val) return "NaN";
    numbers = (numbers == 0xFF) ? 3 : ((numbers > 14) ? 14 : numbers);
    char tmp = 0;
    unsigned char i = _buflen+1;
    unsigned char j = 1;
    while(i)
    {
        UART_TxBuf[i--]='\0';
    }
    if(val == 0.0)
    {
        UART_TxBuf[0] = '0';
        UART_TxBuf[1] = '.';
        UART_TxBuf[2] = '0';
        for(i=3;i<numbers-2;i++)
            UART_TxBuf[i]=' ';
        return &UART_TxBuf[0];
    }
    if(val < 0)
    {
        val = fabs(val);
        UART_TxBuf[0] = '-';
    }
    else
        UART_TxBuf[0] = ' ';
    i=1;
    while(val >= 10)
    {
        val /= 10;
        j++;
        if(j > numbers)
            return "-error-";
    }
    while((--j > 0) || numbers)
    {
        tmp = val;
        val = 10 * (val - tmp);
        UART_TxBuf[i] = (char)(tmp + 0x30);
        numbers--; i++;
        if(!j)
        {
            UART_TxBuf[i] = '.';
            i++;
        }
        if(!numbers)
            break;
    }
    return &UART_TxBuf[0];
}

void UART_changeBDR(char *rateCH)
{

}



