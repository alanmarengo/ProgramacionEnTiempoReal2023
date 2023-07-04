#include <htc.h>
#include <stdlib.h>
#include <stdio.h>

#define _XTAL_FREQ 1000000							

unsigned short datoconv, valor, datoH, datoL;

unsigned short conversor(datoconv)					
{
	return (((datoconv/10) <<4 )+ datoconv%10);
}

void main() 
{
	ADCON1 = 0b10001110; 	
	ADCON0 = 0b01000001; 	
	ADIE = 0; 				
	TRISA = 0b11111; 		
	TRISB = 0b00000000; 	
	TRISD = 0b00000000; 	
	GIE = 0; 				
	GO = 1; 				

	while(1)
	{
		while(GO){}									
		valor = ADRESH << 8 | ADRESL;				
		datoH = valor/100;						
		datoL = valor-datoH*100;		 		
		datoH = conversor(datoH); 					
		datoL = conversor(datoL); 					
		RD7 = 1; 									
		RD6 = 0; 									
		PORTB = datoH; 								
		RD7 = 0; 									
		RD6 = 1; 									
		PORTB = datoL; 								
		GO = 1; 									
	}
}
