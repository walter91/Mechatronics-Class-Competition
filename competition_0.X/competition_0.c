#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

unsigned long milliseconds = 0; //Will run for 48+ days before overflow...
long startTime;
int i;

#include "motion_comp.h" //set up pin names, hold all additional functions


typedef enum {findLoader, toLoader, loading, toShooting, findTarget, shooting, end} state;	//Initialize all states...
state STATE = findLoader;	//Default state initialization

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    // Remember to clear the Timer1 interrupt flag when this ISR is entered.
    _T1IF = 0; // Clear interrupt flag
    
    milliseconds++;
}

void timer1_setup()
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

int main()
{	

        
     

	while(1)
	{
		switch(STATE)
		{
			case findLoader:
				if(PIN_IR_SENSOR)	//the IR-LED is found, then turn off motors and set STATE = toLoader
				{
					STATE = toLoader; //set state
				}
				else //continue to look for the IR-LED
				{
					turn_degrees(10);	//Spin around by turning on one motor forward and another motor backwards (10 is an arbitrary number)
				}
				//An interrupt will turn off the motors if the time expires
				break;
				
			case toLoader:
				if(PIN_SWITCH1 && PIN_SWITCH2)	//If reached by both switches, STATE = loading, turn off motors, break
				{
					STATE = loading; //set state
				}
				else	//Else, drive both motors
				{
					go_straight_inches(.25); //Just an arbitrarily small amount for now, this can be refined...
				}
				break;
				
			case loading:
				//this still needs to have the details worked out...
				//If loaded with 6, STATE = toShooting, Break
				//Else, do nothing (wait for loading)
				break;
				
			case toShooting:
				//call a function go_to_shooting()
				//Function should drive to center of arena
				//This can be done with no feedback if stepper motors are used
				
				//STATE = findTarget
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