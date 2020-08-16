/*
 * File:   spi.c
 * Author: omron
 *
 * Created on August 11, 2020, 7:03 PM
 */


#include <xc.h>


#include "spi.h"
#include "cristal.h"

/*
 * SDO - RB2 
 * SDI - RB1
 * SCL - RB4
 */

static void configure_clock_spi(void);
static void configure_transfer_spi(void);




void init_spi(void) {
    
    
    configure_clock_spi();
    
    configure_transfer_spi();
    /*
     * SSPEN: Synchronous Serial Port Enable bit(1)
     * In SPI mode:
     * 1 = Enables serial port and configures SCK, SDO and SDI as serial port pins
     * 0 = Disables serial port and configures these pins as I/O port pins 
     */
    SSPCONbits.SSPEN    = 1;
    
    
    
    
    return;
}



static void configure_clock_spi(void)
{
    /* SPI clock polarity which determines if the clock will idle high or low   */
    /*  Transmit happens on rising edge,receive on falling edge.                */
    /*  Idle state for clock is a low level                                     */
    SSPCONbits.CKP      = 0;    
    
    SSPSTATbits.CKE     = 0; 
    
    TRISBbits.TRISB4    = 0;    /*  SCK                 */

    SSPCONbits.SSPM0    = 0;    /*  1/64                */
    SSPCONbits.SSPM1    = 1;    /*  1/64                */
}



static void configure_transfer_spi(void)
{
    SSPSTATbits.SMP     = 0;    /*  Data sample timing  */
    SSPSTATbits.BF      = 0;    /*  Buffer full         */  /* Status bit */
    
    
    
    STATUSbits.RP0      = 1; // we're at bank 1
    /* 
     they  must have  their  data  direction  bits  (in  the  TRISB  register)appropriately programmed. 
     * That is: 
     * SDI must have TRISB<1> set
     * SDO must have TRISB<2> cleared
     * SCK (Master mode) must have TRISB<4> cleared
     */
    TRISBbits.TRISB2    = 0;    /*  SDO                 */  
    TRISBbits.TRISB1    = 1;    /*  SDI                 */ 
    
    STATUSbits.RP0      = 0;
}



int write_spi(uint8_t b) {
    /* sspbuf register when filled, send data to the slave */
    int count = 0;
    
    SSPBUF = b;
    //SSPBUF = 0b01010101;
    do {
        __delay_ms(1);
    } while (SSPSTATbits.BF == 0);
        
        
    read_spi();
    return 0;
        
    
}



void read_spi(void) {
    //while ( !SSPSTATbits.BF );
    unsigned char trash;
    trash = SSPBUF;

}



void stop_spi(void) 
{
    /* disable the serial port*/
    SSPCONbits.SSPEN = 0;
}
