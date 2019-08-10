/**
 * Conversor Analogo/Digital.
 * 
 * @author Stiven Muñoz Murillo
 * @version 30/07/2019
 */

#include <xc.h>
#include <stdio.h>
#include <string.h>
#define _XTAL_FREQ 4000000

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

void main(void) {
    TRISA = 1;
    TRISC = 128;

    SPEN = 1;
    SYNC = 0;
    BRGH = 1;
    SPBRG = 25; // Bits de configuración del puerto Serial.

    ADON = 1; // Habilitar ADCON0

    CHS0 = 0;
    CHS1 = 0;
    CHS2 = 0; // Configurar el bit RA0 para leer el sensor

    ADCS0 = 0;
    ADCS1 = 0;
    ADCS2 = 0; // Velocidad de Muestreo en FOSC/2

    ADFM = 1; // Justificamos a la derecha porque necesito 10 bits no 16

    PCFG0 = 0;
    PCFG1 = 0;
    PCFG2 = 0;
    PCFG3 = 0; // Configurar el puerto A como Analogo

    GO = 1; // Bandera

    while (1) {
        if (GO == 0) {
            int adresh = ADRESH * 256; // Obtengo el valor entero en el ADRESH
            int adresl = ADRESL + adresh; // Se lo sumo al ADRESL

            int temperatura = (int) (adresl * 0.49); // 500/1023 = 0.49

            char palabra[8];
            sprintf(palabra, "%i", temperatura);
            strcat(palabra, "C");

            transmitirMensaje(palabra);

            GO = 1;
        }
    }

}