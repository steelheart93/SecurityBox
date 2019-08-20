#include <xc.h>
#include <string.h>
#include <stdio.h>

#define _XTAL_FREQ 4000000


int cambio = 0;
//definicion segun el proyecto
char clave_generica[] = "1234"; //solo se usa para restaurar la caja
char Clave_por_defecto[] = "3333";
char Clave_nueva[] = "";
char Clave_ingresada[] = "";


int posicion_clave_nueva = 0;
int caja_Abierta = 0; //0 para la caja esta cerrada 1 caja abierta pero no se ha precionado el boton de config 2 se preciono A
int intentos = 0;
int claveCorrecta = 0;

//mensajes
char mensaje_ingreso_clave_de_seguridad[] = "ingrese Clave";
char mensaje_de_error[] = "error Clave";


int estado_o_tipo_procedimiento = 0; //0 para ingreso de contraseña 1 para configurar la clave por defecto
int rotartextoPantalla = 0;
int soloTrasmitirUnaVez = 0;

void pulsador() {
    RA4 = 0;
    RA4 = 1;
    __delay_ms(15);
    RA4 = 0;
}

void instrucion(int c) {
    PORTC = c;
    RA4 = 0;
    pulsador();
}

void EscribirPalabra(int d) {
    PORTC = d;
    RA5 = 1;
    pulsador();

}

/**
 *  escribe una palabra en el display
 */
void escribirMensaje(char mensaje[]) {
    instrucion(1);
    for (int i = 0; i < strlen(mensaje); i++) {
        EscribirPalabra(mensaje[i]);
    }
}

void vaciarVector() {

    memset(Clave_ingresada, 0, strlen(Clave_ingresada));
    posicion_clave_nueva = 0;
}

void limpiar() {//SE ENCARGA DE LIMPIAR EL LCD
    PORTC = 128;
    RA5 = 0;
    pulsador();


    PORTC = 1;
    RA4 = 0;
    pulsador();
}

