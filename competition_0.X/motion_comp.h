/* 
 * File: motion.h  
 * Author: Walter Coe
 * Comments: Functions and Declarations which govern the motion for the 
 *              ME330 shooter at BYU.
 * Revision history: See Github...
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


#define PIN_STEP_R _RB0 //4
#define PIN_STEP_L _RB1 //5

#define PIN_DIR_R _RB7  //11
#define PIN_DIR_L _RB8  //12

#define LEFT_FORWARD 0
#define RIGHT_FORWARD 1

#define LEFT_BACKWARD 1
#define RIGHT_BACKWARD 0

#define STEP_DELAY 10

void toggle(int pinToToggle)
{
    if(pinToToggle)
    {
        pinToToggle = 0;
    }
    else
    {
        pinToToggle = 1;
    }
}




int go_straight_inches(float inches)
	{
		#define stepsPerRev 200 //Number of steps required for one rev of motor
		#define wheelCircumferenceInches 12.76	//Drive wheel circumference in inches
		static int stepsPerInch = (int)(stepsPerRev/wheelCircumferenceInches);	//Number of pulses required to move forward 1 inch (pulses/revolution)*(revolutions/inch)
		static int numberOfSteps;
        static int stepsTaken = 0;
        static long lastTime;
        lastTime = milliseconds;
        
        numberOfSteps = (int)(inches*stepsPerInch);	//convert inches to number of steps
		
        if(inches >- 0)
        {
            PIN_DIR_R = RIGHT_FORWARD;  //set direction pins, both forward
            PIN_DIR_L = LEFT_FORWARD;  //set direction pins, both forward
        }
        
        else
        {
            PIN_DIR_R = RIGHT_BACKWARD;  //set direction pins, both forward
            PIN_DIR_L = LEFT_BACKWARD;  //set direction pins, both forward
        }
        
        if(stepsTaken < numberOfSteps)  //Not enough steps yet...
        {
            if((milliseconds - lastTime) < STEP_DELAY)  //Not yet waited long enough
            {
                //do nothing YET...
            }
            else    //Its time to step...
            {
                toggle(PIN_STEP_R); //Change the value from 1->0 or visa-versa
                toggle(PIN_STEP_L); //Change the value from 1->0 or visa-versa
                if(PIN_STEP_R == 1) //Step pin was toggled HIGH
                {
                    stepsTaken++;   //Count steps when pulse goes HIGH
                }
            }
            return(0);  //Need to continue
        }
        else    //We've taken enough steps
        {
            stepsTaken = 0; //Reset steps taken for next call
            return(1);  //We're done...
        }
	}








int turn_degrees(int degrees)
	{
		static int stepsPerDegree = 1.055; //Number of pulses required to spin 1 degrees
		static int numberOfSteps;   //Number of steps to reach the desired degrees
        numberOfSteps = (int)(degrees * stepsPerDegree);
        static int stepsTaken = 0;
        static long lastTime;
        lastTime = milliseconds;
        
        
        if(degrees >= 0)
        {
            PIN_DIR_R = RIGHT_BACKWARD;  //set direction pins
            PIN_DIR_L = LEFT_FORWARD;  //set direction pins
        }
        else
        {
            PIN_DIR_R = RIGHT_FORWARD;  //set direction pins
            PIN_DIR_L = LEFT_BACKWARD;  //set direction pins
        }
		
		if(stepsTaken < numberOfSteps)  //Not enough steps yet...
        {
            if((milliseconds - lastTime) < STEP_DELAY)  //Not yet waited long enough
            {
                //do nothing YET...
            }
            else    //Its time to step...
            {
                toggle(PIN_STEP_R); //Change the value from 1->0 or visa-versa
                toggle(PIN_STEP_L); //Change the value from 1->0 or visa-versa
                if(PIN_STEP_R == 1) //Step pin was toggled HIGH
                {
                    stepsTaken++;   //Count steps when pulse goes HIGH
                }
            }
            return(0);  //Need to continue
        }
        else    //We've taken enough steps
        {
            stepsTaken = 0; //Reset steps taken for next call
            return(1);  //We're done...
        }
	}








#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

