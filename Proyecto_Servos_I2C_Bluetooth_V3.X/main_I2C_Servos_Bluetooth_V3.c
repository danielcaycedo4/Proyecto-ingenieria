#include <xc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#pragma config FOSC = INTOSC_HS
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000
#define time 100
#define time2 50

#define SlaveAddressROM1 0xA0//direccion de memoria 24LC04 memoria EEPROM
#define SlaveAddressROM2 0xA2
//PCA
#define Frequencia 50 // VALOR DA FREQUENCIA DO SERVO en Hz
#define I2C_ADDRESS_PCA9685 0x80
#define SERVOMIN 110 // VALOR PARA UM PULSO MAIOR QUE 1 mS
#define SERVOMAX 510 // VALOR PARA UM PULSO MENOR QUE 2 mS
#define PWM_Frequencia 1000
//Registros
#define PCA9685_MODE1 0x00 /**< Mode Register 1 */
#define PCA9685_PRESCALE 0xFE /**< Prescaler for PWM output frequency */
//Datos
#define FREQUENCY_CALIBRATED 26075000 /**< Oscillator frequency measured at 104.3% */
#define MODE1_AI 0x20 /**< Auto-Increment enabled */
#define PCA9685_PRESCALE_MIN 3 /**< minimum prescale value */
#define PCA9685_PRESCALE_MAX 255 /**< maximum prescale value */
#define MODE1_SLEEP 0x10 /**< Low power mode. Oscillator off */
#define MODE1_RESTART 0x80 /**< Restart enabled */

#define PCA9685_LED0_ON_L 0x06 /**< LED0 output and brightness control byte 0 */

void settings(void);
//Serial
void __interrupt() receive(void);
void start(void);
//I2C
void writeI2C(unsigned char Address,unsigned char Register,unsigned char *Data,int bytes);//*=puntero(direccion de memoria)
void startI2C(void);//empieza la comunicación
void readyI2C(void);//evita colicion de datos
void sendI2C(unsigned char data);
void stopI2C(void);
void readI2C(unsigned char Address,unsigned char Register,unsigned char *Data,int bytes);
void repeatedStart(void);
unsigned char receiveI2C(char flag);
//PCA9685
int map(int x,int In_Min,int In_Max,int Out_Min,int Out_Max);
void setPWMFreq(float freq);
void Servos(void);
//funcion para mover servos
void mover_servo(int servo,int angulo);

int i, pos, dato ,DriverPin = 0,servos=5,z=0,Angle;
unsigned char data1,data2[4],d1,flag=0;
char text[40],angle[3];

void main(void) {
    settings();
    while(1){
       
    }
}

void settings(void){
    OSCCON = 0x72;
    ADCON1 = 0x0F;
    TRISBbits.RB7=1;
    TRISBbits.RB6=0;
    //I2C
    TRISBbits.RB0=1;
    TRISBbits.RB1=1;
    SSPSTAT = 0x80;
    SSPCON1 = 0x28;
    SSPCON2 = 0x00;
    SSPADD = 0x13;
    //serial
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    SPBRG = 0x0C;//0x1F 20MHZ//0x0C 8MHz
    RCSTA = 0x90;
    TXSTA = 0x20;
    //PCA9685
    writeI2C(I2C_ADDRESS_PCA9685, PCA9685_MODE1,MODE1_RESTART,1);
    __delay_ms(10);
    //// set a default frequency
    setPWMFreq(PWM_Frequencia);
    //// set a servo frequency
    setPWMFreq(Frequencia);
    __delay_ms(300);
    //interrupcion
    GIE = 1;
    PEIE = 1;
    RCIE = 1;
    RCIF = 0;
    //
    sprintf(text,"Settings");
    start();
    PORTBbits.RB6=1;
    __delay_ms(1000);
    PORTBbits.RB6=0;
    mover_servo(0,90);
    mover_servo(1,90);
    mover_servo(2,90);
    mover_servo(15,90);
}