void transmitirDato(int dato) {
    TXREG = dato;
    TXEN = 1;
    while (TXIF == 0) {
        ;
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

void transmitirMensaje(char mensaje[]) {
    for (int i = 0; i < strlen(mensaje); i++) {
        transmitirDato(mensaje[i]);
    }

    transmitirDato(10);
    transmitirDato(13);
}

void AnexarLetra(char element_Agregar) {

    //    if (posicion_clave_nueva < 4) {
    Clave_ingresada[posicion_clave_nueva] = element_Agregar;
    posicion_clave_nueva++;
    //    }

    limpiar();
    escribirMensaje(Clave_ingresada);

}

void escribirMensajeConTiempoEspera(char dato[]) {
    limpiar();
    escribirMensaje(dato);
    __delay_ms(120);
}

void compararClaves() {

    if (strcmp(Clave_ingresada, Clave_por_defecto) == 0) {

        escribirMensajeConTiempoEspera("clave Correta!!");
        escribirMensajeConTiempoEspera("puede abrir caja");
        claveCorrecta = 1;


    } else {

        escribirMensajeConTiempoEspera("clave incorrecta");
        escribirMensajeConTiempoEspera(mensaje_ingreso_clave_de_seguridad);
        intentos++;
    }

    //    Clave_ingresada[4]="";
    vaciarVector();

}

void cambioDeClave() {
    limpiar();
    escribirMensaje("cambiando Clave");
    __delay_ms(200);
    //aqui se hacen las validaciones la contrasena
    if (strlen(Clave_ingresada) > 3 && strlen(Clave_ingresada) <= 9) {

        //borrarEEprom
        int i;
        for (i = 0; i < 10; i++) {
            EEPROM_Write(i, 255);
        }

        for (int x = 0; x < strlen(Clave_ingresada); x++) {
            Clave_por_defecto[x] = Clave_ingresada[x];
        }

        limpiar();
        escribirMensaje("exito");
        estado_o_tipo_procedimiento = 0;
        intentos = 0;
        limpiar();
        escribirMensaje(mensaje_ingreso_clave_de_seguridad);
        __delay_ms(200);
    } else {
        limpiar();
        escribirMensaje("Error No valida");
    }



    vaciarVector();

}
//Metodo del loco
//Donde es la posicion de la EEPROM 
//dato es el dato que se va a meter

void meterloEnLaEEPROM(int donde, int dato) {
    escribirMensaje("Metiendo en la EEPROM");

    //Si esta listo para escribir
    if (EECON1bits.WR == 0) {

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

void configuracionDeClave() {

    limpiar();
    escribirMensaje("configurando");
    __delay_ms(200);
    limpiar();
    escribirMensaje("ingrese y tecla B");
    rotartextoPantalla = 1;
    estado_o_tipo_procedimiento = 1;



}

void __interrupt() interrupcion() {
    __delay_ms(100);


    int leer = PORTA;
    rotartextoPantalla = 0;

    INTF = 0;
    RBIF = 0;
    if (RB0 == 1) {
        intentos = 0;
        limpiar();
        soloTrasmitirUnaVez = 0;

        escribirMensaje("Desbloqueando");
        __delay_ms(200);
        limpiar();
        escribirMensaje(mensaje_ingreso_clave_de_seguridad);



    }



    //ahora para el teclado tenemos
    //leer el puerto A y el B y juntarlo todo

    // no escribe si la caja esta bloqueda
    if (intentos < 3) {


        int port = 0;
        if (RB4 == 0) {
            port += 224;
        } else if (RB5 == 0) {
            port += 208;
        } else if (RB6 == 0) {
            port += 176;
        } else if (RB7 == 0) {
            port += 112;
        }




        if (RA0 == 0) {
            port = port + 14;
        } else if (RA1 == 0) {
            port = port + 13;
        } else if (RA2 == 0) {
            port = port + 11;
        } else if (RA3 == 0) {
            port = port + 7;
        }
        switch (port) {
            case 231: //verifivamos que en el puerto b esta un 231  y sacamos un 0
                //            PORTC = 0;
                AnexarLetra('0');
                meterloEnLaEEPROM(0, 1);
                meterloEnLaEEPROM(1, 0);
                meterloEnLaEEPROM(2, 11);
                meterloEnLaEEPROM(3, 11);
                __delay_ms(1000);
                instrucion(1);


                break;

            case 123:
                if (caja_Abierta == 0 || caja_Abierta == 2) {


                    if (estado_o_tipo_procedimiento == 1) {
                        cambioDeClave();
                    } else {
                        compararClaves();
                    }
                }


                break;
            case 187:

                //para saber si se puede configurar la caja
                if (RB1 == 0) {
                    configuracionDeClave();
                    caja_Abierta = 2;
                }


                break;
            case 219:
                if (caja_Abierta == 0 || caja_Abierta == 2) {
                    AnexarLetra('9');
                }
                break;
            case 235: if (caja_Abierta == 0 || caja_Abierta == 2) {
                    AnexarLetra('8');
                }
                break;


            case 125: if (caja_Abierta == 0 || caja_Abierta == 2) {
                    AnexarLetra('7');
                }
                break;
            case 189: if (caja_Abierta == 0 || caja_Abierta == 2) {
                    AnexarLetra('6');
                }
                break;
            case 221: if (caja_Abierta == 0 || caja_Abierta == 2) {
                    AnexarLetra('5');
                }
                break;
            case 237: if (caja_Abierta == 0 || caja_Abierta == 2) {
                    AnexarLetra('4');
                }
                break;



            case 126: if (caja_Abierta == 0 || caja_Abierta == 2) {
                    AnexarLetra('3');
                }
                break;
            case 190: if (caja_Abierta == 0 || caja_Abierta == 2) {
                    AnexarLetra('2');
                }
                break;
            case 222: if (caja_Abierta == 0 || caja_Abierta == 2) {
                    AnexarLetra('1');
                }

                break;
            case 238:
                if (caja_Abierta == 0 || caja_Abierta == 2) {
                    AnexarLetra('0');
                }
                break;


        }


        //fin del teclado


    }//fin de bloqueo de caja

    GIE = 1;
}
int escribirsolounavezCerrada = 0;
int escribirsolounavezAbierta = 1;

void main(void) {
    //PRUEBAS PARA QUE ESCRIBA EN PANTALLA
    nRBPU = 0; // Habilito las resistencias de Pull UP
    TRISA = 0b00000000; // Declaro los pines del puerto A como Salida
    TRISB = 0b11111111; // Declaro los pines de B como Salida y Entrada 
    TRISC = 0; // Declaro los pines del puerto B como Salida

    ADCON1 = 7; // Declaro el puerto A como Digital

    instrucion(14); // 1100, Display ON - Cursor OFF - Blink OFF

    GIE = 1; //activamos la bandera global
    RBIE = 1; //activamos la interrucion
    RBIF = 0; //aseguramos que la bandera este abajo
    INTE = 1;
    INTF = 0;
    //   
    //fin de la configuracion de la interrrupcion





    SPEN = 0; //puto problema 
    //    EscribirPalabra(48);
    SYNC = 0;
    BRGH = 1;

    SPBRG = 26;

    GO = 1; //habilitaar lo de leer el puerto


    //char a entero le resto 48
    if (EEPROM_Read(0) >= 255) {
        int i;
        for (i = 0; i < strlen(Clave_por_defecto); i++) {
            EEPROM_Write(i, Clave_por_defecto[i] - 48);
        }
    } else {
        int t;
        for (t = 0; t < 8; t++) {
            if (EEPROM_Read(t) != 255) {
                Clave_por_defecto[t] = EEPROM_Read(t) + 45;
            }


        }
    }
        EscribirPalabra(EEPROM_Read(0) + 45);
        escribirMensajeConTiempoEspera(Clave_por_defecto);





        //   EEPROM_Write(1, 2);
        //   EscribirPalabra(EEPROM_Read(1));
        //   if (EEPROM_Read(2)==255) {
        //       escribirMensajeConTiempoEspera("vacio");
        //    }

        __delay_ms(120);
        //esribimos en la pantalla el mensaje de ingreso de clave de segurida
        escribirMensaje(mensaje_ingreso_clave_de_seguridad);
        SPEN = 1;
        transmitirMensaje("Proyecto Microprocesadores");
        SPEN = 0;

        while (1) {
            if (rotartextoPantalla == 1) {
                instrucion(24); //rota el display
                __delay_ms(50);

            }

            if (intentos >= 3) {
                if (soloTrasmitirUnaVez == 0) {
                    SPEN = 1;
                    transmitirMensaje("Error por Favor ingrese LLave");
                    SPEN = 0;
                    limpiar();
                    escribirMensaje("Boqueada!!");
                    soloTrasmitirUnaVez = 1;
                }


            }

            //se hace para hacer las verificaciones a la hora de abrir la caja
            if (RB1 == 1) {
                if (escribirsolounavezCerrada == 0) {
                    escribirMensajeConTiempoEspera("cajaCerrada");
                    escribirMensajeConTiempoEspera(mensaje_ingreso_clave_de_seguridad);
                    escribirsolounavezCerrada = 1;
                    escribirsolounavezAbierta = 0;
                    caja_Abierta = 0;
                }
            } else {
                if (escribirsolounavezAbierta == 0 && claveCorrecta == 1) {
                    escribirMensajeConTiempoEspera("cajaAbierta");
                    //escribirMensajeConTiempoEspera(mensaje_ingreso_clave_de_seguridad);
                    escribirsolounavezAbierta = 1;
                    escribirsolounavezCerrada = 0;
                    caja_Abierta = 1;
                    intentos = 0;
                }
            }




            PORTA = 14;

            PORTA = 13;

            PORTA = 11;
            PORTA = 7;
            //fin de rotar el numero
        }

    }