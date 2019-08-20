/**
 * Proyecto final de Microprocesadores.
 * 
 * @author Stiven Muñoz Murillo
 * @version 19/08/2019
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
#include <stdio.h>
#include <string.h>
#define _XTAL_FREQ 4000000

int contador = 0;
char teclas[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

/**
 * Envia un Pulso al LCD.
 */
void enviarPulso() {
    RB3 = 0;
    RB3 = 1;
    __delay_ms(15);
    RB3 = 0;
}

/**
 * Envia un Comando al LCD.
 * 
 * @param comando
 */
void enviarComando(int comando) {
    PORTC = comando;
    RB2 = 0;
    enviarPulso();
}

/**
 * Envia un Dato para escribir en el LCD.
 * 
 * @param dato
 */
void enviarDato(int dato) {
    PORTC = dato;
    RB2 = 1;
    enviarPulso();
}

/**
 * Función que pinta asteriscos.
 * 
 * @param cantidad
 */
void pintarAsteriscos(int cantidad) {
    for (int i = 0; i <= cantidad; i++) {
        enviarComando(128 + i); // Set DD RAM address
        enviarDato('*');
    }
}

/**
 * Muestra un mensaje de menos de 16 caracteres.
 * 
 * @param mensaje
 */
void mensajeCorto(char mensaje[]) {
    for (int i = 0; i < strlen(mensaje); i++) {
        enviarComando(128 + i); // Set DD RAM address
        enviarDato(mensaje[i]);
    }
}

/**
 * Función que escribe en la EEPROM.
 * 
 * @param Address, dirección del bloque de memoria donde se va a escribir.
 * @param Data, caracter que se va escribir en dicho bloque de memoria.
 */
void EEPROM_Write(int Address, char Data) {
    while (EECON1bits.WR); // Waits Until Last Attempt To Write Is Finished
    EEADR = Address; // Writes The Addres To Which We'll Wite Our Data
    EEDATA = Data; // Write The Data To Be Saved
    EECON1bits.EEPGD = 0; // Cleared To Point To EEPROM Not The Program Memory
    EECON1bits.WREN = 1; // Enable The Operation !
    INTCONbits.GIE = 0; // Disable All Interrupts Untill Writting Data Is Done
    EECON2 = 0x55; // Part Of Writing Mechanism..
    EECON2 = 0xAA; // Part Of Writing Mechanism..
    EECON1bits.WR = 1; // Part Of Writing Mechanism..
    INTCONbits.GIE = 1; // Re-Enable Interrupts
    EECON1bits.WREN = 0; // Disable The Operation
    EECON1bits.WR = 0; // Ready For Next Writting Operation
}

/**
 * Función que lee de la EEPROM.
 * 
 * @param Address, dirección del bloque de memoria de donde voy a leer.
 * @return Contenido de dicho bloque de memoria.
 */
int EEPROM_Read(int Address) {
    int Data;
    EEADR = Address; // Write The Address From Which We Wonna Get Data
    EECON1bits.EEPGD = 0; // Cleared To Point To EEPROM Not The Program Memory
    EECON1bits.RD = 1; // Start The Read Operation
    Data = EEDATA; // Read The Data
    return Data;
}

void __interrupt() listener() {
    int leer = PORTA;
    leer = PORTB;

    INTF = 0; // Bandera de RB0
    RBIF = 0; // Bandera del Puerto B

    if (RB0 == 1) {
        contador = 0;
        mensajeCorto("Boton!!!");
        __delay_ms(1000);
        enviarComando(1);
    }

    if (RB7 == 0 && RA5 == 0) {
        mensajeCorto("Switch!!!");
        __delay_ms(500);
        enviarComando(1);
    }

    if (RA0 == 0) {
        if (RB4 == 0) {
            teclas[contador] = '7';
            pintarAsteriscos(contador);
            contador++;
            __delay_ms(1000);
        } else if (RB5 == 0) {
            enviarComando(128 + contador);
            contador++;
            enviarDato('8');
        } else if (RB6 == 0) {
            enviarComando(128 + contador);
            contador++;
            enviarDato('9');
        }
    } else if (RA1 == 0) {
        if (RB4 == 0) {
            enviarComando(128 + contador);
            contador++;
            enviarDato('4');
        } else if (RB5 == 0) {
            enviarComando(128 + contador);
            contador++;
            enviarDato('5');
        } else if (RB6 == 0) {
            enviarComando(128 + contador);
            contador++;
            enviarDato('6');
        }
    } else if (RA2 == 0) {
        if (RB4 == 0) {
            enviarComando(128 + contador);
            contador++;
            enviarDato('1');
        } else if (RB5 == 0) {
            enviarComando(128 + contador);
            contador++;
            enviarDato('2');
        } else if (RB6 == 0) {
            enviarComando(128 + contador);
            contador++;
            enviarDato('3');
        }
    } else if (RA3 == 0) {
        if (RB4 == 0) {
            enviarComando(128 + contador);
            contador++;
            int num = EEPROM_Read(contador);
            // enviarDato('A');
            enviarDato(num);
        } else if (RB5 == 0) {
            enviarComando(128 + contador);
            contador++;
            enviarDato('0');
        } else if (RB6 == 0) {
            /*
            char clave[contador];
            for (int i = 0; i < contador; i++) {
                clave[i] = EEPROM_Read(i);
            }

            if (strcmp(teclas, clave) == 0) {
                mensajeCorto("Clave CORRECTA");
            } else {
                mensajeCorto(clave);
            }*/
            //enviarComando(128 + contador);
            //contador++;
            //enviarDato('B');
        }
    }

    GIE = 1; // Habilitador Global
}

/**
 * Función principal del PIC.
 */
void main() {
    ADCON1 = 7; // Declaro el puerto A como Digital

    TRISA = 0; // Declaro los pines del puerto A como Salida
    TRISB = 241; // 11110001, Declaro los pines de B como Salida y Entrada 
    TRISC = 0; // Declaro los pines del puerto C como Salida

    enviarComando(12); // 1100, Display ON - Cursor OFF - Blink OFF

    nRBPU = 0; // Habilito las resistencias de Pull UP

    GIE = 1; // Habilitador Global
    INTF = 0; // Bandera de RB0
    RBIF = 0; // Bandera del Puerto B
    INTE = 1; // Habilitador de RB0
    RBIE = 1; // Habilitador del Puerto B

    EEPROM_Write(0, '7');
    EEPROM_Write(1, '7');
    EEPROM_Write(2, '7');
    EEPROM_Write(3, '7');

    while (1) {
        PORTA = 14; // 001110
        __delay_ms(100);
        PORTA = 45; // 101101
        __delay_ms(100);
        PORTA = 11; // 001011
        __delay_ms(100);
        PORTA = 39; // 100111
        __delay_ms(100);
    };
}