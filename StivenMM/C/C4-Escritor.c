/**
 * Programa que permite escribir en un LCD utilizando el Teclado Matricial.
 * 
 * @author Stiven Muñoz Murillo
 * @version 16/07/2019
 */

#include <xc.h>
#include <string.h>
#define _XTAL_FREQ 4000000

int contador = 0;

void pulso() {
    RA4 = 0;
    RA4 = 1;
    __delay_ms(15);
    RA4 = 0;
}

void comando(int c) {
    PORTC = c;
    RA5 = 0;
    pulso();
}

void dato(int d) {
    PORTC = d;
    RA5 = 1;
    pulso();
}

/**
 * Muestra un mensaje de menos de 16 caracteres.
 * @param mensaje
 */
void mensajeCorto(char mensaje[]) {
    for (int i = 0; i < strlen(mensaje); i++) {
        comando(128 + i); // Set DD RAM address
        dato(mensaje[i]);
    }
}

void __interrupt() listener() {
    int leer = PORTA;
    leer = PORTB;

    INTF = 0;
    RBIF = 0;

    if (RB0 == 1) {
        contador = 0;
        mensajeCorto("Borrando!!!");
        __delay_ms(1000);
        comando(1);
    }

    if (RA0 == 0) {
        if (RB4 == 0) {
            comando(128 + contador);
            contador++;
            dato('7');
        } else if (RB5 == 0) {
            comando(128 + contador);
            contador++;
            dato('8');
        } else if (RB6 == 0) {
            comando(128 + contador);
            contador++;
            dato('9');
        } else if (RB7 == 0) {
            comando(128 + contador);
            contador++;
            dato('/');
        }
    } else if (RA1 == 0) {
        if (RB4 == 0) {
            comando(128 + contador);
            contador++;
            dato('4');
        } else if (RB5 == 0) {
            comando(128 + contador);
            contador++;
            dato('5');
        } else if (RB6 == 0) {
            comando(128 + contador);
            contador++;
            dato('6');
        } else if (RB7 == 0) {
            comando(128 + contador);
            contador++;
            dato('*');
        }
    } else if (RA2 == 0) {
        if (RB4 == 0) {
            comando(128 + contador);
            contador++;
            dato('1');
        } else if (RB5 == 0) {
            comando(128 + contador);
            contador++;
            dato('2');
        } else if (RB6 == 0) {
            comando(128 + contador);
            contador++;
            dato('3');
        } else if (RB7 == 0) {
            comando(128 + contador);
            contador++;
            dato('-');
        }
    } else if (RA3 == 0) {
        if (RB4 == 0) {
            comando(128 + contador);
            contador++;
            dato('S');
        } else if (RB5 == 0) {
            comando(128 + contador);
            contador++;
            dato('0');
        } else if (RB6 == 0) {
            comando(128 + contador);
            contador++;
            dato('=');
        } else if (RB7 == 0) {
            comando(128 + contador);
            contador++;
            dato('+');
        }
    }

    GIE = 1;
}

void main() {
    nRBPU = 0; // Habilito las resistencias de Pull UP

    TRISA = 0; // Declaro los pines del puerto A como Salida
    TRISB = 0b11110001; // Declaro los pines de B como Salida y Entrada 
    TRISC = 0; // Declaro los pines del puerto B como Salida

    ADCON1 = 7; // Declaro el puerto A como Digital

    comando(12); // 1100, Display ON - Cursor OFF - Blink OFF

    GIE = 1;
    RBIF = 0;
    INTF = 0;
    INTE = 1;
    RBIE = 1;

    while (1) {
        PORTA = 14;
        __delay_ms(100);
        PORTA = 13;
        __delay_ms(100);
        PORTA = 11;
        __delay_ms(100);
        PORTA = 7;
        __delay_ms(100);
    };
}
