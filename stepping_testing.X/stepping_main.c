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
       switch(STATE)
       {
            case 0:
                if(turn_degrees(45))
                {
                    STATE = 1;
                }
                break;
           case 1:
                if(go_straight_inches(3))
                {
                    STATE = 0;
                }
                break;
       }            
    }
}
