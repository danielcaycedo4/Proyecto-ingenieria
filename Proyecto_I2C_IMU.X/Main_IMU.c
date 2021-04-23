/*
 * File:   Main_Lectura_IMU_I2C.c
 * Author: Daniel Caycedo
 *
 * Created on 28 de febrero de 2020, 03:17 PM
 */


#include <xc.h>
#include <string.h>
#include <stdio.h>

#pragma config FOSC = INTOSC_EC
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000
#define time 100

//IMU
#define Adress_IMU 0xD0 //0xD0
// Escalas do girôscopio
#define    GYRO_FULL_SCALE_250_DPS    0x00 // SCALE_250 (°/s) = 0 (0x00 = 000|00|000)
#define    GYRO_FULL_SCALE_500_DPS    0x08 // SCALE_500 (°/s) = 1 (0x08 = 000|01|000)
#define    GYRO_FULL_SCALE_1000_DPS   0x10 // SCALE_1000 (°/s) = 2 (0x10 = 000|10|000)
#define    GYRO_FULL_SCALE_2000_DPS   0x18 // SCALE_2000 (°/s) = 3 (0x18 = 000|11|000)
// Escalas do acelerômetro
#define    ACC_FULL_SCALE_2_G        0x00 // SCALE_2_G (g) = 0 (0x00 = 000|00|000)
#define    ACC_FULL_SCALE_4_G        0x08 // SCALE_4_G (g) = 1 (0x08 = 000|01|000)
#define    ACC_FULL_SCALE_8_G        0x10 // SCALE_8_G (g) = 2 (0x10 = 000|10|000)
#define    ACC_FULL_SCALE_16_G       0x18 // SCALE_16_G (g) = 3 (0x18 = 000|11|000)
// Escalas de conversao (As taxas de conversão são especificadas na documentação)
#define SENSITIVITY_ACCEL     2.0/32768.0             // Valor de conversão do Acelerômetro (g/LSB) para 2g e 16 bits de comprimento da palavra
#define SENSITIVITY_GYRO      250.0/32768.0           // Valor de conversão do Girôscopio ((°/s)/LSB) para 250 °/s e 16 bits de comprimento da palavra
#define SENSITIVITY_TEMP      333.87                  // Valor de sensitividade do Termometro (Datasheet: MPU-9250 Product Specification, pag. 12)
#define TEMP_OFFSET           21                      // Valor de offset do Termometro (Datasheet: MPU-9250 Product Specification, pag. 12)
#define SENSITIVITY_MAGN      (10.0*4800.0)/32768.0   // Valor de conversão do Magnetômetro (mG/LSB) para 4800uT, 16 bits de comprimento da palavra e conversao a Gauss (10mG = 1uT)

void settings(void);
void writeI2C(unsigned char Address,unsigned char Register,unsigned char *Data,int bytes);//*=puntero(direccion de memoria)
void writeI2C1(unsigned char Address,unsigned char Register,unsigned char Data);//*=puntero(direccion de memoria)
void startI2C(void);//empieza la comunicación
void readyI2C(void);//evita colicion de datos
void sendI2C(unsigned char data);
void stopI2C(void);
void readI2C(unsigned char Address,unsigned char Register,unsigned char *Data,int bytes);
void repeatedStart(void);
void start(void);
unsigned char receiveI2C(char flag);

//variables
unsigned char newMagData = 0;
// Offsets de calibração
int i = 0, offset_samples = 100;
double offset_accelx = 996.0, offset_accely = 1610.0, offset_accelz = 17158.0;
double offset_gyrox = -86.00, offset_gyroy = -93.5, offset_gyroz = -6.0;
// Máximos e mínimos para os offsets
double max_accelx = -32768.0, min_accelx = 32767.0, max_accely = -32768.0, min_accely = 32767.0, max_accelz = -32768.0, min_accelz = 32767.0;
double max_gyrox = -32768.0, min_gyrox = 32767.0, max_gyroy = -32768.0, min_gyroy = 32767.0, max_gyroz = -32768.0, min_gyroz = 32767.0;
// Valores "RAW" de tipo inteiro
signed int raw_accelx, raw_accely, raw_accelz;
signed int raw_gyrox, raw_gyroy, raw_gyroz;
signed int raw_temp;
// Valores "RAW" de tipo double
double Raw_accelx, Raw_accely, Raw_accelz;
double Raw_gyrox, Raw_gyroy, Raw_gyroz;
double Raw_temp;
// Saídas calibradas
double accelx, accely, accelz;
double gyrox, gyroy, gyroz;
double temp;
// Bytes
unsigned char data[1];
unsigned char GirAcel[14];
// Tempos
unsigned long t_ini_config, t_fim_config, t_ini1_cal, t_fim1_cal, t_ini2_cal, t_fim2_cal, t_ini3_cal, t_fim3_cal, t_ini, t_fim, t;
int i;
char text[40];

