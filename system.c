/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */


#include "pic16f88.h"
#include "system.h"
#include "user.h"
#include "cristal.h"


static void check_good_clock(void);
static void set_external_osc(void);
static void select_frequency(void);

/* Refer to the device datasheet for information about available
oscillator configurations. */
void configure_oscillator(void)
{
    /* TODO Add clock switching code if appropriate.  */

    /* Typical actions in this function are to tweak the oscillator tuning
    register, select new clock sources, and to wait until new clock sources
    are stable before resuming execution of the main project. */
    //check_good_clock();
    
    STATUSbits.RP0 = 1; // we're at bank 1
    
    
    set_external_osc(); // we're using RA5/CLKIN pin for clock input
    select_frequency(); // set at 4MHz
    
    
    
    STATUSbits.RP0 = 0; // go back to bank 0
                          
}


static void check_good_clock(void)
{
    /* For this reason, the Oscillator Start-up Timer (OST) is used to give the
     *  drive circuitry time to settle. The OST counts 1024 oscillations from 
     * the crystal/resonator drive circuit at the OSC1 pin. The count begins 
     * after a Power On Reset (POR) and if enabled, the Power-Up Reset Timer 
     * (PWRT) has expired. The OST ensures that the crystal oscillator
     *  have started and can provide a stable system clock.*/
    
    //set_timer1();
    if (OSCCONbits.OSTS) {
        return;
    }    
    
}



static void set_external_osc(void)
{
    /* OSCCON 8Fh at bank 1*/
    /*
     * SCS - System Clock Select bit determines which oscillator is to be used 
     * as a clock source.
     * 
     *  1 - Internal oscillator is used for system clock.
     *  0 - External oscillator is used for system clock. The oscillator mode is 
     *      set by bits in Config Word (FOSC) written to the microcontroller memory 
     *      during the process of programming.
     */
    OSCCONbits.SCS = 0;

}



static void select_frequency(void)
{
    /* OSCCON 8Fh at bank 1*/
    /* frequency selection bits */
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
}
