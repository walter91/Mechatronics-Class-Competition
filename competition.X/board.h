/* 
 * File: board.h  
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

_FICD(ICS_PGx3);

//Values
#define LEFT_FORWARD 1
#define RIGHT_FORWARD 0

#define LEFT_BACKWARD 1
#define RIGHT_BACKWARD 0

#define STEP_DELAY 1 // Is this value arbitrary right now or is this a minimum/optimized value? - David

#define INCH_TO_WALL 24.0
#define INCH_FROM_ULTRA_TO_CENTER 4.5
#define DIST_TOL 0.25
#define STANDARD_STEP_LENGTH .2
#define STANDARD_STEP_ANGLE 5
#define LARGE_STEP_ANGLE 10
//Half Step mode
#define stepsPerRev 400.0 //Number of steps required for one rev of motor
#define wheelCircumferenceInches 12.7172 //4.048*3.14	//Drive wheel circumference in inches
#define stepsPerInch  15.6363 // Calibrated //31.4534 //15.7267 //(stepsPerRev/wheelCircumferenceInches)	//Number of pulses required to move forward 1 inch (pulses/revolution)*(revolutions/inch)
#define stepsPerDegree 0.84553 //calibrated //.836 //1.672 // stepsperInch*0.0266*2 //was 10.055

#define INCH_PER_VOLT 0.0465
#define INCH_OFFSET .1955

#define LOADING_IR_FREQ 100

#define INCHES_CORNER_TO_CENTER 33.3

#define IR_FOUND_THRESH 70  //Percent of full voltage

#define INCH_PER_MIRCOSECONDS .00676



unsigned long milliseconds = 0; //Will run for 48+ days before overflow...
unsigned long microseconds = 0;	//Will overflow after 71 minutes
unsigned long countTimer = 0;

unsigned long simpleUltrasonicTime = 0;

unsigned long startTimeUltraF;
unsigned long startTimeUltraB;

unsigned long stopTimeUltraF;
unsigned long stopTimeUltraB;

unsigned long timeTemp;
unsigned long timeTempMillis;

int ultraLastStateB;
int ultraLastStateF;

int targetsFound = 0b000;

int loaderIrState;
#define IR_TIMES 3
unsigned long irTimeValues[IR_TIMES];

#define ULTRASONIC_VALUES 10

unsigned long ultrasonicValuesB[ULTRASONIC_VALUES];
unsigned long ultrasonicValuesF[ULTRASONIC_VALUES];

int i;
unsigned long startTime;

void delay(float millis)
{
    unsigned long startT;
    startT = milliseconds;
    while(milliseconds - startT <= millis)
    {
        //wait
    }
}



void pin_config_init()
{
	//_TRISA5 = 0;	//LOADING SWEEPER, PIN1
	
    _TRISA0 = 1;    //IR FRONT, PIN2
    _TRISA1 = 1;    //IR BACK, PIN3
    
    _TRISB0 = 0;    //FRONT TRIGGER, PIN4
    _TRISB1 = 0;    //BACK TRIGGER, PIN5
    
    _TRISB2 = 1;    //ULTRASONIC ECHO FRONT/ULTRASONIC ANALOG, PIN6
    _TRISA2 = 1;    //ULTRASONIC ECHO BACK, PIN7
    
    _TRISA3 = 1;    //LOADING SENSOR INPUT, PIN8
    
    _TRISB7 = 0;    //DIR-RIGHT, PIN11
    _TRISB8 = 0;    //DIR-LEFT, PIN12
    _TRISB15 = 0;   //STEP, PIN18
    
    _TRISB9 = 0;    //LOADER TOP, PIN13
    _TRISA6 = 0;    //SHOOTER, PIN14
    _TRISB12 = 0;   //LOADER BOTTOM, PIN15
}



void analog_ultrasonic_setup()
{  
    //------------------------------------------------------------------------
    // A /D Configuration Function
    //------------------------------------------------------------------------
    
    // AD1CHS register
    _CH0NA = 0;         // AD1CHS<7:5> -- Use VDD as negative input

    // AD1CON1 register
    _ADON = 1;          // AD1CON1<15> -- Turn on A/D
    _ADSIDL = 0;        // AD1CON1<13> -- A/D continues while in idle mode?
    _MODE12 = 1;        // AD1CON1<10> -- 12-bit
    _FORM = 0b00;       // AD1CON1<9:8> -- Output format, pg. 211
    _SSRC = 0b0111;     // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;          // AD1CON1<2> -- Auto sampling

    // AD1CSSL registers
    AD1CSSL = 0b0000000000010011;   // AD1CSSL<15:0> -- Select lower channels to scan, turn on AN0 and AN1 and AN4
    AD1CSSH = 0x0000;               // AD1CSSH<15:0> -- Select upper channels to scan, NOT USED!

    // AD1CON2 register, see pg. 212
    _PVCFG = 0b00;      // AD1CON2<15:14> -- Use VDD as positive ref voltage
    _NVCFG = 0;         // AD1CON2<13> -- Use VSS as negative ref voltage
    _BUFREGEN = 1;      // AD1CON2<11> -- Results stored using channel indexed
                        // mode -- AN1 result is stored in ADC1BUF1, AN2 result
                        // is stored in ADC1BUF2, etc.
    _CSCNA = 1;         // AD1CON2<10> -- Scans inputs specified in AD1CSSx
                        // registers instead of using channels specified
                        // by CH0SA bits in AD1CHS register
    _ALTS = 0;          // AD1CON2<0> -- Sample MUXA only
    _SMPI = 0b00001;    // AD1CON2<6:2> -- Interrupts at the conversion for
                        // every other sample

    // AD1CON3 register
    _ADRC = 0;              // AD1CON3<15> -- Use system clock
    _SAMC = 0x00001;        // AD1CON3<12:8> -- Auto sample every A/D period TAD
    _ADCS = 0x00111111;     // AD1CON3<7:0> -- A/D period TAD = 64*TCY

    ANSA = 1;       //Turn on analog for port A
	ANSB = 1;		//Turn on analog for port B
	
    _TRISA0 = 1;    //Pin2 as Input
    _TRISA1 = 1;    //Pin3 as Input
	_TRISB2 = 1;	//Pin6 as Input
	
}



void loading_timer(unsigned long waitTime)
{
	static int state = 0;
	static int returnFlag = 0;
    static unsigned long startWaitTime = 0;
	
	switch(state)
	{
		case 0:
			startWaitTime = milliseconds;
			state = 1;
			returnFlag = 0;
			_RA5 = 1;	//turn on loading sweeper, PIN1
			break;
		case 1:
			if((milliseconds - startWaitTime) >= waitTime)	//Timer has expired
			{
				state = 0;
				returnFlag = 1;
				_RA5 = 0;	//turn off loading sweeper, PIN1
			}
			else
			{
				//don't change state
				returnFlag = 0;
			}		
			break;
	}
	return(returnFlag);
}



float analog_ultra_inches()
{
	float ultraVolts;
	ultraVolts = (ADC1BUF0/4095.0)*3.3;	//Read analog-to-digital converter and save in volts
    
	float inches;
	inches = ultraVolts*20.0-3.25
	
    return(inches);
}



void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    // Remember to clear the Timer1 interrupt flag when this ISR is entered.
    _T1IF = 0; // Clear interrupt flag
    
	microseconds = microseconds + 100;
	
	countTimer++;
	
	if(countTimer >= 10)
    {
		milliseconds++;
		countTimer = 0;
	}
}



void timing_interrupt_config()
{
	_TON = 1;	//Turn on timer 1, for general use
	
    _TCKPS = 0b00;  //Timer pre-scaler set to 1:1, page 139
    _RCDIV = 0b011; //1mHz (with 8mHz), divide by 8
    
    // Configure Timer1 interrupt
    _T1IP = 5;      // Select interrupt priority
    _T1IE = 1;      // Enable interrupt
    _T1IF = 0;      // Clear interrupt flag
    PR1 = 50;    // Count to 1 milli-sec at 1 mHz, instruct at 500 kHz
}



float ultra_avg()
{
    static unsigned long averageMicrosF;
	//static unsigned long averageMicrosB;
	static unsigned long oldAverageMicrosF;
	//static unsigned long oldAverageMicrosB;
	static float distance;
	
	static unsigned long sumMicrosF;
	//static unsigned long sumMicrosB;
	
	sumMicrosF = 0;
	//sumMicrosB = 0;
	
	for(i = 0; i <= ULTRASONIC_VALUES; i++)
	{
		sumMicrosF = sumMicrosF + ultrasonicValuesF[i];
		//sumMicrosB = sumMicrosB + ultrasonicValuesB[i];
	}
	
	sumMicrosF = sumMicrosF + oldAverageMicrosF;
	//sumMicrosB = sumMicrosB + oldAverageMicrosB;
	
	averageMicrosF = sumMicrosF/(ULTRASONIC_VALUES + 1);
	//averageMicrosB = sumMicrosB/(ULTRASONIC_VALUES + 1);
	
	oldAverageMicrosF = averageMicrosF;
	//oldAverageMicrosB = averageMicrosB;
    
    distance = averageMicrosF;
    //distance  =  (averageMicrosF+averageMicrosB)/2.0;
    
    
    return(distance);
}



float read_dist()
{
	static unsigned long averageMicrosF;
	static unsigned long averageMicrosB;
	static unsigned long oldAverageMicrosF;
	static unsigned long oldAverageMicrosB;
	static float distance;
	
	static unsigned long sumMicrosF;
	static unsigned long sumMicrosB;
	
	sumMicrosF = 0;
	sumMicrosB = 0;
	
	for(i = 0; i <= ULTRASONIC_VALUES; i++)
	{
		sumMicrosF = sumMicrosF + ultrasonicValuesF[i];
		sumMicrosB = sumMicrosB + ultrasonicValuesB[i];
	}
	
	sumMicrosF = sumMicrosF + oldAverageMicrosF;
	sumMicrosB = sumMicrosB + oldAverageMicrosB;
	
	averageMicrosF = sumMicrosF/(ULTRASONIC_VALUES + 1);
	averageMicrosB = sumMicrosB/(ULTRASONIC_VALUES + 1);
	
	oldAverageMicrosF = averageMicrosF;
	oldAverageMicrosB = averageMicrosB;
	
	distance = (( 48.0 - ((float)averageMicrosF * INCH_PER_MIRCOSECONDS) - INCH_FROM_ULTRA_TO_CENTER ) + ( ((float)averageMicrosF * INCH_PER_MIRCOSECONDS) + INCH_FROM_ULTRA_TO_CENTER ))/2.0;
	
	return(distance);
}



float read_dist_simple()
{
    
    float sumMicrosF = 0;
	
	for(i = 0; i <= ULTRASONIC_VALUES; i++)
	{
		sumMicrosF = sumMicrosF + ultrasonicValuesF[i];
	}
	
	float averageMicrosF = sumMicrosF/(ULTRASONIC_VALUES);
    
	//return(averageMicrosF);
    return(simpleUltrasonicTime);
}



float ir_front_percent()
{
    //return values between 0-100 for percent of IR seen
    
    static float voltageHigh = 2.5;
    static float voltageLow = 0.6;
    
    float numHigh = ((2^12)-1)*(voltageHigh/3.3);
    float numLow = ((2^12)-1)*(voltageLow/3.3);
    
    float rate = 100/(numLow - numHigh);

    float offset = 100 - (rate*numLow);

    float percent = ADC1BUF0*rate + offset;
    
    return(percent);
}



float ir_back_percent()
{
    //return values between 0-100 for percent of IR seen
    
    static float voltageHigh = 2.5;
    static float voltageLow = 0.6;
    
    float numHigh = ((2^12)-1)*(voltageHigh/3.3);
    float numLow = ((2^12)-1)*(voltageLow/3.3);
    
    float rate = 100/(numLow - numHigh);

    float offset = 100 - (rate*numLow);

    float percent = ADC1BUF0*rate + offset;
    
    return(percent);
}



int ir_front_found()
{
    if((-0.0298*ADC1BUF0 + 122.1) >= IR_FOUND_THRESH)
    {
        return(1);
    }
    else
    {
        return(0);
    }
}



int ir_back_found()
{
    if((-0.0298*ADC1BUF1 + 122.1) >= IR_FOUND_THRESH)
    {
        return(1);
    }
    else
    {
        return(0);
    }
}



float abs_f(float value)
{
	if(value >= 0)
		return(value);
	else
		return(-1.0*value);
}



int go_straight_inches(float inches)
{
    
    static float numberOfSteps;
    static float stepsTaken = 0;
    static long lastTime = 0;

    numberOfSteps = (abs_f(inches)*stepsPerInch)*2.0;	//convert inches to number of steps

    if(inches >= 0)
    {
        _RB7 = 0;  //set direction pins, both forward, DIR-R
        _RB8 = 1;  //set direction pins, both forward, DIR-L
    }

    else
    {
        _RB7 = 1;  //set direction pins, both backward, DIR-R
        _RB8 = 0;  //set direction pins, both backward, DIR-L
    }

    if(stepsTaken < numberOfSteps)  //Not enough steps yet...
    {
        if((milliseconds - lastTime) <= STEP_DELAY)  //Not yet waited long enough
        {
            //do nothing YET...
        }
        else    //Its time to step...
        {
            if(_RB15)   //STEP
            {
                _RB15 = 0;  //STEP
            }
            else
            {
                _RB15 = 1;  //STEP
            }
            
            lastTime = milliseconds;
            stepsTaken++;
        }
        return(0);  //Need to continue
    }
    else    //We've taken enough steps
    {
        stepsTaken = 0; //Reset steps taken for next call
        return(1);  //We're done...
    }
}



int turn_degrees(float degrees)
{
    //static float stepsPerDegree = 1.055; //Number of pulses required to spin 1 degrees
    static float numberOfSteps;   //Number of steps to reach the desired degrees
    numberOfSteps = (abs_f(degrees) * stepsPerDegree)*2; // This rounds to lower integer, right? - David
    static float stepsTaken = 0;
    static long lastTime = 0;

    if(degrees >= 0)    //Turn CW
    {
        _RB7 = 1;  //set DIR-R backward
        _RB8 = 1;  //set DIR-L forward
    }
    else
    {
        _RB7 = 0;  //set DIR-R forward
        _RB8 = 0;  //set DIR-L backward
    }

    if(stepsTaken <= numberOfSteps)  //Not enough steps yet...
    {
        if((milliseconds - lastTime) <= STEP_DELAY)  //Not yet waited long enough
        {
            //do nothing YET...
        }
        else    //Its time to step...
        {
            if(_RB15)
            {
                _RB15 = 0;
            }
            else
            {
                _RB15 = 1;
            }
            //toggle(PIN_STEP_L); //Change the value from 1->0 or visa-versa
            
            lastTime = milliseconds;
            stepsTaken++;
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
	static int state = 0;
	static float dist1;
	static float dist2;
	static int dirFlag = 0;	//CW rotation initially...
	int returnFlag = 0;
		
	switch(state)
	{
		case 0:	//Take initial reading...
			dist1 = ultra_avg();;	//only make reading once...
			state = 1;
			break;
		
		case 1:	//Turn... then Measure & Determine Direction...
			if(turn_degrees(LARGE_STEP_ANGLE))	//turn 2 times as far the first time.
			{
                dist2 = ultra_avg();
				//dist2 = read_dist_simple();
                //dist2 = analog_ultra_inches();	//take reading once we've turned a full angle...
				
				if(dist2 >= dist1)	//turned wrong direction
				{
					dirFlag = 1;	//CCW rotation
				}
				else
				{
					dirFlag = 0;	//CW rotation
				}
				dist1 = dist2;	//update dist1
				
				state = 2;
			}
			break;
		
		case 2:	//Turn...then Measure & Determine if normal...
			if(dirFlag = 1)	//CCW rotation, correct direction
			{
				if(turn_degrees(-1*STANDARD_STEP_ANGLE))
				{
                    dist2 = ultra_avg();
					//dist2 = read_dist_simple();
                    //dist2 = analog_ultra_inches();	//take a new measurement
					if(dist2 >= dist1)	//we're getting farther again...
					{
						state = 3;	//normal, move on...
					}
					else
					{
						state = 2;	//not normal, keep turning
					}
					dist1 = dist2;	//update dist1
				}
				else
				{
					//do nothing, still turning...
				}
			}
			else if(dirFlag = 0)	//CW rotation, correct direction
			{
				if(turn_degrees(STANDARD_STEP_ANGLE))
				{
                    dist2 = ultra_avg();
					//dist2 = read_dist_simple();
                    //dist2 = analog_ultra_inches();
					if(dist2 >= dist1)	//we're getting farther again...
					{
						state = 3;	//normal, move on...
					}
					else
					{
						state = 2;	//not normal, keep turning
					}
					dist1 = dist2;	//update dist1
				}
				else
				{
					//do nothing, still turning...
				}
			}
			break;
		
		case 3:	//Turn back to estimated "more" normal...
			if(dirFlag = 1)	//Turn back CW (previously rotated CCW)
			{
				if(turn_degrees(STANDARD_STEP_ANGLE*(dist2/(dist1+dist2))))	//finished turning back... turn back an angle proportional to distances...
				{
					state = 0;	//reset state
					returnFlag = 1;	//prepare to confirm normal
				}
			}
			else if(dirFlag = 0)	//Turn back CCW (previously rotated CW)
			{
				if(turn_degrees(-1*STANDARD_STEP_ANGLE*(dist2/(dist1+dist2))))	//finished turning back... turn back an angle proportional to distances...
				{
					state = 0;	//reset state
					returnFlag = 1;	//prepare to confirm normal
				}
			}
			break;
	}

	return(returnFlag);		
}



int find_normal_analog()
{
	static int state = 0;
	static float dist1;
	static float dist2;
	static int dirFlag = 0;	//CW rotation initially...
	int returnFlag = 0;
		
	switch(state)
	{
		case 0:	//Take initial reading...
			dist1 = analog_ultra_inches();	//only make reading once...
			state = 1;
			break;
		
		case 1:	//Turn... then Measure & Determine Direction...
			if(turn_degrees(LARGE_STEP_ANGLE))	//turn 2 times as far the first time.
			{
				//dist2 = read_dist_simple();
                dist2 = analog_ultra_inches();	//take reading once we've turned a full angle...
				
				if(dist2 >= dist1)	//turned wrong direction
				{
					dirFlag = 1;	//CCW rotation
				}
				else
				{
					dirFlag = 0;	//CW rotation
				}
				dist1 = dist2;	//update dist1
				
				state = 2;
			}
			break;
		
		case 2:	//Turn...then Measure & Determine if normal...
			if(dirFlag = 1)	//CCW rotation, correct direction
			{
				if(turn_degrees(-1*STANDARD_STEP_ANGLE))
				{
					//dist2 = read_dist_simple();
                    dist2 = analog_ultra_inches();	//take a new measurement
					if(dist2 >= dist1)	//we're getting farther again...
					{
						state = 3;	//normal, move on...
					}
					else
					{
						state = 2;	//not normal, keep turning
					}
					dist1 = dist2;	//update dist1
				}
				else
				{
					//do nothing, still turning...
				}
			}
			else if(dirFlag = 0)	//CW rotation, correct direction
			{
				if(turn_degrees(STANDARD_STEP_ANGLE))
				{
					//dist2 = read_dist_simple();
                    dist2 = analog_ultra_inches();
					if(dist2 >= dist1)	//we're getting farther again...
					{
						state = 3;	//normal, move on...
					}
					else
					{
						state = 2;	//not normal, keep turning
					}
					dist1 = dist2;	//update dist1
				}
				else
				{
					//do nothing, still turning...
				}
			}
			break;
		
		case 3:	//Turn back to estimated "more" normal...
			if(dirFlag = 1)	//Turn back CW (previously rotated CCW)
			{
				if(turn_degrees(STANDARD_STEP_ANGLE*(dist2/(dist1+dist2))))	//finished turning back... turn back an angle proportional to distances...
				{
					state = 0;	//reset state
					returnFlag = 1;	//prepare to confirm normal
				}
			}
			else if(dirFlag = 0)	//Turn back CCW (previously rotated CW)
			{
				if(turn_degrees(-1*STANDARD_STEP_ANGLE*(dist2/(dist1+dist2))))	//finished turning back... turn back an angle proportional to distances...
				{
					state = 0;	//reset state
					returnFlag = 1;	//prepare to confirm normal
				}
			}
			break;
	}

	return(returnFlag);		
}



/*
int find_normal()
{
	static int statFlag = 0;
	static int dirFlag = 0;
	//static float dist1 = 24.0;	//Dist1 = Measure distance
    static float dist2;	//Dist2 = Measure distance
	
	if(dirFlag == 0)    //turn cw
	{
		turn_degrees(5.0);	//Turn
	}
	else    //turn ccw
	{
		turn_degrees(-5.0);
	}
	
	
	//dist2 = read_dist_simple();
	dist2 = analog_ultra_inches();
	
	if((dist2 >= dist1) && (statFlag == 0))	//If (Dist2 > Dist1 and Flag = 0)	//Going wrong direction, first time through
	{
		if(dirFlag == 1)	//	Go the other way?
		{
			dirFlag = 0;
		}
		else
		{
			dirFlag = 1;
		}
		dist1 = dist2;
		return(0);	//keep searching
	}
    
	else if(dist1 >= dist2)	//Going right direction
	{
		//	Continue turning in that direction
		statFlag = 1;		//	Flag = 1
        dist1 = dist2;
		return(0);	// keep searching
	}
    
	else if((dist2 > dist1) && (statFlag == 1))		//Else if(Dist2 > Dist1 && Flag==1)	//Reached the minimum
	{
		if(dirFlag == 0)
		{
			turn_degrees(-1*STANDARD_STEP_ANGLE/2.0);	//	Go back half a step in angle
		}
		else
		{
			turn_degrees(STANDARD_STEP_ANGLE/2.0);	//	Go back half a step in angle
		}
		dist1 = dist2;
        statFlag = 0;	//	Flag = 0
		return(1);	//	Return(1)
		        
	}
}
*/