/////////Serial//////////
void __interrupt() receive(void){
    unsigned char d;
    if(RCIF == 1){
        d = RCREG;
        sprintf(text," El caracter recibido fue: %c",d);
        start();
        if(flag==0){//Configuracion del servo a mover
            flag=1;
            if(d=='0'){
                servos=0;
            }
            else if(d==0x0D || d==0x0A){
                mover_servo(0,0);
                mover_servo(0,90);
                mover_servo(0,180);
            }
            else if(d=='1'){
                servos=1;
            }
            else if(d=='2'){
                servos=2;
            }
            else if(d=='3'){
                servos=3;
            }
            else if(d=='4'){
                servos=4;
            }
            else if(d=='5'){
                servos=5;
            }
        }
        else if(flag==1){//Angulos del servo seleccionado en la condicion del flag = 0
            if(d=='1'){
                PORTBbits.RB6=1;
                mover_servo(servos,0);
            }
            else if(d=='2'){
                mover_servo(servos,45);
            }
            else if(d=='3'){
                mover_servo(servos,90);
            }
            else if(d=='4'){
                mover_servo(servos,135);
            }
            else if(d=='5'){
                mover_servo(servos,180);
            }
            else if(d=='s'){
                flag=0;
            }
            else if(d==0x0D || d==0x0A){
            }
            else if(d=='a'){//ASCII 225 ß 
                flag=2;
                z=0;
            }
            else{
                sprintf(text,"El caracter recibido fue: %c",d);
                start();
            }
        }
        else if(flag==2){
            angle[z]=d;
            sprintf(text,"angulo z %c \n",angle[z]);
            start();
            z++;
            if(z==3){
                z=0;
                Angle=atoi(angle);
                sprintf(text,"angulo atoi %d \n",Angle);
                start();
                mover_servo(servos,Angle);
            }
            if(d=='s'){
                flag=0;
            }
        }
    }
}

void start (void){
    for(i=0;i<=strlen(text);i++){
        while(TXIF == 0);//cuando la bandera este en 1 cuando el registro esta vacio
        TXREG = text[i];
    }
    /*while(TXIF == 0);
    TXREG = 0x0A;
    while(TXIF == 0);
    TXREG = 0x0D;*/
}

////////I2C//////////
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
    while((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));//SSPSTAT = ESTA TRANSMITIENDO INFORMACION//SSPCON2 = ver datasheet 
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
        if(Address == 0xA0 || Address == 0xA1){
            //WriteLCD(*Data);
        }
        Data++;
    }
    //settingsLCD(CD);
    stopI2C();
    __delay_ms(time);
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

void setPWMFreq(float freq){
    // Range output modulation frequency is dependant on oscillator
    if (freq < 1) freq = 1;
    if (freq > 3500) freq = 3500; // Datasheet limit is 3052=50MHz/(4*4096)
    /*
    freq *= 0.9; // Correct for overshoot in the frequency setting (see issue #11)
    float prescaleval = FREQUENCY_OSCILLATOR;
    */
    float prescaleval = FREQUENCY_CALIBRATED;
    prescaleval /= freq; // required output modulation frequency
    // rounding to nearest number is equal to adding 0,5 and floor to nearest number
    prescaleval += 2048;
    prescaleval /= 4096;
    prescaleval -= 1;
    if (prescaleval < PCA9685_PRESCALE_MIN) prescaleval = PCA9685_PRESCALE_MIN;
    if (prescaleval > PCA9685_PRESCALE_MAX) prescaleval = PCA9685_PRESCALE_MAX;
    unsigned char prescale = (unsigned char) prescaleval;
    readI2C(I2C_ADDRESS_PCA9685, PCA9685_MODE1, &data1, 1);
    //Serial.print("data");Serial.println(data[0],HEX);//arduino
    unsigned char newmode = (data1 & ~MODE1_RESTART) | MODE1_SLEEP; // sleep
    writeI2C(I2C_ADDRESS_PCA9685, PCA9685_MODE1, &newmode,1);// go to sleep
    writeI2C(I2C_ADDRESS_PCA9685, PCA9685_PRESCALE, &prescale,1);// set the prescaler
    writeI2C(I2C_ADDRESS_PCA9685, PCA9685_MODE1, &data1,1);
    __delay_ms(5);
    // This sets the MODE1 register to turn on auto increment.
    d1=(data1 | MODE1_RESTART | MODE1_AI);
    writeI2C(I2C_ADDRESS_PCA9685, PCA9685_MODE1, &d1, 1);
}

int map(int x,int In_Min,int In_Max,int Out_Min,int Out_Max){
    return ((x - In_Min) * ((Out_Max - Out_Min) / (In_Max - In_Min))) + Out_Min;
}

void mover_servo(int servo,int angulo){
    pos = map (angulo, 0 , 180 , SERVOMIN, SERVOMAX);
    data2[2] = pos;
    data2[3] = pos >> 8;
    writeI2C(I2C_ADDRESS_PCA9685, PCA9685_LED0_ON_L + (4 * servo), data2, 4);
    sprintf(text," Servo: %d \n",servo);
    start();
    sprintf(text," Angulo: %d \n",angulo);
    start();
    __delay_ms(1000);
}
