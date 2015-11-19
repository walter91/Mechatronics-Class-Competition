#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

#include "C:\Users\Walter\Documents\Walter\School\Fall 2015\Mechatronics\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions

int STATE = 0;

int main()
{
    pin_config_init();
    timing_interrupt_config();
    
    //Wait to move hand from switch
        delay(15000);
        
    while(1)
    {
      
        //if(go_straight_inches(13))
        //{
            //do nothing
        //}
        
                
        switch(STATE)
       {
            case 0:
                if(go_straight_inches(10))
                {
                    delay(250);
                    STATE = 1;
                }
                break;
           case 1:
                if(go_straight_inches(-5))
                {
                    delay(250);
                    STATE = 2;
                }
                break;
            case 2:
                if(go_straight_inches(5))
                {
                    delay(250);
                    STATE = 3;
                }
                break;
            case 3:
                if(go_straight_inches(-10))
                {
                    delay(10000);
                    STATE = 0;
                }
                break;
       }
                 
    }
}
