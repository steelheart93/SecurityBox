/*
 * File:   LCD.c
 * Author: Stiven Muñoz Murillo
 *
 * Created on 10 de julio de 2019, 05:13 PM
 */


#include <xc.h>
#define _XTAL_FREQ 4000000

void pulso() {
    RA0 = 0;
    RA0 = 1;
    __delay_ms(15);
    RA0 = 0;
}

void comando(int c) {
    PORTC = c;
    RA1 = 0;
    pulso();
}

void dato(int d) {
    PORTC = d;
    RA1 = 1;
    pulso();
}

void main(void) {
    int mensaje[10] = {'K', 'O', 'N', 'I', 'C', 'H', 'I', 'W', 'A', '!'};

    TRISA = 0;
    TRISC = 0;

    ADCON1 = 7;

    for (int i = 0; i < 10; i++) {
        int x = 128 + i;
        comando(x);
        dato(mensaje[i]);
    }

    comando(12);
}
