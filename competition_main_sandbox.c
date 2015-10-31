#include "shooter.h"
#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

unsigned long milliseconds = 0; //Will run for 48+ days before overflow...

typedef enum STATE {findLoader, toLoader, loading, toShooting, findTarget};

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    // Remember to clear the Timer1 interrupt flag when this ISR is entered.
    _T1IF = 0; // Clear interrupt flag
    
    milliseconds++;
}


main()
{	

    _TON = 1;	//Turn on timer 1, for general use
	
    _TCKPS = 0b00;  //Timer pre-scaler set to 1:1, page 139
    _RCDIV = 0b011; //1mHz (w/ 8mHz), divide by 8
    
    // Configure Timer1 interrupt
    _T1IP = 5;      // Select interrupt priority
    _T1IE = 1;      // Enable interrupt
    _T1IF = 0;      // Clear interrupt flag
    PR1 = 500;    // Count to 1 milli-sec at 1 mHz, instruct at 500 kHz 

	while(1)
	{
		switch(STATE)
		{
			case findLoader:
				int sense1 = _PIN-WITH-IR-SENSOR;	//Check for the IR-LED signature, digital input
				if(sense1)	//the IR-LED is found, then turn off motors and set STATE = toLoader
				{
					STATE = toLoader;
				}
				else //continue to look for the IR-LED
				{
					turn_pulses(10);	//Spin around by turning on one motor forward and another motor backwards
				}
				//An interrupt will turn off the motors if the time expires
				break;
			case toLoader:
				int reached = ((_PIN-WITH-SWITCH1) && (_PIN-WITH-SWITCH2));	//Check if the loader has been reached, digital input
				if(reached)	//If reached, STATE = loading, turn off motors, break
				{
					STATE = loading;
				}
				else	//Else, drive both motors
				{
					go_straight_inches(.25);
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
				//
				break;			
		}
	}
}