void main(void) {
    settings();
    while(1){
        readI2C(Adress_IMU,0x3B,GirAcel,14);
        raw_accelx = ((GirAcel[0] << 8) | GirAcel[1]);
        raw_accely = ((GirAcel[2] << 8) | GirAcel[3]);
        raw_accelz = ((GirAcel[4] << 8) | GirAcel[5]);
        raw_temp = ((GirAcel[6] << 8) | GirAcel[7]);
        raw_gyrox = ((GirAcel[8] << 8) | GirAcel[9]);
        raw_gyroy = ((GirAcel[10] << 8) | GirAcel[11]);
        raw_gyroz = ((GirAcel[12] << 8) | GirAcel[13]);
        //valores de entero a double
        Raw_accelx = raw_accelx; Raw_accely = raw_accely; Raw_accelz = raw_accelz; 
        Raw_gyrox = raw_gyrox; Raw_gyroy = raw_gyroy; Raw_gyroz = raw_gyroz;
        Raw_temp = raw_temp;
        //valores calibrados
        accelx = (float)raw_accelx * SENSITIVITY_ACCEL; accely = (float)raw_accely * SENSITIVITY_ACCEL; accelz = (float)raw_accelz * SENSITIVITY_ACCEL;//g (32768/2 = 1g para a escala configurada +-2g)
        gyrox = (float)raw_gyrox * SENSITIVITY_GYRO; gyroy = (float)raw_gyroy * SENSITIVITY_GYRO; gyroz = (float)raw_gyroz * SENSITIVITY_GYRO;//°/s
        temp = (float)(raw_temp/SENSITIVITY_TEMP) + TEMP_OFFSET; //°C
        sprintf(text,"ax, ay, az, t, gx, gy, gz: %.3f %.3f %.3f %.3f %.3f %.3f %.3f\n",accelx, accely, accelz, temp, gyrox, gyroy, gyroz);
        start();
    }
}

void settings(void){
    OSCCON=0x72;
    ADCON1=0x0F;
    //Configurar pines de comunicacion I2C
    TRISBbits.RB0=1;
    TRISBbits.RB1=1;
    SSPSTAT = 0x80;
    SSPCON1 = 0x28;
    SSPCON2 = 0x00;
    SSPADD = 0x19;
    //Serial UART
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    SPBRG = 0x0C;
    RCSTA = 0x90;
    TXSTA = 0x20;
    sprintf(text,"INICIALIZANDO A COMUNICACAO I2C \n");
    start();
    //MPU9250
    sprintf(text,"Write 1st I2C \n");
    start();
    writeI2C1(Adress_IMU, 0x6B,0x00);
    __delay_ms(10);
    sprintf(text,"INICIALIZANDO TEST WHO I AM \n");
    start();
    readI2C(Adress_IMU,0x75,data,1);
    if (data[0] != 0x68) { // DEFAULT_REGISTER_WHO_AM_I_MPU6500 0x71
        sprintf(text,"TEST WHO I AM FALLIDO \n");
        start();
        while(1);
    }
    else{
        sprintf(text,"TEST WHO I AM FUNCIONO\n");
        start();
    }
    __delay_ms(10);
    
    writeI2C1(Adress_IMU,0x1B,GYRO_FULL_SCALE_250_DPS);
    writeI2C1(Adress_IMU,0x1C,ACC_FULL_SCALE_2_G);
    __delay_ms(10);
}

void start (void){
    for(i=0;i<=strlen(text);i++){
        while(TXIF == 0);//cuando la bandera este en 1 cuando el registro esta vacio
        TXREG = text[i];
        //__delay_ms(time2);
    }
    /*while(TXIF == 0);
    TXREG = 0x0A;*/
    while(TXIF == 0);
    TXREG = 0x0D;
    //__delay_ms(time2);
}

void writeI2C(unsigned char Address,unsigned char Register,unsigned char *Data,int bytes){
    startI2C();
    sendI2C(Address);
    sendI2C(Register);
    for(i=0;i<bytes;i++){
        sendI2C(*Data);
        Data++;
    }
    stopI2C();
    __delay_ms(time);
}

void startI2C(void){
    readyI2C();
    SEN = 1; //coloca en ILDE
}

void readyI2C(void){
    //while(!SSPIF);
    //SSPIF = 0;
    while((SSPCON2 & 0x1F) || (SSPSTAT & 0x04)){
        //sprintf(text,"%x,%x \n ",SSPCON2,SSPSTAT);
        //start();
    }//SSPSTAT = ESTA TRANSMITIENDO INFORMACION//SSPCON2 = ver datasheet 
}

void sendI2C(unsigned char data){
    readyI2C();
    SSPBUF = data;
}

void stopI2C(void)
{
    readyI2C();
    PEN = 1;
}

void readI2C(unsigned char Address,unsigned char Register,unsigned char *Data,int bytes){
    startI2C();
    sendI2C(Address);
    sendI2C(Register);
    repeatedStart();
    sendI2C(Address|0x01);
    //settingsLCD(RAW1);
    for(i=0;i<bytes;i++){
        if(i+1 == bytes){
            *Data = receiveI2C(1);
        }
        else{
            *Data = receiveI2C(0);
        }
        //WriteLCD(*Data);
        Data++;
    }
    //settingsLCD(CD);
    stopI2C();
    //__delay_ms(time);
}

void repeatedStart(void){
    readyI2C();
    RSEN = 1;
}

unsigned char receiveI2C(char flag){
    unsigned char buffer;
    readyI2C();
    RCEN = 1;
    readyI2C();
    buffer = SSPBUF;
    readyI2C();
    ACKDT = flag==1? 1 : 0;
    ACKEN = 1;
    readyI2C();
    return buffer;
}

void writeI2C1(unsigned char Address,unsigned char Register,unsigned char Data){
    startI2C();
    sendI2C(Address);
    sendI2C(Register);
    sendI2C(Data);
    stopI2C();
    __delay_ms(time);
}
