#ifndef SHOOTER_H	//Include guard
#define SHOOTER_H

//Pin Definitions
	//this will include readable names for all the pins we will use...
#define PIN_IR_SENSOR _RA0  //
#define PIN_SWITCH1 _RA1
#define PIN_SWITCH2 _RA2

//Varialbes for use
int i;


//Functions
void go_straight_inches(float inches)
	{
		int stepsPerRev = 200; //Number of steps required for one rev of motor
		float wheelCircumferenceInches = 2.125;	//Drive wheel circumference in inches
		int stepsPerInch = (int)(stepsPerRev/wheelCircumferenceInches);	//Number of pulses required to move forward 1 inch (pulses/revolution)*(revolutions/inch)
		int numberOfSteps = (int)(inches*stepsPerInch);	//convert inches to number of steps
		
		//set direction pins, both forward
		
		for(i = 0; i < numberOfSteps; i++) //Turns the 
		{
			//step pins high
			//pause
			//step pins low
			//pause
		}
	}
	
	void turn_180()
	{
		static int stepsToTurnAround = 200;	//number of steps to turn 180 degrees
		//set direction pins, one each direction
		
		for(i=0; i < stepsToTurnAround; i++)
		{
			//step pins high
			//pause
			//step pins low
			//pause
		}
	}
	
	void turn_degrees(int degrees)
	{
		static int stepsPerDegree = 5; //Number of pulses required to spin 1 degrees
		int numberOfSteps = (int)(degrees * stepsPerDegree); //Number of steps to reach the desired degrees
		
		//set direction pins, one each direction
		
		for(i = 0; i < numberOfSteps; i++)
		{
			//step pins high
			//pause
			//step pins low
			//pause
		}
	}
	
	void shoot_once()
	{
		//open lower gate, drops ball into chamber from holding cell
		//pause, allow time for ball to drop
		//close lower gate, seal the chamber
		//energize shooter solenoid, expel the ball
		//pause, allow for the full stroke
		//de-energize shooter solenoid, bring the hammer back
		//open upper gate, drop a ball into holding cell
		//pause, all time for ball to drop
		//close upper gate, seal the holding cell
	}
	
#endif //SHOOTER_H