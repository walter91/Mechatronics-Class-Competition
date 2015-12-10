#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling


//#include "C:\Users\Walter\Documents\Walter\School\Fall 2015\Mechatronics\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions
#include "C:\Users\David\Documents\GitHub\Mechatronics-Class-Competition\competition.X\board.h"

int STATE = 2;
int findCenterState = 0;
unsigned long startTime = 0;



float find_normal_angle();
//    {
//    int i, j;
//
//    float distance[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//    float distance_filtered[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//    float derivative[36] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//    
//    for(i=0; i<36; i++)		
//    {
//        distance[i] = analog_ultra_inches();	//record distances...
//        while(!turn_degrees(5));	//turn 5 degrees
//    }
//    
//    float alpha = .75;
//    for(i=1; i<36; i++)	//Filter the distances	
//    {
//        distance_filtered[i] = alpha*distance[i] + (1.0 - alpha)*distance[i-1];	//record distances...
//    }
//    distance_filtered[0] = distance_filtered[1];
//    //distances[] is now built
//
//    for(i=1; i<35; i++)
//    {
//        derivative[i] = (distance[i+1] - distance[i-1])/(2.0*5.0);	//Change is distance per degree, central difference
//    }
//
//    derivative[0] = (((distance[1] - distance[0])/10.0) + derivative[1])/2.0;	//Estimate the inital derivative
//    derivative[35] = (((distance[35] - distance[34])/10.0) + derivative[34])/2.0;
//
//    //derivative[] is now built
//    float derivMin;
//    float angleToMinDerive;
//    for(i=0; i<36; i++)
//    {
//        derivative[i] = abs_f(derivative[i]);
//        if(i==0)
//        {
//            derivMin = derivative[i];
//            angleToMinDerive = i*5.0;
//        }
//        else if(derivative[i]<derivMin)
//        {
//            derivMin = derivative[i];
//            angleToMinDerive = i*5.0;
//        }
//    }
//    
//    return(angleToMinDerive - 180.0);
//}

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
    
    float angle = find_normal_angle();
    STATE = 0;
    
    while(1)
    {        
        
//        switch(findCenterState)
//				{
//					case 0:
//						if(find_normal())   //First normal found
//						{
//							findCenterState = 1;
//						}
//						else    //Keep looking
//						{
//							//do nothing
//						}
//						break;
//					case 1:
//						if(find_24())   //First 24" found
//						{
//							findCenterState = 2;
//						}
//						else    //Keep looking
//						{
//							//do nothing
//						}
//						break;
//					case 2:
//						if(turn_degrees(90))    //Turned 90 degrees
//						{
//							findCenterState = 3;
//						}
//						else
//						{
//							//do nothing
//						}
//						break;
//					case 3:
//						if(find_24())   //Second 24" found
//						{
//							findCenterState = 0;
//							//STATE = findLoader;
//                            
//                            delay(10000);
//						}
//						else
//						{
//							//do nothing
//						}
//						break;
//				}
        
        
        switch(STATE)
       {
            case 0:
                if(turn_degrees(angle))//if(find_normal_analog())
                {
                    STATE = 1;
                }
                break;
            case 1:
               delay(5000);
               STATE = 2;
               break;
            case 2:
               if(find_inches(24.0))
               {
                   delay(5000);
                    STATE = 3;
               }
               break;
            case 3:
               if(turn_degrees(90))
               {
                   delay(5000);
                    STATE = 4;
               }
               break;
            case 4:
               if(find_inches(24.0))
               {
                   delay(5000);
                    STATE = 5;
               }
               break;
            case 5:
               //do nothing...
               break;
       }
    }
}
