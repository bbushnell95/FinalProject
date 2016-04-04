#include "PWM.h"
#include <xc.h>

void initPWM(){
    RPD1Rbits.RPD1R = 0b1011; //mapping to RD1
    RPD3Rbits.RPD3R = 0b1011; //mapping to RD3
    
    T2CONbits.TCKPS = 0;
    PR2 = 10000;
    TMR2 = 0;
    IFS0bits.T2IF=0;
    
    OC2CONbits.OCTSEL = 0;      //init OC2      left wheel
    OC2CONbits.OCM = 6;
    OC2RS = 7500;

     OC2CONbits.ON = 1;
    
    OC4CONbits.OCTSEL = 0;      //init OC4      right wheel
    OC4CONbits.OCM = 6;
    OC4RS = 7500;

     OC4CONbits.ON = 1;
     
     T2CONbits.TON = 1;  
}
