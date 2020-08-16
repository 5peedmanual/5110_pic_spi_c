/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "cristal.h"
#include "lcd.h"
#include "pic16f88.h"
#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "spi.h"
#include "user.h"          /* User funct/params, such as InitApp */



/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/

/* i.e. uint8_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
void main(void)
{
    /* Configure the oscillator for the device */
    configure_oscillator();
    /* Initialize I/O and Peripherals for application */
    init_app();
    
    init_spi();
    
    init_lcd();
    clear_lcd();
    write_function_lcd(0x0D);
    uint8_t c = 0x0F;
    while (1) {

        __delay_ms(100);
        write_data_lcd(c);

        __delay_ms(100);
    }

}

