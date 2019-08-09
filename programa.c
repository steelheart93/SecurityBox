/*
 * File:   ConversorAnalogoDigital.c
 * 
 * leer LA TEMPERATURA CON LM35 + CONVERCION ES 10MV/Cª 
 * Author: sebastian
 *
 * Created on 31 de julio de 2019, 16:40
 * 0 ES SALIDA EN EL TRISB
 */


#include <xc.h>
#include <string.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000

void trasmitir(char palabra[]) {
    for (int i = 0; i < strlen(palabra); i++) {
        TXREG = palabra[i]; //DATO QUE SE ENVIA
        TXEN = 1;
        while (TXIF == 0) {
            ;
        }
    }
    TXREG = 10; //DATO QUE SE ENVIA
    TXEN = 1;
    while (TXIF == 0) {
        ;
    }
    TXREG = 13; //DATO QUE SE ENVIA
    TXEN = 1;
    while (TXIF == 0) {
        ;
    }



}

void dato(int dato) {

    TXREG = dato ; //DATO QUE SE ENVIA
    TXEN = 1;
    while (TXIF == 0) {
        ;
    }



}
int datoAnterior = 0;
int milivoltios=5000;
void main(void) {

    //PARA TENER EN CUENTA ES LA CONFIGURACION DE ADDCON1 Y ADDCON0
    TRISA = 1;
    //PARA ADDCON0
    CHS0 = 0; //PARA PONERLOS EN EL CANAL POR EL CUAL VA A TRASMITIR
    CHS1 = 0;
    CHS2 = 0;

    ADON = 1; //ES EL HABILITADOR 

    //JUSTIFICANDO PARA EL ADDCON1 PARA QUE QUEDE JUSTIFICANDO A LA DERECHA ADRESH-ADRES
    ADFM = 1; //1 A LA DERECHA;0 (CERO) A LA IZQUIERA 

    //FRECUENCIA DE MUESTREO TODOS EN 000 PARA QUE LA FRECUENCIA QUEDE ALTA
    ADCS0 = 0;
    ADCS1 = 0;
    ADCS2 = 0;
    //PARA PONER LOS VALORES DEL PUERTO A EN ANALOGOS
    PCFG0 = 0;
    PCFG1 = 0;
    PCFG2 = 0;
    PCFG3 = 0;


    //FIN









    TRISC = 128;

    SPEN = 1;
    SYNC = 0;
    BRGH = 1;
    SPBRG = 26;

    GO = 1; //habilitaar lo de leer el puerto

    while (1) {
        if (GO == 0) {
            int adresh = ADRESH * 256;
            int adresl = ADRESL + adresh;
            //int grados = (int) (adresl * 0.49);
            
            
            double graditos=ADRESL + ADRESH*256;//de (1023-255)/3=256
            int temperatura=(int)(graditos*0.4887585532);
            char palabra [10];

            
                sprintf(palabra, "%i",temperatura);
                //sprintf(palabra, "%i",grados);
                strcat(palabra, "C");
               
                trasmitir(palabra);
     

            
            GO = 1;
        }

    }


    int tr = ADRESH;
    trasmitir("Hola mundo");
    dato(100);




}

