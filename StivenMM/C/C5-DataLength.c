/**
 * Programa que carga un mensaje en un LCD, usando solo 4 bits.
 * 
 * @author Stiven Muñoz Murillo
 * @version 24/07/2019
 */

#include <xc.h>
#include <string.h>
#define _XTAL_FREQ 4000000

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
 * Escribe un mensaje corto en la pantalla LCD usando solo 4 pines del PIC.
 * @param mensaje
 */
void mensajeCuatroBits(char mensaje[]) {
    comando(32); // Activar la escritura en 4 bits

    for (int i = 0; i < strlen(mensaje); i++) {
        int letra = mensaje[i];
        PORTB = letra;

        int masSignificativos = RB4 * 16 + RB5 * 32 + RB6 * 64 + RB7 * 128;
        int menosSignificativos = RB0 * 16 + RB1 * 32 + RB2 * 64 + RB3 * 128;

        dato(masSignificativos);
        dato(menosSignificativos);
    }
}

void main() {
    TRISA = 0; // Declaro los pines del puerto A como Salida
    TRISB = 0; // Declaro los pines del puerto B como Salida 
    TRISC = 0; // Declaro los pines del puerto B como Salida

    ADCON1 = 7; // Declaro el puerto A como Digital

    comando(12); // 1100, Display ON - Cursor OFF - Blink OFF

    mensajeCuatroBits("KONICHIWA!");
}
