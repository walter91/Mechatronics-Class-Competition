#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

#include "C:\Users\Walter\Documents\Walter\School\Fall 2015\Mechatronics\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions

int STATE = 2;
unsigned long startTime = 0;

int main()
{
    pin_config_init();
    timing_interrupt_config();
    ir_finder_analog_setup();
    analog_ultrasonic_setup();
    
    //analog_ultrasonic_setup();
    //ultrasonic_setup();
    //config_pwm_14();
    
   // ANSA = 1;
    //ANSB = 1;   //Digital buffer is active
    
    //_TRISA0 = 1;
    //_TRISB2 = 1;
    //_TRISA2 = 1;
    //_TRISA0 = 1;
    //_TRISA1 = 1;
    while(1)
    {
        
        //OC1R = ((read_dist_simple()/148.0)/48.0)*PR2;    //Should output PWM with duty cycle from 0%-100% for 0"-48"
        
        switch(STATE)
       {
            case 0:
                if(find_normal())//if(find_normal_analog())
                {
                    STATE = 1;
                }
                break;
            case 1:
               delay(1000);
               STATE = 0;
               break;
            case 2:
               if(find_inches(10.0))
               {
                   delay(5000);
                    //STATE = 1;
               }
               break;
       }
    }
}
