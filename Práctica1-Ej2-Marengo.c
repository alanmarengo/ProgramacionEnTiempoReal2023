#include <stdio.h>
#include <dos.h>
#include <conio.h>


#define PIT_CH0_PORT 0x40  //DEFINO EL TIMER 1 DEL 8253
#define PIT_CMD_PORT 0x43 //MODO DE CONTROL
#define TIMER_RATEGEN  0x06    // MODO 3, GENERADOR DE ONDA CUADRADA
#define TIMER_FREQ    1193182 
#define TIMER_SEL0      0x00    // CONTADOR 0 DEL PIT 8253  
#define TIMER_16BIT     0x30    // r/w counter 16 bits, LSB first // (LSB then) MSB

unsigned long interrupt_counter = 0;
int second_counter = 0;
int flag = 0;
unsigned long pitfreq = 0;
unsigned long frecuencia=0;
unsigned long frecuenciabase= 1193800;

void interrupt (*oldTimer)(void);

void interrupt newhandler() {
    interrupt_counter++;
    flag = 1;
}

void init_pit_8253_channel0(unsigned long frequency){
    pitfreq = frecuenciabase / frequency;
    outportb(PIT_CMD_PORT, 0x36);
    outportb(PIT_CH0_PORT, pitfreq); //byte bajo
    outportb(PIT_CH0_PORT, (pitfreq>>8)); //byte alto
    //ESTOY UTILIZANDO EL MODO DE LECTURA/ESCRITURA : 1 1 = Modo de acceso: lobyte/hibyte 
}

int main(void){
    disable(); //desactivo las interrupciones.
    oldTimer=getvect(0x1C);  // Guardar el manejador original del temporizador
    setvect(0x1C, newhandler);// Instalar el nuevo manejador de interrupción
    printf("\nIngrese la frencuencia: ");
    scanf("%lu",&frecuencia);

    if (frecuencia < 1 || frecuencia > 65535) {
        printf("La frecuencia debe estar entre 1 y 65535.\n");
        return 0;
    }

    init_pit_8253_channel0(frecuencia); // Configurar el PIT para generar interrupciones a la frecuencia especificada
    enable();

    while (second_counter < 20) {
        if (kbhit()) break;
        if(flag==1){   
            flag=0;    
            printf("Interrupciones por segundo: %u\n",(unsigned int) interrupt_counter);
            printf("Frecuencia PIT 8253: %lu\n", (unsigned long) pitfreq);
            printf("Cantidad de tiempo  transcurrido en segundos: %d\n", second_counter);
            if(interrupt_counter==pitfreq)
            {
                interrupt_counter = 0; //RESETEO EL CONTADOR DE INTERRUPCIONES.
                second_counter++;
            }
        }
    }

    setvect(0x1C, oldTimer);
    printf(" \n");
	clrscr();
    return 0;
}