int find_inches(float refence)
{
	float error;
		
	error = (reference -  analog_ultra_inches());	//Error = (24 - Dist)
		
	if(abs_f(error) < DIST_TOL)	//We are there...
	{
		return(1);	//Center Found
	}
	
	else	//Change position...
	{
		if(error > 0)	//Positive error, move backward
		{
			go_straight_inches(-1*STANDARD_STEP_LENGTH); // If our DIST_TOL is 0.1 in, how can we reach it taking 0.5 in steps? Reduce to 0.1 in, maybe? - David
		}
		else	//Negative error, move forward
		{
			go_straight_inches(STANDARD_STEP_LENGTH);
		}
		
		return(0);	//Keep looking for center
	}
}



int find_24()
{
	float error;
		
	error = (INCH_TO_WALL -  analog_ultra_inches());	//Error = (24 - Dist)
		
	if(abs_f(error) < DIST_TOL)	//We are there...
	{
		return(1);	//Center Found
	}
	
	else	//Change position...
	{
		if(error > 0)	//Positive error, move backward
		{
			go_straight_inches(-1*STANDARD_STEP_LENGTH); // If our DIST_TOL is 0.1 in, how can we reach it taking 0.5 in steps? Reduce to 0.1 in, maybe? - David
		}
		else	//Negative error, move forward
		{
			go_straight_inches(STANDARD_STEP_LENGTH);
		}
		
		return(0);	//Keep looking for center
	}
}



