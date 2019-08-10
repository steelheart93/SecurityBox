/**
 * Programa que carga en un PIC un sistema de autenticación por clave.
 * 
 * @author Stiven Muñoz Murillo
 * @version 16/07/2019
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
#define CLAVE "1234"

// Variablas GLOBALES
int contador = 0;
int bandera = 1;

char tecla = 0;
char teclas[4];
char asteriscos[4];

/**
 * Pulso de lectura del LCD.
 */
void pulso() {
    RA0 = 0;
    RA0 = 1;
    __delay_ms(15);
    RA0 = 0;
}

/**
 * Lleva la información de visualización al LCD.
 * @param d, dato o carácter que se visualiza.
 */
void dato(int d) {
    PORTC = d;
    RA1 = 1;
    pulso();
}

/**
 * Lleva información de control al LCD.
 * @param c, comando o intrucción de control.
 */
void comando(int c) {
    PORTC = c;
    RA1 = 0;
    pulso();
}

void pintarAsteriscos(int cantidad) {
    for (int i = 0; i <= cantidad; i++) {
        comando(128 + i); // Set DD RAM address
        dato('*');
    }
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

/**
 * Muestra un mensaje de más de 16 caracteres.
 * @param mensaje
 */
void mensajeLargo(char mensaje[]) {
    //comando(1); // Clear display

    // Cargar el mensaje en la pantalla
    for (int i = 0; i < strlen(mensaje); i++) {
        comando(128 + i); // Set DD RAM address
        dato(mensaje[i]);
    }

    // Rota el mensaje por la pantalla
    //for (int i = 0; i < strlen(mensaje); i++) {
    comando(24); // 11000, Display shift - Shift to the left
    //__delay_ms(45);
    //}
}

/**
 * Verifico la tecla presionada por el usuario y la almaceno en "tecla"
 */
void comprobarTeclas() {
    // Compruebo la Fila 0
    RB0 = 0;
    if (RB4 == 0) {
        tecla = '7';
    } else if (RB5 == 0) {
        tecla = '8';
    } else if (RB6 == 0) {
        tecla = '9';
    }
    RB0 = 1;

    // Compruebo la Fila 1
    RB1 = 0;
    if (RB4 == 0) {
        tecla = '4';
    } else if (RB5 == 0) {
        tecla = '5';
    } else if (RB6 == 0) {
        tecla = '6';
    }
    RB1 = 1;

    // Compruebo la Fila 2
    RB2 = 0;
    if (RB4 == 0) {
        tecla = '1';
    } else if (RB5 == 0) {
        tecla = '2';
    } else if (RB6 == 0) {
        tecla = '3';
    }
    RB2 = 1;

    // Compruebo la Fila 3
    RB3 = 0;
    if (RB5 == 0) {
        tecla = '0';
    }
    RB3 = 1;
}

/**
 * Función que se ejecuta al inicio.
 */
void main(void) {
    nRBPU = 0; // Habilito las resistencias de Pull UP

    TRISA = 0; // Declaro los pines del puerto A como Salida
    TRISB = 0b11110000; // Declaro los pines de B como Salida y Entrada 
    TRISC = 0; // Declaro los pines del puerto B como Salida

    ADCON1 = 7; // Declaro el pueto A como Digital

    comando(12); // 1100, Display ON - Cursor OFF - Blink OFF

    while (1) {
        comprobarTeclas();

        // Si se presiono una tecla, deshabilito la Bienvenida y
        if (tecla != 0) {
            bandera = 0;

            // Limpio el LCD
            if (contador == 0) {
                comando(1);
            }

            // Muestro la Tecla
            teclas[contador] = tecla;
            // mensajeCorto(teclas);
            pintarAsteriscos(contador);

            // Incremento el Cursor
            contador++;

            // Libero la Tecla
            tecla = 0;
            __delay_ms(250);

            if (contador > 3) {
                if (strcmp(teclas, CLAVE) == 0) {
                    mensajeCorto("Clave CORRECTA");
                } else {
                    mensajeCorto("Clave ERRONEA");
                }

                // Habilito la Bienvenida
                bandera = 1;

                // Reinicio el Cursor
                contador = 0;
                __delay_ms(1000);

                // Limpio el Arreglo de Teclas
                teclas[0] = ' ';
                teclas[1] = ' ';
                teclas[2] = ' ';
                teclas[3] = ' ';
            }
        }

        // Si la Bienvenida está habilitada
        if (bandera) {
            mensajeLargo("Ingrese la Clave, si quiere!");
        }
    }
}
