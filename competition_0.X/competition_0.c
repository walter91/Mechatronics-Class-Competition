#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

unsigned long milliseconds = 0; //Will run for 48+ days before overflow...
unsigned long microseconds = 0;	//Will overflow after 71 minutes
unsigned long countTimer = 0;
//unsigned long countTimerMilli = 0; 

unsigned long startTimeUltraF;
unsigned long startTimeUltraB;

unsigned long stopTimeUltraF;
unsigned long stopTimeUltraB;

unsigned long timeTemp;
unsigned long timeTempMillis;

int ultraLastStateB;
int ultraLastStateF;

int loaderIrState;
#define IR_TIMES 5
unsigned long irTimeValues[IR_TIMES];




#define ULTRASONIC_VALUES 10

unsigned long ultrasonicValuesB[ULTRASONIC_VALUES];
unsigned long ultrasonicValuesF[ULTRASONIC_VALUES];

int i;
unsigned long startTime;
#include "board.h" //set up pin names, hold all additional functions

typedef enum {findCenter, findLoader, toLoader, loading, toShooting, findTarget, shooting, end} state;	//Initialize all states...
state STATE = findLoader;	//Default state initialization

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

int main()
{	
    
    
	timing_interrupt_config();
	ultrasonic_setup();
	
	int findCenterState = 0;
	int findLoaderState = 0;
	int toLoaderState = 0;
	int loadingState = 0;
	int toShootingState = 0;
	int findTargetState = 0;
	int shottingState = 0;

	while(1)
	{
		switch(STATE)
		{
            case findCenter:
				switch(findCenterState)
				{
					case 0:
						if(find_normal())
						{
							findCenterState = 1;
						}
						else
						{
							//do nothing
						}
						break;
					case 1:
						if(find_24())
						{
							findCenterState = 2;
						}
						else
						{
							//do nothing
						}
						break;
					case 2:
						if(turn_degrees(90))
						{
							findCenterState = 3;
						}
						else
						{
							//do nothing
						}
						break;
					case 3:
						if(find_24())
						{
							findCenterState = 0;
							STATE = findLoader;
						}
						else
						{
							//do nothing
						}
						break;
				}
				break;
            
			case findLoader:
				switch(findLoaderState)
				{
					case 0:	//Default, turn 45 degrees...
						if(turn_degrees(45))
						{
							findLoaderState = 1;
							startTime = milliseconds;
							while((milliseconds - startTime) <= ((5.0 + 2.0)/LOADING_IR_FREQ)*1000.0);	//Pause and allow for the IR sensor to adjust
						}
						else
						{
							//do nothing
						}
						break;
					case 1:	//Turn completed...
						if((milliseconds - irTimeValues[0]) < ((5.0 + 2.0)/LOADING_IR_FREQ)*1000.0)	//The array of 5 values has been filled recently by interrupts (IR Beacon Found)
						{
							findLoaderState = 0;
							STATE = toLoader;
						}
						else
						{
							findLoaderState = 2;
						}
						break;
					case 2: //IR Not found, continue turning...
						if(turn_degrees(90))
						{
							findLoaderState = 1;
							startTime = milliseconds;
							while((milliseconds - startTime) <= ((5.0 + 2.0)/LOADING_IR_FREQ)*1000.0);	//Pause and allow for the IR sensor to adjust
						}
						else
						{
							//do nothing
						}
						break;
				}
				break;
				
			case toLoader:
				switch(toLoaderState)
				{
					case 0:
						if(go_straight_inches(-1*INCHES_CORNER_TO_CENTER))
							STATE = loading;
						break;
				}
				break;
				
			case loading:
				//this still needs to have the details worked out...
				//If loaded with 6, STATE = toShooting, Break
				//Else, do nothing (wait for loading)
				
				STATE = toShooting;
				break;
				
			case toShooting:
				switch(toShootingState)
				{
					case 0:
						if(go_straight_inches(INCHES_CORNER_TO_CENTER))
							STATE = findTarget;
						break;
				}
				break;
				
			case findTarget:
				//Spin around by turning on one motor forward and another motor backwards
				//Check for the IR-LED signature
				//If the IR-LED is found, then turn off motors, STATE = shooting
				break;
				
			case shooting:
				//call shoot() function 6 times
				//function should handle all coordination of solenoids
				//count number of shoot() calls
				
				//STATE = findLoader
				break;
				
			case end:
				//do nothing
				break;
				
			default:
				//Not sure what to do yet. This means there is an error somewhere. Maybe turn on an LED?...
				break;			
		}
	}
    return(0);
}