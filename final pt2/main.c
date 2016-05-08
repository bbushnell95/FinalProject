#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"
#include "led.h"
#include "switch.h"
#include "SPI.h"
#include "UART.h"

#define TRIS_pin1 TRISBbits.TRISB10
#define TRIS_pin2 TRISBbits.TRISB12
#define TRIS_leftPin TRISBbits.TRISB0
#define TRIS_rightPin TRISBbits.TRISB2
#define TRIS_frontPin TRISBbits.TRISB4

#define pin1 LATBbits.LATB10
#define pin2 LATBbits.LATB12 

#define leftPin LATBbits.LATB0  
#define rightPin LATBbits.LATB2
#define frontPin LATBbits.LATB4     

#define ledLeft LATDbits.LATD2       
#define ledFront LATDbits.LATD1
#define ledRight LATDbits.LATD0


#define ON 1
#define OFF 0
#define OUTPUT 0
#define INPUT 1

#define leftWheel OC2RS
#define rightWheel OC4RS

#define STOP 0b11100000
#define LEFT 0b11000011
#define RIGHT 0b11001000
#define BACKWARD 0b11101111
#define FORWARD 0b11100111

const char* WEDIDTHETHING="#WeDidTheThing";
const char* DOMINATION="Domination";

void readFromADC();
void adjustLED();
void lineUP();


volatile int val = 0;
volatile int switchFlag = 0;
volatile int pressCount = 0;     //used to only switch states with one press

int left = 0;
int right = 0;
int front = 0;
int back = 0;
int endFlag = 0;
int DLoop = 0;

int main(void){
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initLEDs();
    initTimer1();
    initTimer4();
    initADC();
    initPWM();
    initUART();
    initSwitch();
    initLCD();
   
    char input = '/0';
    unsigned char input2 = '/0';
    TRIS_pin1 = OUTPUT;
    TRIS_pin2 = OUTPUT;
    TRIS_leftPin = INPUT;
    TRIS_rightPin = INPUT;
    TRIS_frontPin = INPUT;
    
    pin1=ON;
    pin2=OFF;
    
    clearLCD();
    printStringLCD(WEDIDTHETHING);
    moveCursorLCD(3,2);
    printStringLCD(DOMINATION);

    while(1){

        if(U2STAbits.URXDA == 1)
        {
            input = (U2RXREG & 0xFF); //takes character out of buffer
            input2 = input;           //needed to feed through a second variable to strip 
                                      // unwanted characters
        }

        switch(input2) {
            case FORWARD: //forward
                pin1=OFF;
                pin2=ON;
                leftWheel= 10000;
                rightWheel= 9750; 
                break;
            case LEFT: //left
                leftWheel= 0;
                rightWheel= 9000;
                break;
            case BACKWARD: //backward
                pin1=ON;
                pin2=OFF;
                leftWheel= 10000;
                rightWheel= 9750;               
                break;
            case RIGHT: //right
                leftWheel= 9000;
                rightWheel= 0;                
                break;
            case STOP:  //Stop
                pin1=ON;
                pin2=ON;
                break;
        }
    }
}


//Change notification is for the push of a button used for starting the robot
//in part one
void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt()
{
    PORTD;
    IFS1bits.CNDIF = 0;       //set switch flag back down
    switchFlag = 1;
}

//This function is used to indicate to the user when the robot is in the
//correct starting position over the black tape. All LED's will be on if it 
//is lined up correctly.
void lineUP(){
    if (left > 500){
        ledLeft = ON;
    }
    else{
        ledLeft = OFF;
    }
    if (right > 500){
        ledRight = ON;
    }
    else{
        ledRight = OFF;
    }
    if (front > 500){
        ledRight = OFF;
        ledLeft = OFF;
    }
}