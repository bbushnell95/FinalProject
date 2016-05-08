#include <sys/attribs.h>
#include "UART.h"
#include <xc.h>
void initUART()
{
    
    RPF0Rbits.RPF0R=0b0001; //remap TX
    TRISFbits.TRISF0 = 0; //set TX as output
    
    
    U2RXRbits.U2RXR = 0b0100; //remap RX
    TRISFbits.TRISF1 = 1; //set RX as input
    
    //set all TX modes
    U2MODEbits.ON = 0;
    U2MODEbits.SIDL = 0;
    U2MODEbits.IREN = 0;
    U2MODEbits.RTSMD = 1;
    U2MODEbits.UEN = 0;
    U2MODEbits.STSEL = 0;

    
    U2STAbits.URXISEL = 0;
    
    U2BRG = 52;//(8000000.0/(16.0*9600.0)) - 1.0;
    
    U2STAbits.URXEN = 1; 
    U2STAbits.UTXEN = 1;
    U2MODEbits.ON = 1;
}
