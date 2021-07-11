#include <reg52.h>
#include <intrins.h>

sfr P_SW1 = 0xA2;
sfr AUXR = 0x8e;
sfr T2H = 0xd6;
sfr T2L = 0xd7;

#define S1_S0 0x40              //P_SW1.6
#define S1_S1 0x80              //P_SW1.7

char UAR[16],temp;
unsigned int index = 0;
unsigned int valid = 0;
void UartInit(void)
{
       
//   ACC = P_SW1;
//   ACC &= ~(S1_S0 | S1_S1);    //S1_S0=0 S1_S1=0
//   P_SW1 = ACC;                //(P3.0/RxD, P3.1/TxD)
   
  ACC = P_SW1;
  ACC &= ~(S1_S0 | S1_S1);    //S1_S0=1 S1_S1=0
  ACC |= S1_S0;               //(P3.6/RxD_2, P3.7/TxD_2)
  P_SW1 = ACC;  
       
        EA = 1; //'ò¿a×üÖD¶Ï
        ES = 1; //'ò¿a'®¿úÖD¶Ï
       
//  9600
//        SCON = 0x50;
//        AUXR |= 0x01;
//        AUXR |= 0x04;
//        T2L = 0xE0;
//        T2H = 0xFE;
//        AUXR |= 0x10;
       
//    38400
                SCON = 0x50;    // SCON: Ä?ê½ 1, 8-bit UART, ê1Äü½óêÕ               
                AUXR |= 0x01;               
                AUXR |= 0x04;               
                T2L = 0xB8;               
                T2H = 0xFF;               
                AUXR |= 0x10;   

//    57600
//                SCON = 0x50;               
//                AUXR |= 0x01;               
//                AUXR |= 0x04;               
//                T2L = 0xD0;               
//                T2H = 0xFF;               
//                AUXR |= 0x10;   

}

void SendData(unsigned char dat)
{
    ES=0;
    SBUF=dat;
    while(!TI);
    TI=0;
    ES=1;
}

void SendString(char *s)
{
    while (*s)
    {
        SendData(*s++);
    }
}

void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main()
{
    UartInit();
    while(1)
    {
	     SendString(UAR);
			 Delay1000ms();
		}
}

void ser() interrupt 4
{
	if(RI) {
		RI = 0;
	  temp = SBUF;
		if (temp == 0x40)
			valid = 1;
		if (valid) {
		    UAR[index] = temp;
		    index++;
		    if(temp == 0x0D) {
		        index = 0;
					  valid = 0;
		    }
	  }
	}

}