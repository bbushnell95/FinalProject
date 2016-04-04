#include <xc.h>
#include <sys/attribs.h>
#include <string.h>
#include "lcd.h"
#include "timer.h"
#include "config.h"
#include "interrupt.h"

#define TRIS_pin1 TRISBbits.TRISB10
#define TRIS_pin2 TRISBbits.TRISB12
#define TRIS_leftPin TRISBbits.TRISB0
#define TRIS_rightPin TRISBbits.TRISB2
#define TRIS_frontPin TRISBbits.TRISB4
#define TRIS_backPin TRISBbits.TRISB6

#define pin1 LATBbits.LATB10
#define pin2 LATBbits.LATB12 

#define leftPin LATBbits.LATB0  
#define rightPin LATBbits.LATB2
#define frontPin LATBbits.LATB4
#define backPin LATBbits.LATB6

#define leftSensor LATDbits.LATD0       //change these later
#define middleSensor LATDbits.LATD1
#define rightSensor LATDbits.LATD2

#define ON 1
#define OFF 0
#define OUTPUT 0
#define INPUT 1

#define leftWheel OC2RS
#define rightWheel OC4RS

void displayVoltage();       //used to write the output voltage to the LCD
char* buildString(float value);
int readFromADC();

typedef enum stateTypeEnum{
    Forward, Backwards
} stateType;

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
    initLCD();
    initADC();
    initPWM();

    int State = Forward;
    
    TRIS_pin1 = OUTPUT;
    TRIS_pin2 = OUTPUT;
    TRIS_leftPin = INPUT;
    TRIS_rightPin = INPUT;
    TRIS_frontPin = INPUT;
    TRIS_backPin = INPUT;
    
    pin1=ON;
    pin2=OFF;
    while(1){
        //read from the three sensors
        readFromADC();
        calculateODC();
        
        //if left is higher OC2RS goes low/oc4rs goes high
        //if right is higher oc4rs goes low/oc2rs goes high
        //if middle is highest stay the same
        
        
        //if all three read same voltage turn around
        
    }
}

//controls the speed of the wheels
//when pot is in the middle both motors should be going full speed
//when pot is rotated all the way the respective motor should be going full speed

void calculateODC(){
    if (endFlag == 0){
        if(left>1000 && right>1000){
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

int readFromADC(){
    if(IFS0bits.AD1IF == 1){
        int *buffer = &ADC1BUF0;
        left = *buffer;
        right = *(buffer+1);
        //front = *(buffer+2);
        //back = *(buffer+3);
        
        IFS0bits.AD1IF = 0;
    }
}