#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

#include "C:\Users\Walter\Documents\Walter\School\Fall 2015\Mechatronics\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions


int main()
{
    pin_config_init();
   
    //ADC is configured in pin_config_int()
    
    //return values between 0-100 for percent of IR seen
    
    static float voltageLow = 1.22;
          
    float numLow;
    numLow = (4095)*(1.22/3.3);
    
    float percent;
    float percent2;
    
    //return(percent);
    
    while(1)
    {
        percent = 100.0 * ((ADC1BUF1-numLow)/(4095.0-numLow));
        percent2 = ir_front_percent();
        
    }
}
