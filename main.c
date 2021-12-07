/*
 * File:   main.c
 * Escritor: Gustavo Siqueira Marquardt
 * Matricula:2020027785
 *Curso:ECO
 * Criado em : 16/11/2021 14:30
 */



#include "controle.h"
#include "keypad.h"
#include "lcd.h"
#include "bits.h"
#include "config.h"
#include "ssd.h"
#include "io.h"
#include <pic18f4520.h>
#include "i2c.h"
#include "ds1307.h"
// precisa dessas 2 aqui para fazer o srand(time(NUL)))
#include<time.h> 
#include <stdlib.h>




void main(void) {
    unsigned int tecla = 16;
    //configuração do sistema
    srand(time(NULL));
    ssdInit();
    lcdInit();
    kpInit();
    lcdCommand(0x0F);
    textInit();
    waitTxtProgress();
    for (;;) {
        kpDebounce();
        if (kpRead() != tecla) {
            tecla = kpRead();
            if (bitTst(tecla, 1)) {//  BOTÃO ESQUERDO D4 
                lcdCommand(0x01);
                apostandoTxtProgress();
                waitTxtProgress();
            }
            if (bitTst(tecla, 0)) { //BOTÃO NA ESQUERDA D3
                lcdCommand(0x01);
                saldoTxt();
                waitTxtProgress();
            }
            if (bitTst(tecla, 2)) { //BOTÃO ESQUERDO D5
                lcdCommand(0x01);
            }

        }

    }
}