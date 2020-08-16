/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"
#include "pic16f88.h"


#define RST PORTBbits.RB5
//#include "pic16f627.h"


/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/
static void enable_global_interrupts(void);

/* <Initialize variables in user.h and insert code for user algorithms.> */

void init_app(void)
{
     STATUSbits.RP0     = 1; // we're at bank 1
     
    /* TODO Initialize User Ports/Peripherals/Project here */
    
     /* PORTA is input */
    TRISAbits.TRISA0    = 1;
    TRISAbits.TRISA1    = 1;
    TRISAbits.TRISA2    = 1;
    TRISAbits.TRISA3    = 1;
             
     
    /* lets set PORTB to digital outputs */
    TRISB = 0;  // PORTB is now directed as output~
    RST = 1;
   

    /* Setup analog functionality and port direction */
    ANSEL = 0; // set as digital i/O

    /* Initialize peripherals */
     /* The PIE1 register contains peripheral interrupt enable bits.  */

    /* Enable interrupts */
     
     enable_global_interrupts();
    /*
    * ********************************************************************** *
    ADIE - A/D Converter Interrupt Enable bit.
        1 - Enables the ADC interrupt.
        0 - Disables the ADC interrupt.

    RCIE - EUSART Receive Interrupt Enable bit.
        1 - Enables the EUSART receive interrupt.
        0 - Disables the EUSART receive interrupt.

    TXIE - EUSART Transmit Interrupt Enable bit.
        1 - Enables the EUSART transmit interrupt.
        0 - Disables the EUSART transmit interrupt.

    SSPIE - Master Synchronous Serial Port (MSSP) Interrupt Enable bit - 
        * enables an interrupt request to be generated upon each data 
        * transmission via synchronous serial communication module 
        *     (SPI or I2C mode).
        1 - Enables the MSSP interrupt.
        0 - Disables the MSSP interrupt.

    CCP1IE - CCP1 Interrupt Enable bit enables an interrupt request to be 
      * generated in CCP1 module used for PWM signal processing.
        1 - Enables the CCP1 interrupt.
        0 - Disables the CCP1 interrupt.

    TMR2IE - TMR2 to PR2 Match Interrupt Enable bit
        1 - Enables the TMR2 to PR2 match interrupt.
        0 - Disables the TMR2 to PR2 match interrupt.

    TMR1IE - TMR1 Overflow Interrupt Enable bit enables an 
      * interrupt request to be generated upon each timer TMR1 register 
      * overflow, i.e. when the counting starts from zero.
        1 - Enables the TMR1 overflow interrupt.
        0 - Disables the TMR1 overflow interrupt.
      * 
      * ********************************************************************** *
      */
     PIE1bits.TMR1IE = 1;
     
     //set_timer1();
     
     
     /* 10h T1CON? T1RUN T1CKPS1 T1CKPS0 T1OSCEN T1SYNC TMR1CS TMR1ON -000 0000 */
     /* Timer1  can  be  enabled/disabled  by  setting/clearingcontrol bit, TMR1ON (T1CON<0>) */
     
     
}



void set_timer0(void) 
{
    TMR0 = 0xFF;
    //INTCON = 0x20;     // Bit T0IE is set, bit T0IF is cleared
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 1;
}

void set_timer1(void)
{
    PIR1bits.TMR1IF     = 0;        // Reset the TMR1IF flag bit

    T1CONbits.T1OSCEN   = 1;     // enabling timer1 external oscillator
    T1CKPS1 = T1CKPS0   = 1;      // Assigned prescaler rate is 1:8
    PIR1bits.TMR1IF     = 1;        // 

    // TMR1H = 0xFc;
    // TMR1L = 0xFc;
    TMR1 = 0xFF;                // initial value
    //T1CONbits.TMR1ON = 1;        // Turn the timer TMR1 on
}

void on_timer1(void)
{
    T1CONbits.TMR1ON    = 1;
}

static void enable_global_interrupts(void)
{
        INTCON = 0xC0;              // Enable interrupt (bits GIE and PEIE)
}

