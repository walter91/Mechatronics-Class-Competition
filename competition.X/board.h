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

_FICD(ICS_PGx3);    //Debug Using programming module 3

//Values
#define LEFT_FORWARD 1
#define RIGHT_FORWARD 0

#define LEFT_BACKWARD 1
#define RIGHT_BACKWARD 0

#define STEP_DELAY 5 // Is this value arbitrary... but it works well

#define INCH_TO_WALL 24.0
#define INCH_FROM_ULTRA_TO_CENTER 4.5

#define DIST_TOL 0.25

#define LARGE_STEP_LENGTH 0.50
#define STANDARD_STEP_LENGTH 0.25

#define LARGE_STEP_ANGLE 10.0
#define STANDARD_STEP_ANGLE 5.0


//Half Step mode
#define stepsPerRev 400.0 //Number of steps required for one rev of motor
#define wheelCircumferenceInches 12.7172 //4.048*3.14	//Drive wheel circumference in inches
#define stepsPerInch  15.6363 // Calibrated //31.4534 //15.7267 //(stepsPerRev/wheelCircumferenceInches)	//Number of pulses required to move forward 1 inch (pulses/revolution)*(revolutions/inch)
#define stepsPerDegree 0.84553 //calibrated //.836 //1.672 // stepsperInch*0.0266*2 //was 10.055

#define LOAD_TIME .2    //Seconds for a PPball to fall past the gate...
#define SHOOT_TIME .2   //Seconds for a PPball to be shot...

#define INCH_PER_VOLT 20.825    //Calibrated (11-24-15)
#define INCH_OFFSET 0.915   //Calibrated (11-24-15)

#define LOADING_IR_FREQ 100

#define INCHES_CORNER_TO_CENTER 33.3

#define IR_FOUND_THRESH 70  //Percent of full voltage

#define INCH_PER_MIRCOSECONDS .00676

#define CHECK_BIT(var,pos) !!((var) & (1<<(pos)))

unsigned long milliseconds = 0; //Will run for 48+ days before overflow...
unsigned long microseconds = 0;	//Will overflow after 71 minutes
unsigned long countTimer = 0;
unsigned long countTimerDist = 0;

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

float ultraVoltsInt[10] = {0,0,0,0,0,0,0,0,0,0};	//Interrupt driven Analog Ultrasonic Values
float ultraInchesAvg[10] = {0,0,0,0,0,0,0,0,0,0};

int loaderIrState;
#define IR_TIMES 3
unsigned long irTimeValues[IR_TIMES];

#define ULTRASONIC_VALUES 10

unsigned long ultrasonicValuesB[ULTRASONIC_VALUES];
unsigned long ultrasonicValuesF[ULTRASONIC_VALUES];

int rightStage, leftStage, straightStage;


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



int stage_timer()
{
	if(milliseconds < 10000)	//First 10 seconds...
	{
		return(0);
	}
	else if(milliseconds < 40000)	//First shooting...
	{
		return(1);
	}
	else if(milliseconds < 70000)	//Second shooting...
	{
		return(2);
	}
	else if(milliseconds < 100000)	//Third shooting...
	{
		return(3);
	}
	else if(milliseconds < 105000)	//Ending...
	{
		return(4);
	}
	else	//It's Over...
	{
		return(5);
	}
}



int stage_change()
{
	static int stage = 0;
	
	int newStage = stage_timer();
	
	if(newStage != stage)
	{
		stage = newStage;
		return(1);
	}
	else
	{
		return(0);
	}
}



