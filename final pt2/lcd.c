/*
 * File:   lcd.c
 * Authors:
 *
 * Created on December 31, 2014, 1:39 PM
 */


#include <xc.h>
#include "lcd.h"
#include "timer.h"

/* The pins that you will use for the lcd control will be
 *                     
 * LCD_RS      RC4     
 * LCD_E       RC2     
 * LCD_D4      RE7     
 * LCD_D5      RE5      
 * LCD_D6      RE3      
 * LCD_D7      RE1      
 * Use these so that we will be able to test your code for grading and to
 * help you debug your implementation!
 */

#define TRIS_D7 TRISEbits.TRISE1
#define TRIS_D6 TRISEbits.TRISE3
#define TRIS_D5 TRISEbits.TRISE5
#define TRIS_D4 TRISEbits.TRISE7
#define TRIS_RS TRISCbits.TRISC4
#define TRIS_E  TRISCbits.TRISC2


#define LCD_D7_3  LATEbits.LATE1
#define LCD_D6_2  LATEbits.LATE3
#define LCD_D5_1  LATEbits.LATE5
#define LCD_D4_0  LATEbits.LATE7
#define LCD_RS  LATCbits.LATC4
#define LCD_E   LATCbits.LATC2


/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATE. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower){
    LCD_RS = commandType;

    if (lower == 1) {
        LCD_D4_0 = word&0x01;
        LCD_D5_1 = (word&0x02)>>1;
        LCD_D6_2 = (word&0x04)>>2;
        LCD_D7_3 = (word&0x08)>>3;
    }
    else if (lower == 0){
        LCD_D4_0 = (word&0x10)>>4;     //16
        LCD_D5_1 = (word&0x20)>>5;     //32
        LCD_D6_2 = (word&0x40)>>6;     //64
        LCD_D7_3 = (word&0x80)>>7;     //128
    }
    
    LCD_E = 1;          //enable
    delayUs(1);          //delay        
    LCD_E=0;            //disable   
    delayUs(1);
    LCD_RS = 0;
    delayUs(1);
    delayUs(delayAfter);//delay
}

/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter){
    writeFourBits(word,commandType,1,0);   
    writeFourBits(word,commandType,delayAfter,1);
}

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) {
    writeLCD(c,1,50);     
}
void initLCD(void) {    
    TRIS_D7 = 0;        //everything is an output
    TRIS_D6 = 0;
    TRIS_D5 = 0;
    TRIS_D4 = 0;
    TRIS_RS = 0;
    TRIS_E  = 0;

    // Enable 4-bit interface
    delayMs(15);    // wait 15ms or more after Vdd reaches 4.5V

    /*SYD:
     set RS 0 R/W 0 DB7-4 0 0 1 1
     * delay 4.2 ms
     */
    
    writeFourBits(0b00110000, 0, 4200, 0);      
    
    /*SYD:
     set RS 0 R/W 0 DB7-4 0 0 1 1
     * wait 100 us or more
     */
    writeFourBits(0b00110000, 0, 150, 0);       
    
    /*SYD:
     * set RS 0 R/W 0 DB7-4 0 0 1 1 (upper)
     * no delay
     * set DB7-4 0 0 1 0
     * delay 40 us
     */   
    writeLCD(0b00110010, 0, 40);
    /*SYD:
     Function Set Initialization
     */
    writeLCD(0b00101000, 0, 40);
    /*SYD:
     Display Off Init
     */
    writeLCD(0b00001000, 0, 40);     
    /*SYD:
     Display Clear
     */
    writeLCD(0b00000001, 0, 1640);
     /*SYD:
     Entry Mode Set
     */
    writeLCD(0b00000110, 0, 40);

    writeLCD(0b00001111, 0, 40);        //turns display on w/ blinking cursor
}

/*
 * function takes a constant char* and prints the string to the LCD
 */
void printStringLCD(const char* s) {

    int i = 0;
    while(s[i] != '\0'){
        printCharLCD( s[i] );
        i++;
    }
}
/*
 * Clear the display.
 */
void clearLCD(){
    writeFourBits(0b00000001,0, 1, 0);
    writeFourBits(0b00000001,0, 1640, 1);
}
/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 * Assigns C to be the binary value of any position the lcd then sends the command to the 
 * LCD to move the cursor
 */
void moveCursorLCD(unsigned char x, unsigned char y)
{
    char c=NULL;

    if(y==1)
    {
        if(x==1) c=0b10000000;
        else if(x==2)c=0b10000001;
        else if(x==3)c=0b10000010;
        else if(x==4)c=0b10000011;
        else if(x==5)c=0b10000100;
        else if(x==6)c=0b10000101;
        else if(x==7)c=0b10000110;
        else if(x==8)c=0b10000111;
        else if(x==9)c=0b10001000;
        else if(x==10)c=0b10001001;
        else if(x==11)c=0b10001010;
        else if(x==12)c=0b10001011;
        else if(x==13)c=0b10001100;
        else if(x==14)c=0b10001101;
        else if(x==15)c=0b10001110;
        else if(x==16)c=0b10001111;        
    }
    else if(y==2)
    {
        if(x==1) c=0b11000000;
        else if(x==2)c=0b11000001;
        else if(x==3)c=0b11000010;
        else if(x==4)c=0b11000011;
        else if(x==5)c=0b11000100;
        else if(x==6)c=0b11000101;
        else if(x==7)c=0b11000110;
        else if(x==8)c=0b11000111;
        else if(x==9)c=0b11001000;
        else if(x==10)c=0b11001001;
        else if(x==11)c=0b11001010;
        else if(x==12)c=0b11001011;
        else if(x==13)c=0b11001100;
        else if(x==14)c=0b11001101;
        else if(x==15)c=0b11001110;
        else if(x==16)c=0b11001111;
    }
    
    writeLCD(c,0,40);
}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD(){
    initLCD();
    int i = 0;
    printCharLCD('c');
    for(i = 0; i < 1000; i++) delayUs(1000);
    clearLCD();
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for(i = 0; i < 1000; i++) delayUs(1000);
    printStringLCD("Hello!");
    for(i = 0; i < 1000; i++) delayUs(1000);
}