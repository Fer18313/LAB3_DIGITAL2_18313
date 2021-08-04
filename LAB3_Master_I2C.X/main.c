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

// LIBS
#include <xc.h>
#include <stdint.h> 
#include "SPI.h"


// VARS
uint8_t J_1 = 0;
uint8_t J_2_dontpayenough = 0;
uint8_t unidad = 0;
uint8_t decathlon = 0;
uint8_t var = 1;
uint8_t temp_read;  
uint8_t counter=0; 
uint8_t HUGO=0;
uint8_t c; 
uint8_t var1; 
uint8_t var0;
uint8_t J0 = 1;
uint8_t unit; 
uint8_t unit_1_change;
uint8_t k0;
uint8_t k1;
uint8_t dozen;
uint8_t dozen_1;
int8_t FZERO; 


// VOID PROTOTYPES
void initCONFIG(void); 
void initTMR0(void);

// VOID MAIN CODE

void main(void) {
    initCONFIG();
    initTMR0();
    while(1){
       PORTAbits.RA0 = 0;
       __delay_ms(1);
       
       spiWrite(var);
       if (var == 1) {
       var0 = spiRead();
       var = 2;
       }
       else if (var == 2) {
       var1 = spiRead();
       var = 1;
       }
       __delay_ms(1);
       PORTAbits.RA0 = 1;          
       __delay_ms(250);
    unit = var1 / 51;
    dozen = ((var1 * 100 / 51) - (unit*100))/10;
    dozen_1 = ((var1 * 100 / 51) - (unit*100) - (dozen*10));  
    unit_1_change = var0 / 51;
    k0 = (((var0 * 100) / 51) - (unit_1_change*100))/10;
    k1 = (((var0 * 100) / 51) - (unit_1_change*100) - (k0*10));   
    
    if (PORTDbits.RD1 == 1){
        HUGO = 0;
        counter = 0;
        PORTD = 0;
        J0 = 1;
        J_1 = 0;
        J_2_dontpayenough = 0;
    }
    if(c > 20){ 
         c = 0;
         TXIE = 1; 
     }
    }
    return;
}


// VECTOR INTERRUPT
void __interrupt() isr(void){
    if (INTCONbits.T0IF){         
        c++;
        INTCONbits.T0IF = 0;      
    }
    if (TXIF == 1){
         switch(FZERO){
            case 0:
                TXREG = unit + 48;  // 48 MEANS ASCII FOR ZERO.
                FZERO = 1;
                break;
            case 1:
                TXREG = 0b101110;
                FZERO = 2;
                break;
            case 2:
                TXREG = dozen + 48; 
                FZERO = 3;
                break;
            case 3:
                TXREG = dozen_1 + 48; 
                FZERO = 4;
                break;
            case 4:
                TXREG = 48;
                FZERO = 5;
                break;
            case 5:
                TXREG = unit_1_change + 48;
                FZERO = 6;    
                break;
            case 6:
                TXREG = 0b101110;
                FZERO = 7;
                break;
            case 7:
                TXREG = k0 + 48;
                FZERO = 8;
                break;
            case 8:
                TXREG = k1 + 48;
                FZERO = 9;
                break;
            case 9:
                TXREG = 0b1101;
                FZERO = 0;    
                break;
        }       
    TXIF = 0;
    }   
    if(PIR1bits.RCIF == 1){ 
        if (RCREG ==  0x0D){
        PORTB = counter; 
        PORTD =2;
        }
        if (RCREG !=  0x0D){
        temp_read = RCREG;
            if(temp_read==48){
                HUGO = 0;
            }else if(temp_read==49){
                HUGO = 1;
            }else if(temp_read==50){
                HUGO = 2;
            }else if(temp_read==51){
                HUGO = 3;
            }else if(temp_read==52){
                HUGO = 4;
            }else if(temp_read==53){
                HUGO = 5;
            }else if(temp_read==54){
                HUGO = 6;
            }else if(temp_read==55){
                HUGO = 7;
            }else if(temp_read==56){
                HUGO = 8;
            }else if(temp_read==57){
                HUGO = 9;
            }
        if (J0 == 1){
            counter = HUGO;
            unidad = HUGO;
            J0 = 0;
            J_1 = 1;
        }
        else if (J_1 == 1){
            counter = (unidad*10)+HUGO;
            decathlon = HUGO;
            J_1 = 0;
            J_2_dontpayenough = 1;
        }
        else if (J_2_dontpayenough == 1){
            counter = (unidad*100)+(decathlon*10)+HUGO;
            J_1 = 0;
            J_2_dontpayenough = 1;
        }     
        }}
}

void initCONFIG(void){
    OSCCONbits.IRCF2 = 1; 
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 1;
    ANSELH = 0;
    ANSEL = 0b00000000;
    TRISA = 0b00000000;
    TRISB = 0b00000000;
    TRISC = 0b10010000;
    TRISD = 0;
    TRISE = 0;    
    PORTA = 0; 
    PORTB = 0;
    PORTC = 0;    
    PORTD = 0;
    PORTE = 0;
    INTCONbits.GIE = 1; 
    INTCONbits.T0IE = 1;           
    INTCONbits.T0IF = 0; 
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 1;
    BAUDCTLbits.BRG16 = 0;
    SPBRG = 51;
    SPBRGH = 0;
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;
    RCSTAbits.CREN = 1; 
    TXSTAbits.TXEN = 1; 
    TRISA0 = 0;
    PORTAbits.RA0 = 1;
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
}
void initTMR0(){
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1; 
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = 217;  
}