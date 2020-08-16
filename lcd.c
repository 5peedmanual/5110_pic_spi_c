/*
 * File:   lcd.c
 * Author: omron
 *
 * Created on August 12, 2020, 5:23 PM
 */


#include <xc.h>
#include <stdint.h>



#include "cristal.h"
#include "lcd.h"
#include "pic16f88.h"
#include "spi.h"


#define RST PORTBbits.RB5 // led 6
#define DC  PORTBbits.RB0
#define SCE PORTBbits.RB3

#define HIGH    1
#define LOW     0

#define LCD_X 84
#define LCD_Y 48



static void reset_lcd(void);
static void set_sce(uint8_t mode);
static void set_dc(uint8_t mode);



/*
 * When H = 0 the commands 
 * ?display control?, 
 * ?setY address? and 
 * ?set X address? can be performed
 *  when H = 1, the others can be executed.
 * The ?write data? and?function set? commands can be executed in both cases.
 */




void init_lcd(void) {
    /*
     * Immediately following power-on, the contents of all internal registers
     *  and of the RAM are undefined.A RES pulse must be applied.
     */
    
    
    reset_lcd();
    
    /* Set PD to 0, to activate the chip, choose addressing with V bit,
     * 0 is horizontal, 1 is vertical. Select instruction set with H bit,
     * 0 is basic instruction set, 1 is extended instruction set.
     * 
     * 
     * Function set instructions D/C need to be 0. To write data (writes data to display RAM)
     *  set D/C high. So we are going to set it low to proceed with initialization
     * 
     * 
     * The byte:
     * 
     * DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
     *  0   0   0   0   0  PD   V   H
     */
    
    
    /* set extended instruction mode            */
    /* H = 1                                    */
    write_function_lcd(0x21);
    __delay_ms(2000);
    /* set V0P, something to do with voltage... */
    write_function_lcd(0xB0);
    __delay_ms(2000);

    write_function_lcd(0x04);
    __delay_ms(2000);

    write_function_lcd(0x14);
    __delay_ms(2000);    
    /* set normal instruction mode              */
    /* H = 0                                    */
    write_function_lcd(0x20);
    __delay_ms(2000);    
    /* display control set normal mode          */
    /* D = 1, E = 0                             */
    write_function_lcd(0x0C);
    __delay_ms(5000);    
    /************ init complete *****************/
    
    

    return;
}



static void reset_lcd(void)

{
    /* RES is active low so we must set it low to reset the lcd */
    RST = 0;
    __delay_ms(50);
    RST = 1;
    //set_sce(LOW);
    //RST = 0;
    //__delay_ms(2000);
    //set_sce(HIGH);
    //RST = 1;
}



static void set_sce(uint8_t mode)
{
    if (mode == LOW) {
        /* SCE is going LOW */
        SCE = 0;
    }
    
    if (mode == HIGH) {
        SCE = 1;
    }
        
    
}


/* set data or command mode */
static void set_dc(uint8_t mode)
{
    if (mode == LOW) {
        /* SCE is going LOW */
        DC = 0;
    }
    
    if (mode == HIGH) {
        DC = 1;
    }
        
    
}



void write_function_lcd(uint8_t byte) {
    set_dc(LOW);
    set_sce(LOW);
    write_spi(byte);
    set_dc(HIGH);
    set_sce(HIGH);
    
    
}



void write_data_lcd(uint8_t byte)
{
    uint8_t slash = byte;
    // slash = (uint8_t) 0x159;
    set_dc(HIGH);
    set_sce(LOW);
    
    
    /*
     * If D/C(mode select) is set LOW, the current byte is interpreted 
     * as command byte.
     * If D/C is set HIGH, the following bytes are stored in the display dataRAM.
     */
    
    write_spi(slash);
    set_dc(LOW);
    set_sce(HIGH);
}


void set_pos_xy(uint8_t x, uint8_t y)
{
    /* set X 0x80 1 0000000 (d7, d6...d0) */
    /*            X address (d7, d6...d0) */
    
    
    /* lets go to x0, y0*/
    write_function_lcd(0x80); // x0
    write_function_lcd(0x40); // y0
    //write_data_lcd(0xFF);
}

void clear_lcd(void)
{
    for (int index =0; index < (LCD_X *LCD_Y /8); index++) {
        write_data_lcd(0x00);
    }
    
    set_pos_xy(0, 0);
}


