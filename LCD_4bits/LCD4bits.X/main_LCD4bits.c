/*
 * File:   main.c
 * Author: user
 *
 * Created on 4 de Abril de 2020, 12:37
 */

#include <xc.h>
#include <stdio.h>
#include <string.h>

#pragma config FOSC = INTOSC_HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000
#define time 10
//LCD
#define CD 0x01
#define RH 0x02
#define EMS 0x06
#define DC 0x0F
#define DSr 0x1C
#define DSl 0x18
#define FS 0x28
#define RAW1 0x80 //0b1000 0000
#define RAW2 0xC0 //0b1100 0000
#define E LATE0
#define RS LATE1
#define button RB2

void settings(void);
//LCD
void SettingsLCD(unsigned char word);
void LCD(unsigned char data);
void WriteLCD(unsigned char word);
void DisplayShiftRight(void);
void DisplayShiftLeft(void);
void __interrupt() LCD_int(void);
void PrintDataLCD(void);

unsigned char name[8]={'F','A','B','I','A','N','-','B'};
int i;

void main(void) {
    settings();
    while(1){
        
    }
}

void settings(void){
    OSCCON = 0x72;
    ADCON1 = 0x0F;
    TRISD = 0;
    TRISE = 0;
    LATD = 0;
    LATE = 0;
    //LCD
    SettingsLCD(0x02); //Iniciar la LCD con el método nibble (4 MSB y 4 LSB)
    SettingsLCD(EMS);
    SettingsLCD(DC);
    SettingsLCD(FS);
    SettingsLCD(CD);
    //Interrupción externa 2
    GIE = 1;
    INT2IE = 1;
    INT2IF = 0;
    INTEDG2 = 0;
}

void SettingsLCD(unsigned char word){
    RS = 0;
    LCD(word >> 4); // 4 MSB
    LCD(word & 0x0F); // 4 LSB
}

void WriteLCD(unsigned char word){
    RS = 1;
    LCD(word >> 4);
    LCD(word & 0x0F);
}

void LCD(unsigned char data){ //Opción bits
    
    LATDbits.LATD0 = (data & 0x01);
    __delay_ms(time);
    LATDbits.LATD1 = (data & 0x02) >> 1;
    __delay_ms(time);
    LATDbits.LATD2 = (data & 0x04) >> 2;
    __delay_ms(time);
    LATDbits.LATD3 = (data & 0x08) >> 3;
    __delay_ms(time);
    E = 1;
    __delay_ms(time);
    E = 0;
    __delay_ms(time);
}

void __interrupt() LCD_int(void){
    if(INT2IF == 1){
        PrintDataLCD();
        DisplayShiftRight();
        DisplayShiftLeft();
        INT2IF = 0;
    }
}

void PrintDataLCD(void){
    SettingsLCD(RAW1+4);
    for(i=0; i<=strlen(name); i++){
        WriteLCD(name[i]);
        __delay_ms(time*10);
    }
}

void DisplayShiftRight(void){
    for(i=0; i<=3; i++){
        SettingsLCD(DSr);
        __delay_ms(time*10);
    }
}

void DisplayShiftLeft(void){
    for(i=0; i<=3; i++){
        SettingsLCD(DSl);
        __delay_ms(time*10);
    }
}