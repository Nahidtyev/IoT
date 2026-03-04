#include <msp430.h>
#include <intrinsics.h>
#include <main.h>


void main()
{
    board_init();
    UART_Init();
    SPI_Init();

    CC_Protocol_t CC_TX, CC_RX;
    CC_TX.LEN = 0;
    CC_RX.LEN = 0;
    CC_TX.ADDRESS = 0;
    CC_RX.ADDRESS = 0;
    for(unsigned char i=0; i<62; i++)
    {
        CC_TX.DATA[i] = 0;
        CC_RX.DATA[i] = 0;
    }

    // networking
    CC_SetChannel( 0xE7 );  // all devices in this network must use the same channel
    CC_SetAddress( 0xC3 );    // the device address must be unique inside the network

    // send "hello world"
    CC_AddData(&CC_TX, "Hello World ");
    CC_AddData(&CC_TX, IToA(52, 10));
    CC_TX.ADDRESS = 0xC4; // the address of destination node
    CC_SendMessage(CC_TX); // send message

    Status.GDO_0_Set = 0;
    CC_Strobe(CC2500_SRX); // Go to RX state

    {
        char bytes=0;
        int cnt = 0;
        while(!Status.GDO_0_Set)
        {
            CC_ReadReg(CC2500_RXBYTES | 0xC0, &bytes); // read RXBYTES register value
            if(bytes == 0)
                cnt++;
            else
                break; // go out of this loop

            if(cnt==10)
            {
                cnt=0;
                delay_ms(1000);
            }
            else
                delay_us(100);
        }
        CC_ReadMessage(&CC_RX);
    }




    while(1)
    {
        SerialWrite("Hello");
        SerialWriteln(" World!");
    }
}
