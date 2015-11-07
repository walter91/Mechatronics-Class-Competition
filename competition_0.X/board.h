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

//Pin Definitions
#define PIN_STEP_R _RB0 //4
#define PIN_STEP_L _RB1 //5

#define PIN_DIR_R _RB7  //11
#define PIN_DIR_L _RB8  //12

#define PIN_IR_F _RA0 //2
#define PIN_IR_B _RA1 //3

#define PIN_DIST_F _RB2  //6
#define PIN_DIST_B _RA2  //7

#define PIN_SWITCH_R _RB13  //16
#define PIN_SWITCH_L _RB14  //17


//Values
#define LEFT_FORWARD 0
#define RIGHT_FORWARD 1

#define LEFT_BACKWARD 1
#define RIGHT_BACKWARD 0

#define STEP_DELAY 10

#define INCH_TO_WALL 24.0
#define DIST_TOL 0.1

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


int find_normal()
{
	static int statFlag = 0;
	static int dirFlag = 0;
	static float dist1 = read_dist();	//Dist1 = Measure distance
	//TODO: Write read_dist() function. Should return float value of filtered distance measurements...
	if(dirFlag ==0)
	{
		turn_degrees(1);	//Turn 1 degree
	}
	else
	{
		turn_degrees(-1);
	}
	static float dist2;	//Dist2 = Measure distance
	dist2 = read_dist();
	
	if((dist2 > dist1) && (statFlag == 0))	//If (Dist2 > Dist1 and Flag = 0)	//Going wrong direction, first time through
	{
		if(dirFlag == 1)	//	Go the other way?
		{
			dirFlag = 0;
		}
		else
		{
			dirFlag = 1;
		}
		
		return(0);	//keep searching
	}		
	else if(dist1 > dist2)	//Going right direction
	{
		//	Continue turning in that direction
		statFlag = 0;		//	Flag = 1
		return(0);	// keep searching
	}
	else if((dist2 > dist1) && (statFlag == 1))		//Else if(Dist2 > Dist1 && Flag==1)	//Reached the minimum
	{
		if(dirFlag == 0)
		{
			turn_degrees(-.5);	//	Go back half a step in angle
		}
		else
		{
			turn_degrees(.5);	//	Go back half a step in angle
		}
		
		return(1);	//	Return(1)
		statFlag = 0;	//	Flag = 0
	}
}

float abs_f(float value)
{
	if(value >= 0)
		return(value);
	else
		return(-1.0*value);
}

int abs_i(int value)
{
	if(value >= 0)
		return(value);
	else
		return(-1*value);
}



int find_24()
{
	static float error;
		
	error = (INCH_TO_WALL -  distance_measured());	//Error = (24 - Dist)
	//TODO: write a distance_measured() function. This should return a float which is a filtered value representing the distance from the wall
		
	if(abs_f(error) < DIST_TOL)	//We are there...
	{
		return(1)	//Center Found
	}
	
	else	//Change position...
	{
		if(error > 0)	//Positive error, move backward
		{
			go_straight_inches(-.5);
		}
		else	//Negative error, move forward
		{
			go_straight_inches(.5);
		}
		
		return(0);	//Keep looking for center
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

