#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

int findCenterState = 0;
int findLoaderState = 0;
int toLoaderState = 0;
int loadingState = 0;
int toShootingState = 0;
int findTargetState = 0;
int shottingState = 0;


#include "board.h" //set up pin names, hold all additional functions

typedef enum {findCenter, findLoader, toLoader, loading, toShooting, findTarget, shooting, findLoader2, end} state;	//Initialize all states...
//state STATE = findCenter;	//Default state initialization
state STATE = findCenter; //Testing purposes

int main()
{
    pin_config_init();
    timing_interrupt_config();
   
    //while(some_button_is_not_pushed); //This will allow the system to pause until a start button is pushed before the 
    
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
							if(ir_front_percent() >= IR_FOUND_THRESH)   //front found reference corner
                            {
                                findLoaderState = 1;    //turn -90 and then back up
                            }
                            else if(ir_front_percent() >= LOADER_FOUND_THRESH)
                            {
                                findLoaderState = 2;    //turn 180 and then back up
                            }
//                            else if(ir_back_percent() >= IR_FOUND_THRESH)
//                            {
//                                findLoaderState = 3;    //turn 90 and then back up
//                            }
//                            else if(ir_back_percent() >= IR_FOUND_THRESH)
//                            {
//                                findLoaderState = 4;    //turn -90 and then back up
//                            }
                            else
                            {
                                findLoaderState = 5;//turn 90 and check again...
                            }
                            
						}
						else
						{
							//do nothing
						}
						break;
					case 1:	//Turn completed...
						if(turn_degrees(-90))
                        {
                            findLoaderState = 6;    //Back up
                        }
                        else
                        {
                            //do nothing
                        }
						break;
					case 2: //IR Not found, continue turning...
						if(turn_degrees(180))
                        {
                            findLoaderState = 6;    //Back up
                        }
                        else
                        {
                            //do nothing
                        }
						break;
                    case 3: //IR Not found, continue turning...
						if(turn_degrees(90))
                        {
                            findLoaderState = 6;    //Back up
                        }
                        else
                        {
                            //do nothing
                        }
						break;
                    case 4: //IR Not found, continue turning...
						if(turn_degrees(180))
                        {
                            findLoaderState = 6;    //Back up
                        }
                        else
                        {
                            //do nothing
                        }
						break;
                    case 5:
                        if(turn_degrees(45))
                        {
                            findLoaderState = 0;
                        }
                                         
                        //turn 90 and check
                        break;
                    case 6:
                            findLoaderState = 0;
                            STATE = toLoader;
				}
				break;
				
			case toLoader: 								// At this point, is it already pointing backwards to the loader? - David
				switch(toLoaderState)
				{
					case 0:
                        if(stage_timer() != 4 || stage_timer() != 5)
                        {
                            if(go_straight_inches(-1*INCHES_CORNER_TO_CENTER)) 			// Are we going to use control here? E.i. re-center on IR - David
                                STATE = loading;
                        }
						break;
				}
				break;
				
			case loading:
				//this still needs to have the details worked out...
				//If loaded with 6, STATE = toShooting, Break
				//Else, do nothing (wait for loading)
				if(loading_timer(6.0))
                {
                    STATE = toShooting;
                }
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
                if(aquire_target()) //target found
                {
                    STATE = shooting;
                }
                else
                {
                    //wait for target to be found...
                }
				/*
                switch(findTargetState)
				{
					case 0: //Just starting
						if(ir_front_percent() >= 50.0)    //straight ahead is the target
                        {
                            STATE = shooting;
                            targetsFound = targetsFound|0b010;	//Straight ahead has been found...
                            findTargetState = 0;	//Reset for next time...
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
                        if(ir_front_percent() >= 50.0)    //It was the right one, start shooting
                        {
                            STATE = shooting;
                            targetsFound = targetsFound|0b001;
                            findTargetState = 0;
                        }
                        else if(ir_back_percent() >= 50.0)    //Turned the wrong way...
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
                 */
				
			case shooting:
				if(!stage_change())
				{
					if(shoot(7))    //Six shots plus one load complete
					{               
						STATE = toLoader;
                        //TODO: turn to face away from loader...
					}
					else
					{
						//keep shooting...
					}
				}
				else if(stage_timer() == 1 || stage_timer() == 2 || stage_timer() == 3)  //Still a shooting stage...
				{
					STATE = findTarget;
				}
				else if(stage_timer() == 0)	//The time hasn't come to shoot
				{
					//Do nothing...
					//Wait for the time to shoot to arrive...
				}
				else if(stage_timer() == 4 || stage_timer() == 5)
				{
					STATE = end;
				}
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

