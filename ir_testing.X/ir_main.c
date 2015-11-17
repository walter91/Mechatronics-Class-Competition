#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

#include "C:\Users\Walter\Documents\Walter\School\Fall 2015\Mechatronics\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions

int STATE = 0;

void config_pwm_14()
{
    //------------------------------------------------------------------
    // CONFIGURE PWM1 (on pin 14) USING TIMER2

    // Configure Timer2, which will be the source for the output compare that
    // drives PWM1
    T2CONbits.TON = 0;      // Timer2 off while configuring PWM, pg. 144

    // Configure PWM1 on OC1 (pin 14)
    OC1CON1 = 0b0000;               // Clear OC1 configuration bits, Table 4-8
    OC1CON2 = 0b0000;               // Clear OC1 configuration bits, Table 4-8
    OC1CON1bits.OCTSEL = 0b000;       // Use Timer2, pg. 157
    OC1CON1bits.OCM = 0b110;          // Edge-aligned PWM mode, pg. 158
    OC1CON2bits.SYNCSEL = 0b01100;    // Use Timer2, pg. 160

    OC1R = 1;               // Set Output Compare value to achieve desired duty
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
    ultrasonic_setup();
    
    
    while(1)
    {
        OC1R = ir_front_percent()*PR2;    //Should output PWM with duty cycle from 0%-100% for 0"-48"
    }
}
