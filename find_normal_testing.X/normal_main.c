#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

#include "C:\Users\Walter\Documents\Walter\School\Fall 2015\Mechatronics\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions

int STATE = 0;
unsigned long startTime = 0;

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
    pin_config_init();
    timing_interrupt_config();
    ir_finder_analog_setup();
    analog_ultrasonic_setup();
    
    //analog_ultrasonic_setup();
    //ultrasonic_setup();
    //config_pwm_14();
    
    _ANSA0;
    ANSB = 1;   //Digital buffer is active
    
    //_TRISA0 = 1;
    _TRISB2 = 1;
    _TRISA2 = 1;
    _TRISA0 = 1;
    _TRISA1 = 1;
    while(1)
    {
        
        //OC1R = ((read_dist_simple()/148.0)/48.0)*PR2;    //Should output PWM with duty cycle from 0%-100% for 0"-48"
        
        switch(STATE)
       {
            case 0:
                if(find_24())//if(find_normal_analog())
                {
                    STATE = 1;
                }
                break;
           case 1:
               delay(2500);
               STATE = 0;
                break;
       }
                 
    }
}
