; Escribir los numeros pares en la eeprom

org 0x0
; voy al banco 3
BSF 0x03, 6 ;status.rp1
BSF 0x03, 5 ;status.rp0
    
; miro si la wea esta lista para escritura
listoWR:
btfsc 0x18C, 1 ; eecon1.wr
; si no esta lista se procede a retroceder
goto listoWR

;me cambio al banco 2
BCF 0x03, 5 ;status.rp0
    
    
    
    ; procedere a realizar un for
    ; el registro 12F controlara el crecimiento 0,2,4,6,8...254
    movlw .0
    movwf 0x12F
    
    ; el registro 12E controla la pos de la eprom donde se escribe
    movlw .0
    movwf 0x12E
    
    ; Condicion del for i=0 to 10 do:
    ; El ciclo lo quiero hacer 7F veces por ello guardo 7f en 0x120
    movlw 0x7f
    movwf 0x120
    
    ; Pongo en 2 en 0x121
    movlw .2
    movwf 0x121
   
    ; creo una etiqueta para el ciclo
    etiqueta:
    ; Sumo el 2 que hay en aku en .33
    ;Cuerpo FOR
    
    ;me cambio al banco 2
    BCF 0x03, 5 ;status.rp0
    
    ; voy a poner el eecon1.wren = 1
    BSF 0x18C, 2
    
    ; vamos a escribir en lo que diga 0x12E
    movf 0x12E, 0 ; >> aku sabe donde voy a escribir
    ; esta es la direccion de la EEPROM donde voy a escribir
    movwf 0x10D ; EEADR >> ? // se va a escribir en .1 de la EEPROM
    
    ; voy a guardar lo que diga 0x12F
    movf 0x12F, 0
    ; yo voy a mover del aku a 0x10C
    movwf 0x10C ; EEDATA es ?
    
    ; voy a moverme al bank3
    BSF 0x03, 5 ;status.rp0
    
    ; voy a poner el EECON1.EEPGD = 0
    BCF 0x18C, 7 ; eecon1.eepgd
    
    ; voy a poner el EECON1.WREN = 1
    BSF 0x18C, 2
    
    ; voy a poner el intcon.gie = 0
    BCF 0x18B, 7
    
    ; obligatorio
    movlw 0x55
    movwf 0x18D
    movlw 0xAA
    movwf 0x18D
    ; obligatoria

    BSF 0x18C, 1 ; aki se escribe


    ; voy a poner el intcon.gie = 1
    BSF 0x18B, 7
    
    ; voy a poner el eecon1.wren = 0
    BCF 0x18C, 2
    
    ; aumenta lo que diga 0x12E
    incf 0x12E, 1
    
    ;Fin for 
    DECFSZ 0x120, 1
    goto etiqueta

    
    
end