void pin_config_init()
{
    
    //TO DO: Update pin out
    
	//_TRISA5 = 0;	//LOADING SWEEPER, PIN1
    
    // Pin 5: loading sweeper, latb1, see loading_timer()
	
    _TRISA0 = 1;    // Ultrasonic, PIN2
    _ANSA0 = 1;     // ADC1BUF0
    _TRISA1 = 1;    // IR FRONT, PIN3
    _ANSA1 = 1;     // ADC1BUF1
    _TRISB0 = 1;    // IR BACK, Pin 4
    _ANSB0 = 1;     // ADC1BUF2

    
    //_TRISB0 = 0;    //FRONT TRIGGER, PIN4
    _TRISB1 = 0;    //BACK TRIGGER, PIN5
    
    _TRISB2 = 1;    //ULTRASONIC ECHO FRONT/ULTRASONIC ANALOG, PIN6
    _TRISA2 = 1;    //ULTRASONIC ECHO BACK, PIN7
    
    _TRISA3 = 1;    //LOADING SENSOR INPUT, PIN8
    
    _TRISB7 = 0;    //DIR-RIGHT, PIN11
    _TRISB8 = 0;    //DIR-LEFT, PIN12
    _TRISB15 = 0;   //STEP, PIN18
    _ANSB15 = 0;
    
   
    
    _TRISB9 = 0;    //LOADER TOP, PIN13
    _TRISA6 = 0;    //SHOOTER, PIN14
    _TRISB12 = 0;   //LOADER BOTTOM, PIN15
    
    
    //----------- ADC Config-----------//
    
     // AD1CON1 register
    _ADON = 0;          // AD1CON1<15> -- Turn off A/D
    _ADSIDL = 0;        // AD1CON1<13> -- A/D continues while in idle mode
    _MODE12 = 1;        // AD1CON1<10> -- 12-bit A/D operation
    _FORM = 0;          // AD1CON1<9:8> -- Unsigned integer output
    _SSRC = 7;          // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;          // AD1CON1<2> -- Auto sampling

    // AD1CSSL register
    //AD1CSSL = 0;        // AD1CSSL<15:0> -- Skip all channels on input scan --
                        // see the CSCNA bits in AD1CON2
    _CSS0 = 1;          // Pin 2 = ADC1BUF0
    _CSS1 = 1;          // Pin 3= ADC1BUF1
    _CSS2 = 1;          // Pin 4= ADC1BUF2

    // AD1CON2 register
    _PVCFG = 0;         // AD1CON2<15:14> -- Use VDD as positive ref voltage
    _NVCFG = 0;         // AD1CON2<13> -- Use VSS as negative ref voltage
    _CSCNA = 1;         // AD1CON2<10> -- Does scan inputs specified in
                        // AD1CSSx registers (instead uses channels specified
                        // by CH0SA bits in AD1CHS register) -- Selecting '0'
                        // here probably makes writing to the AD1CSSL register
                        // unnecessary.
    //_ALTS = 0;          // AD1CON2<0> -- Sample MUXA only
    _SMPI = 0b10;       // Set channels to scan = 3

    // AD1CON3 register
    _ADRC = 0;          // AD1CON3<15> -- Use system clock
    _SAMC = 1;          // AD1CON3<12:8> -- Auto sample every A/D period TAD
    _ADCS = 0x3F;       // AD1CON3<7:0> -- A/D period TAD = 64*TCY
    
    _ADON = 1;          // AD1CON1<15> -- Turn on A/D
    
}


