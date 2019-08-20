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
#include <string.h>
#define _XTAL_FREQ 4000000

int contador = 0;
int contadorErrores = 0;
int banderaNueva = 0;
char teclas[] = {' ', ' ', ' ', ' '};

void transmitirDato(int dato) {
    TXREG = dato;
    TXEN = 1;
    while (TXIF == 0) {
        ;
    }
}

void transmitirMensaje(char mensaje[]) {
    for (int i = 0; i < strlen(mensaje); i++) {
        transmitirDato(mensaje[i]);
    }

    transmitirDato(10);
    transmitirDato(13);
}

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
        enviarComando(1);
        contador = 0;
        contadorErrores = 0;
        mensajeCorto("Unlock");
        SPEN = 1; // Habilitador del puerto serial
        transmitirMensaje(" SecurityBox Unlock ");
        SPEN = 0; // Habilitador del puerto serial
        __delay_ms(1000);
        enviarComando(1);
    }

    if (RB7 == 0 && RA5 == 0) {
        mensajeCorto("Ingrese la CLAVE");
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
            teclas[contador] = '8';
            pintarAsteriscos(contador);
            contador++;
            __delay_ms(1000);
        } else if (RB6 == 0) {
            teclas[contador] = '9';
            pintarAsteriscos(contador);
            contador++;
            __delay_ms(1000);
        }
    } else if (RA1 == 0) {
        if (RB4 == 0) {
            teclas[contador] = '4';
            pintarAsteriscos(contador);
            contador++;
            __delay_ms(1000);
        } else if (RB5 == 0) {
            teclas[contador] = '5';
            pintarAsteriscos(contador);
            contador++;
            __delay_ms(1000);
        } else if (RB6 == 0) {
            teclas[contador] = '6';
            pintarAsteriscos(contador);
            contador++;
            __delay_ms(1000);
        }
    } else if (RA2 == 0) {
        if (RB4 == 0) {
            teclas[contador] = '1';
            pintarAsteriscos(contador);
            contador++;
            __delay_ms(1000);
        } else if (RB5 == 0) {
            teclas[contador] = '2';
            pintarAsteriscos(contador);
            contador++;
            __delay_ms(1000);
        } else if (RB6 == 0) {
            teclas[contador] = '3';
            pintarAsteriscos(contador);
            contador++;
            __delay_ms(1000);
        }
    } else if (RA3 == 0) {
        if (RB4 == 0) {
            mensajeCorto("Digite la nueva");
            banderaNueva = 1;
            __delay_ms(3000);
            enviarComando(1);
        } else if (RB5 == 0) {
            teclas[contador] = '7';
            pintarAsteriscos(contador);
            contador++;
            __delay_ms(1000);
        } else if (RB6 == 0) {
            if (banderaNueva == 1) {
                if (contador == 4) {
                    mensajeCorto("Cambiando CLAVE");
                    EEPROM_Write(0, teclas[0]);
                    EEPROM_Write(1, teclas[1]);
                    EEPROM_Write(2, teclas[2]);
                    EEPROM_Write(3, teclas[3]);
                    __delay_ms(5000);
                    enviarComando(1);
                } else {
                    mensajeCorto("Longitud ERRONEA");
                    __delay_ms(3000);
                    enviarComando(1);
                }

                banderaNueva = 0;
            } else {
                // Compruebo si la clave ingresada corresponde
                int i = 0;
                int bandera = 1;
                while (bandera && i < 4) {
                    bandera = (teclas[i] == EEPROM_Read(i));
                    i++;
                }

                // Indico si la clave ingresada fue la correcta
                if (bandera == 1 && i == 4) {
                    mensajeCorto("Clave CORRECTA");
                    __delay_ms(5000);
                    enviarComando(1);
                } else {
                    contadorErrores++;
                    mensajeCorto("Clave ERRONEA");
                    __delay_ms(5000);
                    enviarComando(1);
                }
            }

            teclas[0] = ' ';
            teclas[1] = ' ';
            teclas[2] = ' ';
            teclas[3] = ' ';

            contador = 0;
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

    SYNC = 0; // Sincronia-1, Asincronia-0
    BRGH = 1; // Velocidad de Transmision: 0-Alta, 1-Baja
    SPBRG = 25; // Bits de configuración del puerto Serial

    if (EEPROM_Read(0) == 255 || EEPROM_Read(3) == 255) {
        mensajeCorto("EEPROM vacia");
        __delay_ms(3000);
        enviarComando(1);

        mensajeCorto("Escribiendo...");
        EEPROM_Write(0, '1');
        EEPROM_Write(1, '2');
        EEPROM_Write(2, '3');
        EEPROM_Write(3, '4');
        __delay_ms(2000);
        enviarComando(1);
    } else {
        mensajeCorto("EEPROM con clave");
        __delay_ms(3000);
        enviarComando(1);

        mensajeCorto("Leyendo...");
        __delay_ms(2000);
        enviarComando(1);
    }

    while (1) {
        PORTA = 14; // 001110
        __delay_ms(100);
        PORTA = 45; // 101101
        __delay_ms(100);
        PORTA = 11; // 001011
        __delay_ms(100);
        PORTA = 39; // 100111
        __delay_ms(100);

        while (contadorErrores >= 3) {
            mensajeCorto("SecurityBox Lock");
            SPEN = 1; // Habilitador del puerto serial
            transmitirMensaje(" SecurityBox Lock ");
            SPEN = 0; // Habilitador del puerto serial
        }
    };
}