#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

int findCenterState = 0;
	int findLoaderState = 0;
	int toLoaderState = 0;
	int loadingState = 0;
	int toShootingState = 0;
	int findTargetState = 0;
	int shottingState = 0;

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

int targetsFound = 0b000;

int loaderIrState;
#define IR_TIMES 5
unsigned long irTimeValues[IR_TIMES];




#define ULTRASONIC_VALUES 10

unsigned long ultrasonicValuesB[ULTRASONIC_VALUES];
unsigned long ultrasonicValuesF[ULTRASONIC_VALUES];

int i;
unsigned long startTime;
#include "board.h" //set up pin names, hold all additional functions

typedef enum {findCenter, findLoader, toLoader, loading, toShooting, findTarget, shooting, findLoader2, end} state;	//Initialize all states...
state STATE = findCenter;	//Default state initialization

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



void config_pwm_14()
{
    //------------------------------------------------------------------
    // CONFIGURE PWM1 (on pin 14) USING TIMER2

    // Configure Timer2, which will be the source for the output compare that
    // drives PWM1
    T2CONbits.TON = 0;      // Timer2 off while configuring PWM, pg. 144
    T2CONbits.TCKPS = 0b00;    // Timer2 1:1 clock pre-scale, pg. 144
    T2CONbits.T32 = 0;      // Timer2 acts as a single 16-bit timer, pg. 144
    T2CONbits.TCS = 0;      // Timer2 internal clock source, pg. 144

    // Configure PWM1 on OC1 (pin 14)
    OC1CON1 = 0b0000;               // Clear OC1 configuration bits, Table 4-8
    OC1CON2 = 0b0000;               // Clear OC1 configuration bits, Table 4-8
    OC1CON1bits.OCTSEL = 0b000;       // Use Timer2, pg. 157
    OC1CON1bits.OCM = 0b110;          // Edge-aligned PWM mode, pg. 158
    OC1CON2bits.SYNCSEL = 0b01100;    // Use Timer2, pg. 160

    // Set period and duty cycle
    PR2 = 3999;                // Period of Timer2 to achieve FPWM = 1 kHz
                            // See Equation 15-1 in the data sheet
    OC1R = 3999;               // Set Output Compare value to achieve desired duty
                            // cycle. This is the number of timer counts when
                            // the OC should send the PWM signal low. The duty
                            // cycle as a fraction is OC1R/PR2.

    // Turn on Timer2
    T2CONbits.TON = 1;
}





int main()
{	
    
    config_pwm_14();
	timing_interrupt_config();
	ultrasonic_setup();
    ANSB = 0;
    _TRISB15 = 0;
    _TRISB8 = 0;
	

	while(1)
	{
		switch(STATE)
		{
            case findCenter:
				switch(findCenterState)
				{
					case 0:
						if(find_normal())   //First normal found
						{
							findCenterState = 1;
						}
						else    //Keep looking
						{
							//do nothing
						}
						break;
					case 1:
						if(find_24())   //First 24" found
						{
							findCenterState = 2;
						}
						else    //Keep looking
						{
							//do nothing
						}
						break;
					case 2:
						if(turn_degrees(90))    //Turned 90 degrees
						{
							findCenterState = 3;
						}
						else
						{
							//do nothing
						}
						break;
					case 3:
						if(find_24())   //Second 24" found
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
							while((milliseconds - startTime) <= ((5.0 + 2.0)/LOADING_IR_FREQ)*1000.0)//Pause and allow for the IR sensor to adjust
                            {
                                //do nothing
                            }
						}
						else
						{
							//do nothing
						}
						break;
					case 1:	//Turn completed...
						if((milliseconds - irTimeValues[0]) < ((5.0 + 2.0)/LOADING_IR_FREQ)*1000.0)	//The array of 5 values has been filled recently by interrupts (IR Beacon Found)
						{
							findLoaderState = 0; // Must be used in series with findCenter
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
				
			case toLoader: 								// At this point, is it already pointing backwards to the loader? - David
				switch(toLoaderState)
				{
					case 0:
						if(go_straight_inches(-1*INCHES_CORNER_TO_CENTER)) 			// Are we going to use control here? E.i. re-center on IR - David
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
						if(go_straight_inches(INCHES_CORNER_TO_CENTER))				// Same as toLoader--are we going to use control? - David
							STATE = findTarget;
						break;
				}
				break;
				
			case findTarget:
				switch(findTargetState)
				{
					case 0: //Just starting
						if(ir_front_found())    //straight ahead is the target
                        {
                            STATE = shooting;
                            targetsFound = targetsFound|0b010;
                            findTargetState = 0;
                        }
                        else    //Not ahead
                        {
                            findTargetState = 1;
                        }
						break;
                    case 1:
                        if(turn_degrees(90))
                        {
                            findTargetState = 2;
                        }
                        else
                        {
                            //do nothing
                        }
                        break;
                    case 2:
                        if(ir_front_found())    //It was the right one, start shooting
                        {
                            STATE = shooting;
                            targetsFound = targetsFound|0b001;
                            findTargetState = 0;
                        }
                        else if(ir_back_found())    //Turned the wrong way...
                        {
                            findTargetState = 3;
                            targetsFound = targetsFound|0b100;
                        }
                    case 3:
                        if(turn_degrees(180))
                        {
                            STATE = shooting;
                            findTargetState = 0;
                        }
				}
				break;
				
			case shooting:
				//check milliseconds
				//call shoot() function 6 times
				//function should handle all coordination of solenoids
				//count number of shoot() calls
				
				//STATE = findLoader
                
                    STATE = toLoader;
				break;
			
            case findLoader2:
                //TO DO: Turn back to face loader depending on which target you shot at... 
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

