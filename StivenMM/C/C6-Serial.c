/**
 * Programa que carga en un PIC un sistema de Transmisión Serial.
 * 
 * @author Stiven Muñoz Murillo
 * @version 30/07/2019
 */

#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <string.h>
#define _XTAL_FREQ 4000000

void trasmitir(char palabra[]) {
    for (int i = 0; i < strlen(palabra); i++) {
        TXREG = palabra[i];
        TXEN = 1;

        while (TXIF == 0) {
            ;
        }
    }

    TXREG = 10;
    TXEN = 1;

    while (TXIF == 0) {
        ;
    }

    TXREG = 13;
    TXEN = 1;

    while (TXIF == 0) {
        ;
    }
}

void main(void) {
    TRISC = 128;

    SPEN = 1;
    SYNC = 0;
    BRGH = 1;
    SPBRG = 25;

    trasmitir("Hola Mundo");
    trasmitir("Adios Mundo");

    while (1) {
        ;
    }
}
