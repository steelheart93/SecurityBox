;Almacenar en la EEPROM los numero 2n [2,254]
;luego leer una posicion usando los 7 bits menos significativos del puerto C
    
    
org 0x0
    
    ; los numeros pares se van a guardar en 0x120: 2,4,6,8 ... 254
    ; la dir donde se guarda en la eeprom se guarda en 0x121 h
    ;voy al banco 2
    BSF 0x03, 6 ;status.rp1
    BCF 0x03, 5 ;status.rp0
    movlw .2
    movwf 0x120 ; aka se guardan los pares
    movlw .0
    movwf 0x121 ; aka se guardo la dir de la eeprom 
   
    ; Esto se tiene que hacer 7F veces por ello 0x12F controlara eso
    movlw 0x80
    movwf 0x12f
    
    
    ; Condicion del for i=0 to 7f do:
    ; El ciclo lo quiero hacer 7f veces por ello guarde en 12f
    
    ; creo una etiqueta para el ciclo
    etiqueta:
    ;Cuerpo FOR
    
    
	    ; voy al banco 3
	    BSF 0x03, 6 ;status.rp1
	    BSF 0x03, 5 ;status.rp0

	    ; 2 - miro si la wea esta lista para escritura
	    listoWR:
	    btfsc 0x18C, 1 ; eecon1.wr
	    ; si no esta lista se procede a retroceder
	    goto listoWR
    
	    ; 3 - voy al banco 2
	    BSF 0x03, 6 ;status.rp1
	    BCF 0x03, 5 ;status.rp0
   
	    ; 6 - le paso la dir eeprom y el dato que quiero escribir
	    movf 0x121, 0 ; cargo la dir que voy a escribir
	    movwf 0x10D ; ya se en cual dir voy a escribir
	    movf 0x120, 0 ; cargo el dato que que voy a escribir
	    movwf 0x10C
    
	    ; 7 - voy al banco 3
	    BSF 0x03, 6 ;status.rp1
	    BSF 0x03, 5 ;status.rp0
    
	    ; 8 - activo la escritura
	    BCF 0x18C, 7
	    BSF 0x18C, 2
    
	    ; ------------------------
	    ; Paso obligatorio
	    ; Desactivar interrupciones
	    ; voy a poner el intcon.gie = 0
	    BCF 0x18B, 7
	    ; obligatorio
	    movlw 0x55
	    movwf 0x18D
	    movlw 0xAA
	    movwf 0x18D
	    ; obligatoria
    
	    ; Le digo que escriba
	    BSF 0x18C, 1 ; aki se escribe

	    ; voy a poner el intcon.gie = 1
	    BSF 0x18B, 7

	    ; voy a poner el eecon1.wren = 0
	    BCF 0x18C, 2

    
    ;voy al banco 2
    BSF 0x03, 6 ;status.rp1
    BCF 0x03, 5 ;status.rp0
    ; Aumenta la pos eeprom
    incf 0x121, 1
    ; Aumenta el Nro par
    incf 0x120, 1
    incf 0x120, 1
    ;Fin for 
    DECFSZ 0x12f, 1
    goto etiqueta
    
    ;En estos momentos la eeprom contiene los pares
    ;Procedo a leer por los 7 bits mas significativos del puertoC
    ;Procedo a entregar el resultado por el puertoB
    
    
    ;voy al banco0
    BCF 0x03, 6 ;status.rp1
    BCF 0x03, 5 ;status.rp0
    ; ---- voy a decirle a trisb .0
    movlw 0x86
    movwf 0x04 ; fsr >> trisb
    movlw .0
    movwf 0x0 ; portb salida
    
    ;Procedo a configurar el puerto C como entrada
    movlw 0x87
    movwf 0x04 ; fsr >> trisc
    movlw .255 ; pues a mi que me importa el bit que sobro
    movwf 0x0 ; portc entrada
    
    ; Procedo a limpiar el puertoB
    movlw .0
    movwf 0x06
    
    capturar:
    ; voy al banco0 xq ahi esta portC
    BCF 0x03, 6 ;status.rp1
    BCF 0x03, 5 ;status.rp0
    ; voy a capturar lo que diga el puertoC lo guardo en aku
    movf 0x07, 0
    ; voy al banco2
    BSF 0x03, 6 ;status.rp1
    BCF 0x03, 5 ;status.rp0
    ; el registro 0x130 me guardara lo que diga portC
    movwf 0x130 ; esta es la dir de la eeprom que voy a leer
    ; Procedo a leer la EEPROM
    ; voy a leer lo que diga 0x130
    movf 0x130, 0 ; aqui se sabe la dir a leer
    ; le mando la pos de lectura a EEADR
    movwf 0x10D
    ; voy al banco3
    BSF 0x03, 6 ;status.rp1
    BSF 0x03, 5 ;status.rp0
    ; EECON1.EEPGD = 0
    BCF 0x18C, 7 ; sabra Dios que es eso
    ; EECON1.RD = 1
    BSF 0x18C, 0 ; esta wea como que lee
    ; voy al banco2
    BSF 0x03, 6 ;status.rp1
    BCF 0x03, 5 ;status.rp0
    ; En el banco2 en EEDATA esta ya el dato que quiero
    movf 0x10C, 0
    ; voy al banco0
    BCF 0x03, 6 ;status.rp1
    BCF 0x03, 5 ;status.rp0
    movwf 0x06
    goto capturar
    
end