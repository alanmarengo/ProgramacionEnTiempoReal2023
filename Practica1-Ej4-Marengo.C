#include <dos.h>
#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <conio.h>

#define PIT_CH0_PORT 0x40  
#define PIT_CMD_PORT 0x43 

int xa,xn,ya,yn,k=0;
int flag=0,cont = 0;

unsigned int DIRBAS = 0x300;
unsigned int OUTBR = DIRBAS + 4;
unsigned int STINR = DIRBAS + 0;
unsigned int CTREG = DIRBAS + 0;
unsigned int ADHIG = DIRBAS + 9;
unsigned int ADLOW = DIRBAS + 8;
unsigned char A, B, C;
float VA,VD;


void interrupt (*viejoManejador)();

void interrupt nuevo_manejador(){
	cont++;
}

void calculartemperatura(int canal){
outportb (CTREG,canal);
C = inportb (ADLOW);
do {
C = inportb (STINR); 
C = C && 0x20; 
}while (C == 0x00);
A = inportb (ADHIG);
B = inportb (ADLOW);
VD = A * 256 + B;
VA = (float) 5 / 4096 * VD;
}

void grafico_abcsisas(){
   char ey,ex;
   int i,j;
   int gd = DETECT, gm;
   initgraph(&gd, &gm, "..\\BGI");

   setlinestyle(SOLID_LINE,0,2);

   line(100,460,100,60);
   line(100,460,600,460);
   line(90,70,100,60);
   line(110,70,100,60);
   line(590,450,600,460);
   line(590,470,600,460);

   outtextxy(85,35,"Voltaje Temperatura");
   outtextxy(613,452,"Segundos");

   setlinestyle(DOTTED_LINE,0,2);

   for(i=460,j=0;i>=60;i-=80,j++){
	line(100,i,600,i);
	itoa(j,&ey,10);
	outtextxy(85,i-5,&ey);
   }
      for(i=100;i<=600;i+=25){
	line(i,460,i,60);
	itoa(k,&ex,10);
	outtextxy(i,465,&ex);
	k++;
   }
   setcolor(GREEN);
   setlinestyle(SOLID_LINE,0,2);
}


void main(){
calculartemperatura(0);
grafico_abcsisas();
xa=100;
ya=(int)(-80*VA)+460;

viejoManejador=getvect(0x1c);
setvect(0x1c,nuevo_manejador);

outportb(PIT_CMD_PORT,0x36);
outportb(PIT_CH0_PORT,65535>>8);
outportb(PIT_CH0_PORT,65535);

do{
	
	
if(cont==18){
	calculartemperatura(0);
	if(xa==600){
		clearviewport();
		xa=100;
		grafico_abcsisas();
}
	xn=xa+25;
	yn=(int)(-80*VA)+460;
	line(xa,ya,xn,yn);
	xa=xn;
	ya=yn;
	if(VA>3.5){
		 outportb(OUTBR, 0x08);
}
	else if(VA>2.7){
		 outportb(OUTBR, 0x09);
}
}
}while(!kbhit());
setvect(0x1c,viejoManejador);
closegraph();
}
