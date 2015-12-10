#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling


//#include "C:\Users\Walter\Documents\Walter\School\Fall 2015\Mechatronics\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions
#include "C:\Users\David\Documents\GitHub\Mechatronics-Class-Competition\competition.X\board.h"

int STATE = 2;
unsigned long startTime = 0;



float find_normal_angle()
    {
    int i, j;

    float distance[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    float derivative[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    float angle[4] = {0,0,0,0};

    for(i=0; i<36; i++)		
    {
        distance[i] = analog_ultra_inches();	//record distances...
        while(!turn_degrees(5));	//turn 5 degrees
    }

    //distances[] is now built

    for(i=1; i<35; i++)
    {
        derivative[i] = (distance[i+1] - distance[i-1])/(2.0*5.0);	//Change is distance per degree, central difference
    }

    derivative[0] = (((distance[1] - distance[0])/10.0) + derivative[1])/2.0;	//Estimate the inital derivative
    derivative[35] = (((distance[35] - distance[34])/10.0) + derivative[34])/2.0;

    //derivative[] is now built

    for(i=0; i<=35; i++)	//Search for the minimums, where the derivative changes sign
    {
        if(derivative[i]*derivative[i+1] >= 0)	//both positive or both negative
        {
            //do nothing
        }
        else
        {
            angle[j] = i*5.0;	//record the angle in degrees from the start position to the minimum...
            j++;
        }
    }

    //angle[] now holds the angles to each of the four normals...

    return(angle[1]);
}




int main()
{
    pin_config_init();
    timing_interrupt_config();
    //ir_finder_analog_setup();
    //analog_ultrasonic_setup();
    
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
        
        float angle = find_normal_angle();
        
        switch(STATE)
       {
            case 0:
                if(turn_degrees(angle))//if(find_normal_analog())
                {
                    STATE = 1;
                }
                break;
            case 1:
               delay(1000);
               STATE = 0;
               break;
            case 2:
               if(find_inches(24.0))
               {
                   delay(5000);
                    //STATE = 1;
               }
               break;
       }
    }
}