int shoot(int rounds)
{
    static int shootState = 0;
    static int roundsFired = 0;
    static int returnStatus = 0;
    static unsigned long shootStartTime;
    
    switch(shootState)
    {
        case(0):
            shootStartTime = milliseconds;
            //TO DO: actuate lower gate
            shootState = 1;
            returnStatus = 0;
            break;
        
        case(1):    //Load one ball into barrel (open lower gate for LOAD_TIME seconds)
            if((milliseconds - shootStartTime) >= LOAD_TIME)    //Timer is expired
            {
                shootState = 2; //Move on to shooting
                //TODO: turn off the lower gate
                _RA6 = 1; // Actuate the shooting solenoid
                shootStartTime = milliseconds;
                delay(250); // Short delay to allow full extention/strength of solenoid
            }
            else
            {
                //wait for the timer to expire
            }
            returnStatus = 0;
            break;
            
        case(2):    //Fire one ball (actuate shooter for SHOOT_TIME seconds)
            if((milliseconds - shootStartTime) >= SHOOT_TIME)    //Timer is expired
            {
                shootState = 3; //Move on to shooting
                _RA6 = 0; //TODO: turn off the shooting solenoid
                //TODO: open the lower gate
                shootStartTime = milliseconds;
                delay(250); 
            }
            else
            {
                //wait for the timer to expire
            }           
            returnStatus = 0;
            break;
            
        case(3):    //Load one ball into holding chamber (open upper gate for LOAD_TIME seconds)
            if((milliseconds - shootStartTime) >= LOAD_TIME)    //Timer is expired
            {
                shootState = 4; //Move on to shooting
                //TODO: close the lower gate
            }
            else
            {
                //wait for the timer to expire
            }
            
            returnStatus = 0;
            break;
            
        case(4):            
            if(roundsFired < rounds)
            {
                returnStatus = 0;   //We've not shot enough, return for more...
            }
            else
            {
                returnStatus = 1;   //We've shot enough, go out...
            }
            shootState = 0;
            break;
    }
    return(returnStatus);
}



int loading_timer(unsigned long waitTime)
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
			_LATB1 = 1;	//turn on loading sweeper, PIN1
			break;
		case 1:
			if((milliseconds - startWaitTime) >= waitTime)	//Timer has expired
			{
				state = 0;
				returnFlag = 1;
				_LATB1 = 0;	//turn off loading sweeper, PIN1
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



float analog_ultra_inches() //Returns average of last ten distance readings...
{  
	static float ultraVolts;
	ultraVolts = (ADC1BUF0/4095.0)*3.3;	//Read analog-to-digital converter and save in volts
    
    for(i=0; i <10; i++)	//For 0-9 (all 10)
	{
		ultraInchesAvg[i] = ultraInchesAvg[i+1];    //Shift the inch values...
	}
    
	//static float inches;
	ultraInchesAvg[10] = ultraVolts*INCH_PER_VOLT + INCH_OFFSET;    //Update the final value in the array
    
    static float sum;
	sum = 0;
	
	for(i=0; i <=10; i++)	//For 0-9 (all 10)
	{
		sum = sum + ultraInchesAvg[i];
	}
    
    static float averageInches;
    averageInches = sum/10.0;
	
    return(averageInches);
}



void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    // Remember to clear the Timer1 interrupt flag when this ISR is entered.
    _T1IF = 0; // Clear interrupt flag
    milliseconds++;
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
    PR1 = 500;    // Count to 1 milli-sec at 1 mHz, instruct at 500 kHz
}






float ir_front_percent()
{
    static float irFrontPercent[6] = {0,0,0,0,0,0};
    
    
    //return values between 0-100 for percent of IR seen
    
    static float voltageLow = 1.22;
          
    float numLow;
    numLow = (4095)*(1.22/3.3);
    
    float percent;
    percent = 100.0 * ((ADC1BUF1-numLow)/(4095.0-numLow));
    
    //Update array values
    for(i = 0; i < 5; i++ )
    {
        irFrontPercent[i] = irFrontPercent[i+1];
    }
    irFrontPercent[5] = percent;
    
    //Average the array
    float sum = 0;
    for(i = 0; i<=5; i++)
    {
        sum = sum + irFrontPercent[i];
    }
    
    float averagePercent = sum/6.0;
    
    
    return(averagePercent);
}



float ir_back_percent()
{
    //return values between 0-100 for percent of IR seen
    
    static float voltageLow = 1.22;
          
    float numLow;
    numLow = (4095)*(1.22/3.3);
    
    float percent;
    percent = 100.0 * ((ADC1BUF2-numLow)/(4095.0-numLow));
    return(percent);
}


