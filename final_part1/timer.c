/*
 * File:   timer.c
 * Authors:
 *
 * Created on December 30, 2014, 8:07 PM
 */

#include <xc.h>
#include "timer.h"


void initTimer1(){
    TMR1=0;
    IFS0bits.T1IF=0;        //interrupt flag is down
}

void delayMs(unsigned int delay){
    TMR1=0;
    T1CONbits.TCKPS=3;      //prescaler of 256
    PR1=(((delay*10000)/256)-1);
    
    T1CONbits.ON=1;
    IFS0bits.T1IF=0;
    while(IFS0bits.T1IF==0)
    {
        
    }
    T1CONbits.ON=0;
    IFS0bits.T1IF=0;
}


initTimer4(){
    TMR4=0;
    IFS0bits.T4IF=0;        //flag
}
void delayUs(unsigned int delay){
    TMR4=0;
    T4CONbits.TCKPS=0;      //prescale of 1
    PR4=((delay*10)-1);
    T4CONbits.ON=1;
    IFS0bits.T4IF=0;
    while(IFS0bits.T4IF==0)
    {
        
    }
    T4CONbits.ON=0;
    IFS0bits.T4IF=0;
}