#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"
#include "led.h"
#include "switch.h"

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

typedef enum stateTypeEnum{
    lineUp, Drive
} stateType;

void readFromADC();
void adjustLED();
void lineUP();

volatile int val = 0;
volatile int switchFlag = 0;
volatile int pressCount = 0;     //used to only switch states with one press
void calculateODC();


int left = 0;
int right = 0;
int front = 0;
int back = 0;
int endFlag = 0;

int main(void){
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    initLEDs();
    initTimer1();
    initTimer4();
    initADC();
    initPWM();
    initSwitch();
    

    int State=lineUp;
    
    TRIS_pin1 = OUTPUT;
    TRIS_pin2 = OUTPUT;
    TRIS_leftPin = INPUT;
    TRIS_rightPin = INPUT;
    TRIS_frontPin = INPUT;
    
    pin1=ON;
    pin2=OFF;
    while(1){
        switch(State){
                case lineUp:
                    readFromADC();
                    lineUP();
                    if(ledLeft == ON  && ledRight == ON){    //&& ledRight == ON
                        if (switchFlag == 1){
                            State = Drive;
                            switchFlag = 0;
                        }
                    }
                    switchFlag = 0;
                    delayMs(20);
                    break;
                case Drive:
                    //leftWheel=10000;
                    //rightWheel=10000;
                    readFromADC();
                    //adjustLED();
                    calculateODC();
                    delayMs(20);
                    break;
        }
    }
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt()
{
    PORTD;
    IFS1bits.CNDIF = 0;       //set switch flag back down
    switchFlag = 1;
}

//controls the speed of the wheels
//when pot is in the middle both motors should be going full speed
//when pot is rotated all the way the respective motor should be going full speed

void calculateODC(){
    if (endFlag == 0){  //endFlag == 0
        /*if(left<500 && right<500){
           endFlag = 1;
           leftWheel = 0;
           rightWheel=6000;
        }
        else */if(left<400){
            leftWheel = 0;
            rightWheel=6000;
        }
        else if(right<400){
            leftWheel = 7000;
            rightWheel=0;  
        }
        else {
            leftWheel=6500;
            rightWheel=5500;
        }
    }
    else{
        leftWheel = 0;
        rightWheel = 0;
    }
    /*else if(endFlag == 1){
        while (endFlag == 1){
            if(left < 500 && right < 500){
                endFlag = 0;
                leftWheel = 6000;
                rightWheel= 6000;
                break;
            }
        }
    }*/
}

void readFromADC(){
    if(IFS0bits.AD1IF == 1){
        left = ADC1BUF0;
        right=ADC1BUF1;
        front=ADC1BUF2;   
        IFS0bits.AD1IF = 0;
    }
}



void adjustLED(){
    if (front > 400){
        ledLeft = ON;
    }
    else{
        ledLeft = OFF;
    }
    /*if (right > 500){
        ledRight = ON;
    }
    else{
        ledRight = OFF;
    }*/
}

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