void analog_update()
{
    ir_front_percent();
    ir_back_percent();
    analog_ultra_inches();
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
    static unsigned long lastTime = 0;

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
			if(dirFlag == 1)	//CCW rotation, correct direction
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
			else if(dirFlag == 0)	//CW rotation, correct direction
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
			if(dirFlag == 1)	//Turn back CW (previously rotated CCW)
			{
				if(turn_degrees(STANDARD_STEP_ANGLE*(dist2/(dist1+dist2))))	//finished turning back... turn back an angle proportional to distances...
				{
					state = 0;	//reset state
					returnFlag = 1;	//prepare to confirm normal
				}
			}
			else if(dirFlag == 0)	//Turn back CCW (previously rotated CW)
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



int find_corner()
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
				
				if(dist2 <= dist1)	//turned wrong direction
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
			if(dirFlag == 1)	//CCW rotation, correct direction
			{
				if(turn_degrees(-1*STANDARD_STEP_ANGLE))
				{
					//dist2 = read_dist_simple();
                    dist2 = analog_ultra_inches();	//take a new measurement
					if(dist2 <= dist1)	//we're closer again...
					{
						state = 3;	//corner, move on...
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
			else if(dirFlag == 0)	//CW rotation, correct direction
			{
				if(turn_degrees(STANDARD_STEP_ANGLE))
				{
					//dist2 = read_dist_simple();
                    dist2 = analog_ultra_inches();
					if(dist2 <= dist1)	//we're getting closer again...
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
			if(dirFlag == 1)	//Turn back CW (previously rotated CCW)
			{
				if(turn_degrees(STANDARD_STEP_ANGLE*(dist2/(dist1+dist2))))	//finished turning back... turn back an angle proportional to distances...
				{
					state = 0;	//reset state
					returnFlag = 1;	//prepare to confirm normal
				}
			}
			else if(dirFlag == 0)	//Turn back CCW (previously rotated CW)
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



int find_inches(float reference)
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



/*
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
*/



int stage_when_found(int direction)
{
    static int stage;
    switch(direction)
    {
        case(0):
            stage = rightStage;
            break;
        case(1):
            stage = straightStage;
            break;
        case(2):
            stage = leftStage;
            break;
    }
    return(stage);
}



void stage_accounting(int stage, int dir)
{
    switch(dir)
    {
        case(0):
            rightStage = stage;
            break;
        case(1):
            straightStage = stage;
            break;
        case(2):
            leftStage = stage;
            break;
    }
}



int aquire_target()
{
    static int targetsFound = 0b000;
    static int state = 0;
    static int returnStatus = 0;
    
    switch(state)
    {
        case(0):    //Default (straight ahead)
            if(ir_front_percent() >= IR_FOUND_THRESH)    //This is the target...
            {
                state = 0;  //reset the state for next time
                targetsFound = targetsFound|0b010;  //Add straight to the list...
                returnStatus = 1;   //We've found it...                
            }
            else    //Not the target...
            {
                state = 1;  //Move on to another corner
                returnStatus = 0;   //Not yet found
            }
            break;
        case(1):    //It wasn't straight ahead...
            if(CHECK_BIT(targetsFound, 0))  //The right (0th bit) has previously been found...
            {
                if(stage_timer() == stage_when_found(0))   //We're in the stage
                {
                    state = 2;  //turn right and check
                    returnStatus = 0;   //Not yet found
                }
                else    //we're not in the stage anymore...
                {
                    state = 3; //don't turn right...
                    returnStatus = 0;   //Not yet found
                }
            }
            else    //Right has not previously been found...
            {
                state = 2;  //turn right and check
                returnStatus = 0;   //Not yet found
            }
            break;
        case(2):    //turn right and check
            if(turn_degrees(90))    //We've turned right...
            {
                if(ir_front_percent() >= IR_FOUND_THRESH)    //This is the target...
                {
                    state = 0;  //reset the state for next time
                    targetsFound = targetsFound|0b001;  //Add right to the list...
                    stage_accounting(stage_timer(), 0); //Take note that the right target was found while in this time stage...
                    returnStatus = 1;   //We've found it...
                }
                else    //Not the target...
                {
                    state = 5;  //It wasn't right, check left (180 CCW)
                    returnStatus = 0;   //Not yet found
                }
            }
            else
            {
                //wait until turn is complete...
            }
            break;
        case(3):    //facing straight, don't turn right...
            if(CHECK_BIT(targetsFound, 2))  //The left target (2nd bit) has previously been found...
            {
                if(stage_timer() == stage_when_found(2))   //We're in the stage
                {
                    state = 4;  //turn left (90 CCW) and check
                    returnStatus = 0;   //Not yet found
                }
                else    //we're not in the stage anymore...
                {
                    state = 7; //It wasn't straight, right has already been found, left has already been found...
                    returnStatus = 0;   //Not yet found
                }
            }
            else    //Left has not previously been found...
            {
                state = 4;  //turn left (90 CCW) and check
                returnStatus = 0;   //Not yet found
            }
            break;
        case(4):    //turn left (90 CCW) and check
            if(turn_degrees(-90))    //We've turned left...
            {
                if(ir_front_percent() >= IR_FOUND_THRESH)    //This is the target...
                {
                    state = 0;  //reset the state for next time
                    targetsFound = targetsFound|0b100;  //Add left to the list...
                    stage_accounting(stage_timer(), 2); //Take note that the left target was found while in this time stage...
                    returnStatus = 1;   //We've found it...
                }
                else    //Not the target...
                {
                    state = 7;  //It wasn't left...just checked, not straight, not right...
                    returnStatus = 0;   //Not yet found
                }
            }
            else
            {
                //wait until turn is complete...
            }
            break;        

        case(5):    //turn 180 CCW and check
            if(turn_degrees(-180))    //We've turned left...
            {
                if(ir_front_percent() >= IR_FOUND_THRESH)    //This is the target...
                {
                    state = 0;  //reset the state for next time
                    targetsFound = targetsFound|0b100;  //Add left to the list...
                    stage_accounting(stage_timer(), 2); //Take note that the left (2nd bit) target was found while in this time stage...
                    returnStatus = 1;   //We've found it...
                }
                else    //Not the target...
                {
                    state = 7;  //It wasn't left...just checked, not straight, not right...
                    returnStatus = 0;   //Not yet found
                }
            }
            else
            {
                //wait until turn is complete...
            }
            break;
        case(7):    //We're stuck, we didn't find the target...
            if(turn_degrees(90))
            {
                if(ir_front_percent() >= IR_FOUND_THRESH)    //This is the target...
                {
                    state = 0;
                    returnStatus = 1;   //We've found it...
                }
            }
            break;
    }
    
    return(returnStatus);
}



void ir_finder_analog_setup()
{
      
    //------------------------------------------------------------------------
    // A /D Configuration Function
    //
    // This function configures the A/D to read from two channels in auto
    // conversion mode.
    //------------------------------------------------------------------------
    
    _ANSA0 = 1;
    _ANSA1 = 1;
    _ANSA2 = 1;
    
    
    // AD1CHS register
    _CH0NA = 0;         // AD1CHS<7:5> -- Use VDD as negative input
    //_CH0NB = 0;
    
    //_CH0SA = 0b00011;

    // AD1CON1 register
    _ADON = 0;          // AD1CON1<15> -- Turn off A/D
    _ADSIDL = 1;        // AD1CON1<13> -- A/D continues while in idle mode?
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
                        // every sample

    // AD1CON3 register
    _ADRC = 0;              // AD1CON3<15> -- Use system clock
    _SAMC = 0b00000;        // AD1CON3<12:8> -- Auto sample every A/D period TAD
    _ADCS = 0b00111111;     // AD1CON3<7:0> -- A/D period TAD = 64*TCY
    
    _ADON = 1;          // AD1CON1<15> -- Turn on A/D

    _ANSA0;
    _ANSA1;         //Turn on analog for port A
    _TRISA0 = 1;    //Pin2 as Input
    _TRISA1 = 1;    //Pin3 as Input
}



/*
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
*/

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

