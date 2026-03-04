#include "board.h"
#include "Typedefs.h"

extern __Status_t Status;
extern char UART_RxBuf[8];

void board_init()
{
    WDTCTL = WDTPW | WDTHOLD;

    BCSCTL1 = CALBC1_8MHZ;  //
    DCOCTL = CALDCO_8MHZ;   //  MCLK = 8MHz
    BCSCTL3 &= 0xCF;        // SMCLK = 8MHz
    BCSCTL3 |= LFXT1S_2;    //  ACLK = 12kHz

    P1DIR = 0x03;
    P1REN = 0x04;
    P1OUT = 0x04;
    P1IFG = 0;
    P1IE = 0x04;
    P1IES &= 0xFB;
    __asm(" EINT"); // global interrupt enable
}

void delay_ms( unsigned int ms)
{
    if(ms<5461)
    {
        TAR = 0;
        TACCR0 = 12*ms;
        TACCTL0 = 0x0010;
        TACTL = 0x0110;
        __low_power_mode_3();
    }
}

void delay_us( unsigned int us)
{
    if(us<16383)
    {
        TAR = 0;
        TACCR0 = 4*us;
        TACCTL0 = 0x0010;
        TACTL = 0x0210;
        __low_power_mode_0();
    }
}

void wait()
{
    __low_power_mode_0();
}

void SPI_Init()
{
    UCB0CTL0 = 0xA9; // 1010 1001
    UCB0CTL1 = 0xC1; // 11.. ...1
    UCB0BR0 = 4;     // SLKC = 2MHz     <== 8MHz (SCLK) / 4 = 2MHz

    P3SEL &= 0xF0; // clear select bits for P3.0 .. P3.3
    P3SEL |= 0x0E; // SET sel bits for P3.1 .. P3.3 --> SPI

    // CSN line initialization
    P3DIR = 0x01; // P3.0 = OUTPUT
    P3OUT = 0x01; // P3.0 -> HIGH (communication is disabled for CC2500)

    UCB0CTL1 &= 0xFE; // clear UCSWRST bit --> enable SPI
}

void SPI_Send(char B)
{
    IFG2 &= ~UCB0RXIFG; // clear UCB0RXIFG bit
    UCB0TXBUF = B; // send data byte
   // while(!(IFG2 & UCB0TXIFG));// wait till TXBuffer is empty (this does not mean that the whole byte is sent out)
    while(!(IFG2 & UCB0RXIFG)); // wait till RXBuffer is not empty
}

void SPI_Read(char* ptr)
{
    *ptr = UCB0RXBUF;
}



#pragma vector=PORT1_VECTOR
__interrupt void Port1_ISR()
{
    if(P1IFG & BIT2)
    {
        P1IFG &= 0xFB;
        Status.Button = 1;
    }
    __low_power_mode_off_on_exit();
}

#pragma vector=PORT2_VECTOR
__interrupt void Port_2_ISR(void)
{
    if(P2IFG & 0b10000000)
    {
        P2IFG &= 0b01111111;
        Status.GDO_2_Set = 1;
    }
    if(P2IFG & 0b01000000)
    {
        P2IFG &= 0b10111111;
        Status.GDO_0_Set = 1;
    }
    __low_power_mode_off_on_exit();
}

#pragma vector=TIMERA0_VECTOR
__interrupt void TIMERA0()
{
    TACTL &= 0xFFCF;
    __low_power_mode_off_on_exit();
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void UART_Rx()
{
    if(IFG2 & 0x01)
    {
        if(UCA0RXBUF != 13)
        {
            UART_RxBuf[Status.UART_RX_cnt] = UCA0RXBUF;
            UCA0TXBUF = UART_RxBuf[Status.UART_RX_cnt++];
            while(UCA0STAT & 0x01);
        }
        else
            Status.UART_RX = 1;
    }
    __low_power_mode_off_on_exit();
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC_Done(void)
{
    adcStatus.DataReady = 1;
    adcStatus.ADC_Val = ADC10MEM;
    __low_power_mode_off_on_exit();
}
