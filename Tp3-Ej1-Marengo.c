#include <htc.h> 
#define _XTAL_FREQ 1000000  //1MHz osc

void main()
{
   int flag=0;
   TRISB = 0b11001111; //Config puerto B /habilita las salidas de los LED 
   TRISA = 0b00011; //Config puerto A /habilita como entradas los pulsadores

	RB5 = 1; //enciende LED verde R4 al comenzar el programa
	RB4 = 1; //enciende LED rojo R5 al comenzar el programa
     
      while (1)
      {
         if (RA0==0 || RA1==0) //habilita el parpadeo de luces si se apreta el pulsador
         {
            flag=1; 
         }

         while(flag==1)
         {
            PORTB = PORTB ^ 0b00110000; // invierte bits, necesario para encender y apagar los leds
			   __delay_ms(250); //funcion para el delay cada 250 ms

         }
      }
      
   
}