int find_24_analog()
{
	static float error;
		
	error = (INCH_TO_WALL -  analog_ultra_inches());	//Error = (24 - Dist)
	//TO DO: write a read_dist() function. This should return a float which is a filtered value representing the distance from the wall
		
	if(abs_f(error) < DIST_TOL)	//We are there...
	{
		return(1);	//Center Found
	}
	
	else	//Change position...
	{
		if(error > 0)	//Positive error, move backward
		{
			go_straight_inches(-1*STANDARD_STEP_LENGTH); // If our DIST_TOL is 0.1 in, how can we reach it taking 0.5 in steps? Reduce to 0.1 in, maybe? - David
		}
		else	//Negative error, move forward
		{
			go_straight_inches(STANDARD_STEP_LENGTH);
		}
		
		return(0);	//Keep looking for center
	}
}



void ultrasonic_setup()
{
	// Configure CN interrupt on Pin2 and Pin3
    _CN6IE = 1;     // Enable CN on pin 6 (CNEN1 register)
    _CN6PUE = 0;    // Disable pull-up resistor (CNPU1 register)
    _CN30IE = 1;     // Enable CN on pin 7 (CNEN1 register)
    _CN30PUE = 0;    // Disable pull-up resistor (CNPU1 register)
    _CNIP = 4;      // Set CN interrupt priority (IPC4 register)
    _CNIF = 0;      // Clear interrupt flag (IFS1 register)
    _CNIE = 1;      // Enable CN interrupts (IEC1 register)
	
	ultraLastStateF = _RB2; //Current state of ultrasonic
	ultraLastStateB = _RA2; //Current state of ultrasonic
	
	for(i = 0; i <= ULTRASONIC_VALUES; i++)
	{
		ultrasonicValuesB[i] = 2000;	//Start with an array of 2 milliseconds times
		ultrasonicValuesF[i] = 2000;	//Start with an array of 2 milliseconds times
	}

    // CONFIGURE PWM2 (on pin 4) USING TIMER2
    // Configure Timer2, which will be the source for the output compare that
    // drives Ultrasonic_Front
    T2CONbits.TON = 0;      // Timer2 off while configuring PWM, pg. 144
    T2CONbits.TCKPS = 0b00;    // Timer2 1:1 clock pre-scale, pg. 144
    T2CONbits.T32 = 0;      // Timer2 acts as a single 16-bit timer, pg. 144
    T2CONbits.TCS = 0;      // Timer2 internal clock source, pg. 144

    // Configure PWM2 on OC1 (pin 4)										-- Is this supposed to be PWM2 on pin 4? - David
    OC2CON1 = 0b0000;               // Clear OC2 configuration bits, Table 4-8
    OC2CON2 = 0b0000;               // Clear OC2 configuration bits, Table 4-8
    OC2CON1bits.OCTSEL = 0b000;       // Use Timer2, pg. 157
    OC2CON1bits.OCM = 0b110;          // Edge-aligned PWM mode, pg. 158
    OC2CON2bits.SYNCSEL = 0b01100;    // Use Timer2, pg. 160

    // Set period and duty cycle
    PR2 = 25000;            // Period of Timer2 = 50ms
                            // See Equation 15-1 in the data sheet
    OC2R = 5;               // On time of 10us

    // Turn on Timer2
    T2CONbits.TON = 1;
	
	// CONFIGURE PWM3 (on pin 5) USING TIMER2
    // Configure Timer3, which will be the source for the output compare that
    // drives Ultrasonic_Front													
    T3CONbits.TON = 0;      	// Timer3 off while configuring PWM, pg. 144
    T3CONbits.TCKPS = 0b00;    	// Timer3 1:1 clock pre-scale, pg. 144
    //T3CONbits.T32 = 0;      	// Timer3 acts as a single 16-bit timer, pg. 144
    T3CONbits.TCS = 0;      	// Timer3 internal clock source, pg. 144

    // Configure PWM3 on OC1 (pin 5)											
    OC3CON1 = 0b0000;               	// Clear OC3 configuration bits, Table 4-8
    OC3CON2 = 0b0000;               	// Clear OC3 configuration bits, Table 4-8
    OC3CON1bits.OCTSEL = 0b001;       	// Use Timer3, pg. 157
    OC3CON1bits.OCM = 0b110;          	// Edge-aligned PWM mode, pg. 158
    OC3CON2bits.SYNCSEL = 0b01101;    	// Unsyncronized, pg. 160

    // Set period and duty cycle
    PR3 = 25000;         	// Period of Timer3 = 50ms
                            // See Equation 15-1 in the data sheet
    OC3R = 5;               // On time of 10us

    // Turn on Timer2
    T3CONbits.TON = 1;
    
    _TRISB0 = 0;
    _TRISB1 = 0;
    
    ANSB = 0;
    
    TMR2 = 0;
    TMR3 = 12500;
	
}



