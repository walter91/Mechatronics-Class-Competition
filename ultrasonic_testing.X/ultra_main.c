#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

#include "C:\Users\Walter\Documents\Walter\School\Fall 2015\Mechatronics\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions

int STATE = 0;

int main()
{
    pin_config_init();
    timing_interrupt_config();
    
    while(1)
    {
        float inches = analog_ultra_inches();        
        
    }
}

