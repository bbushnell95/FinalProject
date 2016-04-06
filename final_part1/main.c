#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"
#include "led.h"

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
    initTimer1();
    initTimer4();
    initADC();
    initPWM();
    initLEDs();

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
                    if(ledLeft == ON && ledFront == ON && ledRight == ON){
                        if (switchFlag == 1){
                            State = Drive;
                        }
                    }
                    switchFlag = 0;
                    delayMs(20);
                    break;
                case Drive:
                    
                    readFromADC();
                    adjustLED();
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
    pressCount++;
}

//controls the speed of the wheels
//when pot is in the middle both motors should be going full speed
//when pot is rotated all the way the respective motor should be going full speed

void calculateODC(){
    if (endFlag == 0){
        if(left>1000 && right>1000 && front>1000){
           endFlag = 1;
           leftWheel = 0;
           rightWheel=7500;
        }
        else if(left>1000){
            leftWheel = 5000;
            rightWheel=6000;
        }
        else if(right>1000){
            leftWheel = 6000;
            rightWheel=5000;  
        }
        else{
            leftWheel=7500;
            rightWheel=7500;
        }
    }
    else if(endFlag == 1){
        while (endFlag == 1){
            if(left < 1000 && right < 1000){
                endFlag = 0;
                leftWheel = 6000;
                rightWheel= 6000;
                break;
            }
        }
    }
}

void readFromADC(){
    if(IFS0bits.AD1IF == 1){
        int *buffer = &ADC1BUF0;
        left = *buffer;
        right = *(buffer+1);
        front = *(buffer+2);
        //back = *(buffer+3);
        
        IFS0bits.AD1IF = 0;
    }
}

void adjustLED(){
    if (left>1000){
        ledLeft = ON;
    }
    else{
        ledLeft = OFF;
    }
    if (front>1000){
        ledFront = ON;
    }
    else{
        ledFront = OFF;
    }
    if (right>1000){
        ledRight = ON;
    }
    else{
        ledRight = OFF;
    }
}

void lineUP(){
    if (left<1000){
        ledLeft = ON;
    }
    else{
        ledLeft = OFF;
    }
    if (front>1000){
        ledFront = ON;
    }
    else{
        ledFront = OFF;
    }
    if (right<1000){
        ledRight = ON;
    }
    else{
        ledRight = OFF;
    }
}