void loader_finder_digital_setup()
{
    //TODO: Add all configurations needed to change pin to digital input...
    //      as well as anything else to make the switch between analog and digital
    
	// Configure CN interrupt on Pin2 and Pin3
    _CN3IE = 1;     // Enable CN on pin 6 (CNEN1 register)
    _CN3PUE = 0;    // Disable pull-up resistor (CNPU1 register)
    _CNIP = 4;      // Set CN interrupt priority (IPC4 register)
    _CNIF = 0;      // Clear interrupt flag (IFS1 register)
    _CNIE = 1;      // Enable CN interrupts (IEC1 register)
	
	loaderIrState = _RA1;   //IR Back
	
	for(i = 0; i <= IR_TIMES; i++)
	{
		irTimeValues[i] = 0;	//Start with a blank array
	}
    
    ANSA = 0;       //Turn off analog for port A
    _TRISA0 = 1;    //Pin2 as Input
    _TRISA1 = 1;    //Pin3 as Input
}



void ir_finder_analog_setup()
{
    //TODO: All configurations needed to switch between digital interrupt to
    //      analog input
    
    _CN3IE = 0;     // Disable CN on pin 6 (CNEN1 register)
    
    //------------------------------------------------------------------------
    // A /D Configuration Function
    //
    // This function configures the A/D to read from two channels in auto
    // conversion mode.
    //------------------------------------------------------------------------
    
    // AD1CHS register
    _CH0NA = 0;         // AD1CHS<7:5> -- Use VDD as negative input

    // AD1CON1 register
    _ADON = 1;          // AD1CON1<15> -- Turn on A/D
    _ADSIDL = 0;        // AD1CON1<13> -- A/D continues while in idle mode?
    _MODE12 = 1;        // AD1CON1<10> -- 12-bit or 10-bit?
    _FORM = 0b00;       // AD1CON1<9:8> -- Output format, pg. 211
    _SSRC = 0b0111;     // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;          // AD1CON1<2> -- Auto sampling

    // AD1CSSL registers
    AD1CSSL = 0XFFFF;   // AD1CSSL<15:0> -- Select lower channels to scan, turn on AN0 and AN1
    AD1CSSH = 0x0000;               // AD1CSSH<15:0> -- Select upper channels to scan, NOT USED!

    // AD1CON2 register, see pg. 212
    _PVCFG = 0b00;      // AD1CON2<15:14> -- Use VDD as positive ref voltage
    _NVCFG = 0;         // AD1CON2<13> -- Use VSS as negative ref voltage
    _BUFREGEN = 1;      // AD1CON2<11> -- Results stored using channel indexed
                        // mode -- AN1 result is stored in ADC1BUF1, AN2 result
                        // is stored in ADC1BUF2, etc.
    _CSCNA = 1;         // AD1CON2<10> -- Scans inputs specified in AD1CSSx
                        // registers instead of using channels specified
                        // by CH0SA bits in AD1CHS register
    _ALTS = 0;          // AD1CON2<0> -- Sample MUXA only
    _SMPI = 0b00001;    // AD1CON2<6:2> -- Interrupts at the conversion for
                        // every other sample

    // AD1CON3 register
    _ADRC = 0;              // AD1CON3<15> -- Use system clock
    _SAMC = 0x00001;        // AD1CON3<12:8> -- Auto sample every A/D period TAD
    _ADCS = 0x00111111;     // AD1CON3<7:0> -- A/D period TAD = 64*TCY

    ANSA = 1;       //Turn on analog for port A
    _TRISA0 = 1;    //Pin2 as Input
    _TRISA1 = 1;    //Pin3 as Input
}



