/**
 * Programa que carga en un PIC un sistema de autenticación por clave.
 * 
 * @author Stiven Muñoz Murillo
 * @version 16/07/2019
 * 
 * 
 * @autot: el loko
 * @version 16/07/2019
 * Nota: El loco se propone a crear un metodo que pueda leer y escribir de la 
 * EEPROM
 * 
 * 
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



//Metodo del loco
//Donde es la posicion de la EEPROM 
//dato es el dato que se va a meter
void meterloEnLaEEPROM(int donde, int dato){
    mensajeCorto("Metiendo en la EEPROM");
    
    //Si esta listo para escribir
    if(EECON1bits.WR == 0){
        
        EEADR = donde; // posicion en la que voy a escribir
        EEDATA = dato; // que le voy a escribir
        
        //Habilito para escritura
        EECON1bits.EEPGD = 0;
        EECON1bits.WREN = 1;
        
        //Paso obligatorio
        GIE = 0; //desactivo las interrupciones
        EECON2 = 85; //alisto motores para escritura
        EECON2 = 170; 
        EECON1bits.WR = 1; //Le digo que estoy escribiendo
        
        //Habilito interrupciones
        GIE = 1;
        //Desabilito la escritura
        EECON1bits.WREN = 0;
        
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
    
    if (RB1 == 1) {
        contador = 0;
        mensajeCorto("Si!!!");
        __delay_ms(1000);
        comando(1);
    } else {
        contador = 0;
        meterloEnLaEEPROM(0,1);
        meterloEnLaEEPROM(1,0);
        meterloEnLaEEPROM(2,11);
        meterloEnLaEEPROM(3,11);
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
    TRISB = 0b11110011; // Declaro los pines de B como Salida y Entrada 
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