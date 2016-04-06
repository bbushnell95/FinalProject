#include "switch.h"
#include <xc.h>


#define TRIS_RD6 TRISDbits.TRISD6
#define OUTPUT 1


void initSwitch(){
    TRIS_RD6 = OUTPUT;
    
    CNPUDbits.CNPUD6=1;
    CNCONDbits.ON=1;        //turn on CN for port D
    CNENDbits.CNIED6=1;     //enables CN for RD6
    IFS1bits.CNDIF=0;
    IEC1bits.CNDIE=1;
    IPC8bits.CNIP=7;    
}

