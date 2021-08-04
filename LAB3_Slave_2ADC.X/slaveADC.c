/*
 * File:   main.c
 * Author: Fernando Sandoval
 * CARNE: 18313
 * DIGITAL 2
 * Created on July 22, 2021, 2:32 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
#define _XTAL_FREQ 8000000 

// LIBRARIES
#include <xc.h>
#include <stdint.h>
#include "ADC.h"
#include "SPI.h"



// VAR

uint8_t var0; 
uint8_t var1;
uint8_t F1=0; 
uint8_t F0; 

// FUNCTION PROTOTYPES
void initCONFIG(void);

// MAIN CODE
void __interrupt() isr(void) {
    if(ADIF == 1){
        switch (F0){  // SW FLAG TO CONTROL CHANNEL SELECTION OVER ADC CONVERSIONS
            case 1:
                var0 = ADRESH;
                ADCON0bits.CHS0 = 1;
                F0 = 0;
                break;
            case 0:
                var1 = ADRESH;
                ADCON0bits.CHS0 = 0;
                F0 = 1;
                break;
        }
                ADIF = 0; 
                __delay_us(60);
                ADCON0bits.GO = 1;   
    }
    if(SSPIF == 1){
        F1 = spiRead();
        if (F1 == 1){
            spiWrite(var0);
        }
        else if(F1 == 2){
            spiWrite(var1);
        }
        SSPIF = 0;
    }
    }
   
// MAIN VOID
void main(void) {
    initCONFIG(); 
    while(1){     
    }
    return;
}

void initCONFIG(void){
    ANSELH = 0;
    ANSEL = 0b00000011;  // AN0, AN1 SELECT CORRESPONDING TO RA0, RA1
    TRISA = 0b00001011;  // RA0, RA1 AS INPUTS
    TRISB = 0b00001000;
    TRISC = 0b00011000;  // TX, RX AS INPUTS CORRESPONDING TO RC6,RC7
    TRISD = 0; 
    TRISE = 0; 
    PORTA = 0; 
    PORTC = 0;    
    PORTD = 0;
    PORTE = 0;
    OSCCONbits.IRCF2 = 1; // OSCILLATOR TO 8MHZ
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    setupADC();
    INTCONbits.GIE = 1;  
    INTCONbits.T0IE = 1;           
    INTCONbits.T0IF = 0; 
    INTCONbits.PEIE = 1; 
    PIR1bits.SSPIF = 0;     
    PIE1bits.SSPIE = 1;      
    TRISBbits.TRISB3 = 1;     
    PIE1bits.ADIE = 1;  
    PIR1bits.ADIF = 0; 
    spiInit(SPI_SLAVE_SS_EN, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);  
}