void _ISR _CNInterrupt(void)    //Interrupt
{
    _CNIF = 0;      // Clear interrupt flag (IFS1 register)
    
    int RB2 = _RB2;
    int RA2 = _RA2;
    int RA1 = _RA1;
    
	
	timeTemp = microseconds;
    timeTempMillis = milliseconds;
	
    if(RA1 != loaderIrState) //Rear (towards the Loader) state has changed
    {
        loaderIrState = RA1;
        
        for(i = 0; i < IR_TIMES; i++)
			{
				irTimeValues[i] = irTimeValues[i+1];	//Replace current value with next value...
			}
		irTimeValues[IR_TIMES] = (timeTempMillis);
    }
        
	else if(RA2 != ultraLastStateB)	//The Rear Ultrasonic Pulse Train Just Changed
	{
        ultraLastStateB = RA2;
        
		if(RA2)	//Rear Ultrasonic Pulse Just Started
		{
			startTimeUltraB = timeTemp;
		}
		else	//Rear Ultrasonic Pulse Just Ended
		{
			for(i = 0; i < ULTRASONIC_VALUES; i++)
			{
				ultrasonicValuesB[i] = ultrasonicValuesB[i+1];	//Replace current value with next value...
			}
			ultrasonicValuesB[ULTRASONIC_VALUES] = (timeTemp - startTimeUltraB);
		}
	}
	
	else if(RB2 != ultraLastStateF)	//The Front Ultrasonic Pulse Train Just Changed
	{
        ultraLastStateF = RB2;
        
		if(RB2)	//Front Ultrasonic Pulse Just Started
		{
			startTimeUltraF = timeTemp;
		}
		else	//Front Ultrasonic Pulse Just Ended
		{
			simpleUltrasonicTime = ((timeTemp - startTimeUltraF) + simpleUltrasonicTime)/2;
            
			for(i = 0; i < ULTRASONIC_VALUES; i++)
			{
				ultrasonicValuesF[i] = ultrasonicValuesF[i+1];	//Replace current value with next value...
			}
			ultrasonicValuesF[ULTRASONIC_VALUES] = (timeTemp - startTimeUltraF);
            
		}
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

