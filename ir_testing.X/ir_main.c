#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

#include "C:\Users\Walter\Documents\Walter\School\Fall 2015\Mechatronics\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions


int main()
{
    pin_config_init();
   
    //ADC is configured in pin_config_int()
    
    //return values between 0-100 for percent of IR seen
    
    static float voltageLow = 0.3;
          
    float numLow;
    numLow = (4095)*(1.22/3.3);
    
    float percent;
    float percent2;
    
    //return(percent);
    
    while(1)
    {
        float percent_front = ir_front_percent();
        float percent_back = ir_back_percent();
        
    }
}
