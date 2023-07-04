#include <htc.h> 
#define _XTAL_FREQ 20000000 //1MHz osc

int flag =0;
int contador_ms=0;
void interrupt timer(void)
{
	contador_ms++;
	if(contador_ms==250){
            PORTB = PORTB ^ 0b00110000;
            contador_ms=0;
            T0IF=0;
            TMR0=236;
	}
	
}

void main()
{
    TRISB = 0b11001111;
	TRISA = 0b00011;
	OPTION_REG = 0x07;
    INTCON=0b00100000;
    TMR0=0;
    GIE=0;
    
    while(1)
    {
        if (RA0==0 || RA1==0) 
		{
            GIE=1;
		}
    }

}