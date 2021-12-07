#include "controle.h"

#include<time.h> //necessário p/ função time()
#include <stdlib.h>// necessário p/ as funções rand() e srand()

#include "keypad.h"
#include "lcd.h"
#include "bits.h"
#include "config.h"
#include "ssd.h"
#include "io.h"
#include "i2c.h"
#include "ds1307.h"
#include <pic18f4520.h>


#define DISP0 PIN_A2
#define DISP1 PIN_A3
#define DISP2 PIN_A4
#define DISP3 PIN_A5

char i;
int random = 0;
int saldo = 650;


void printf(char text[]) {
    char i = 0;
    while (text[i] != '\0') {
        lcdChar(text[i]);
        i++;
    }
}

void delay() {
    char i, j;
    for (i = 0; i < 255; i++) {
        for (j = 0; j < 255; j++);
        for (j = 0; j < 255; j++);
    }
}

void verso() {
    delay();
    unsigned char k, x;
    x = 0b10000001;
    for (k = 0; k < 8; k++) {
        delay();
        PORTD = x;
        x = x << 1;
    }
}

void inverso() {
    delay();
    unsigned char k, x;
    x = 0b10000000;
    for (k = 0; k < 8; k++) {
        delay();
        PORTD = x;
        x = x >> 1;
    }
}

void textInit() {
    char textCima[12] = "Welcome to";
    char textBaixo[17] = "THE GAME!";
    lcdCommand(0x80 | (0));
    printf(textCima);
    lcdCommand(0xC0 | (0));
    printf(textBaixo);
    //delay
    delay();
    delay();
    delay();
    delay();
    //limpa a tela
    lcdCommand(0x01);
}
// achei esse código na internet, não entendi muito bem como funciona

char* itoa(int value, char* result, int basea) {
    // check that the base if valid
    if (basea < 2 || basea > 36) {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= basea;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * basea)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

void apostaTxt() {
    char textCima[15] = "Voce acha que";
    char textBaixo[15] = "AGUENTA??!!";
    lcdCommand(0x80 | (0));
    printf(textCima);
    lcdCommand(0xC0 | (0));
    printf(textBaixo);
    //delay
    delay();
    delay();
    delay();
    delay();
    //limpa a tela
    lcdCommand(0x01);
}



void saldoTxt() {
    //limpa a tela
    lcdCommand(0x01);
    char textCima[7] = "Saldo: ";
    char textBaixo[10] = "";
    itoa(saldo, textBaixo, 10);
    lcdCommand(0x80 | (0));
    printf(textCima);
    lcdCommand(0xC0 | (0));
    printf(textBaixo);
    //delay
    delay();
    delay();
    delay();
    //limpa a tela
    lcdCommand(0x01);
}

void waitTxtProgress() {
    lcdCommand(0x01);
    char textCima[15] = "D4 esq Aposta";
    char textBaixo[15] = "D3 esq Saldo";
    lcdCommand(0x80 | (0));
    printf(textCima);
    lcdCommand(0xC0 | (0));
    printf(textBaixo);
    //delay
    delay();
    delay();
    delay();

}

void GameOver() {
    char textCima[15] = "GAME OVER";
    char textBaixo[20] = "YOU LOSE...";
    lcdCommand(0x80 | (0));
    printf(textCima);
    lcdCommand(0xC0 | (0));
    printf(textBaixo);
    //delay
    delay();
    delay();
    delay();
    lcdCommand(0x01);
    saldoTxt();
}

void Lose() {
    char textCima[15] = "YOU LOSE";
    char textBaixo[15] = "perdeu: -100";
    lcdCommand(0x80 | (0));
    printf(textCima);
    lcdCommand(0xC0 | (0));
    printf(textBaixo);
    //delay
    delay();
    delay();
    delay();
    lcdCommand(0x01);
    saldoTxt();
}

void WinAll() {
    char textCima[15] = "OMG YOU HIT IT";
    char textBaixo[15] = "OMG:+500!!! ";
    lcdCommand(0x80 | (0));
    printf(textCima);
    lcdCommand(0xC0 | (0));
    printf(textBaixo);
    //delay
    delay();
    delay();
    delay();
    lcdCommand(0x01);
    saldo += 500; //tem que ser a mais por que tem os -100 que foram tirandos na hora de começar
    saldoTxt();
}

void Win2x() {
    char textCima[21] = "HOLY MOLY YOU WIN 2X";
    char textBaixo[15] = "recebeu: +200 ";
    lcdCommand(0x80 | (0));
    printf(textCima);
    lcdCommand(0xC0 | (0));
    printf(textBaixo);
    //delay
    delay();
    delay();
    delay();
    lcdCommand(0x01);
    saldo += 300; //tem que ser a mais por que tem os -100 que foram tirandos na hora de começar
    saldoTxt();
}

void Win1x() {
    char textCima[15] = "OMG YOU WIN 1x";
    char textBaixo[15] = "recebeu: +100 ";
    lcdCommand(0x80 | (0));
    printf(textCima);
    lcdCommand(0xC0 | (0));
    printf(textBaixo);
    //delay
    delay();
    delay();
    delay();
    lcdCommand(0x01);
    saldo += 200; //tem que ser a mais por que tem os -100 que foram tirandos na hora de começar
    saldoTxt();
}

void Apostando() {
    srand(time(NULL)); // descobrir o porque a semente não esta funcionando 
    random = ((rand() % 10000) * getSeconds());
    random = random % 10000;
    random -= 1;
    char randomChar[4];
    itoa(random, randomChar, 10);
    for (float t = 0; t < 200; t++) {
        ssdDigit(random / 1000, 0);
        ssdDigit((random / 100) % 10, 1);
        ssdDigit((random / 10) % 10, 2);
        ssdDigit(random % 10, 3);
        ssdUpdate();
        for (float tempo = 0; tempo < 100; tempo++); // +- 10ms
    }

    digitalWrite(DISP0, 0);
    digitalWrite(DISP1, 0);
    digitalWrite(DISP2, 0);
    digitalWrite(DISP3, 0);
    //lógica, caso alguma das casas seja igual a outra do seu lado você ganha
    if ((randomChar[0] == randomChar[1]) || (randomChar[1] == randomChar[2]) || (randomChar[2] == randomChar[3])) {
        Win1x();
    }
    if ((randomChar[0] == randomChar[1] && randomChar[1] == randomChar[2]) || (randomChar[1] == randomChar[2])&& (randomChar[2] == randomChar[3])) {
        Win2x();
    }
    if ((randomChar[0] == randomChar[1]) && (randomChar[1] == randomChar[2]) & (randomChar[2] == randomChar[3])) {
        WinAll();
    }

    if ((randomChar[0] != randomChar[1]) && (randomChar[1] != randomChar[2]) & (randomChar[2] != randomChar[3])) {
        Lose();
    }
}

void apostandoTxtProgress() {
    if (saldo - 100 <= 0) {
        GameOver();
    } else {
        char text[15] = ":O ";
        char text2[15] = "Apostando: -100 ";
        lcdCommand(0x80 | (0));
        printf(text);
        lcdCommand(0xC0 | (0));
        printf(text2);
        //aposta
        saldo -= 100;
        TRISD = 0x00;
        PORTD = 0x00;
        verso();
        inverso();
        Apostando();
        lcdCommand(0x01);
    }
}