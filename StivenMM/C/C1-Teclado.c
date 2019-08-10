/*
 * File:   Teclado.c
 * Author: Stiven Muñoz Murillo
 *
 * Created on 9 de julio de 2019, 05:13 PM
 */


#include <xc.h>

void __interrupt() listener() {
    RBIF = 0;

    if (RB0 == 0) {
        if (RB4 == 0) {
            PORTC = 7;
        } else if (RB5 == 0) {
            PORTC = 8;
        } else if (RB6 == 0) {
            PORTC = 9;
        } else if (RB7 == 0) {
            PORTC = 10;
        }
    } else if (RB1 == 0) {
        if (RB4 == 0) {
            PORTC = 4;
        } else if (RB5 == 0) {
            PORTC = 5;
        } else if (RB6 == 0) {
            PORTC = 6;
        } else if (RB7 == 0) {
            PORTC = 11;
        }
    } else if (RB2 == 0) {
        if (RB4 == 0) {
            PORTC = 1;
        } else if (RB5 == 0) {
            PORTC = 2;
        } else if (RB6 == 0) {
            PORTC = 3;
        } else if (RB7 == 0) {
            PORTC = 12;
        }
    } else if (RB3 == 0) {
        if (RB4 == 0) {
            PORTC = 15;
        } else if (RB5 == 0) {
            PORTC = 0;
        } else if (RB6 == 0) {
            PORTC = 14;
        } else if (RB7 == 0) {
            PORTC = 13;
        }
    }

    GIE = 1;
}

void main() {
    nRBPU = 0;

    TRISB = 240;
    TRISC = 0;
    PORTC = 0;

    GIE = 1;
    RBIF = 0;
    RBIE = 1;

    while (1) {
        PORTB = 14;
        PORTB = 13;
        PORTB = 11;
        PORTB = 7;
    };
}