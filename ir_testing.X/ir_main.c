#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

#include "C:\Users\David\Documents\GitHub\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions

int STATE = 0;

//void config_pwm_14()
//{
//    //------------------------------------------------------------------
//    // CONFIGURE PWM1 (on pin 14) USING TIMER2
//
//    // Configure Timer2, which will be the source for the output compare that
//    // drives PWM1
//    T2CONbits.TON = 0;      // Timer2 off while configuring PWM, pg. 144
//
//    // Configure PWM1 on OC1 (pin 14)
//    OC1CON1 = 0b0000;               // Clear OC1 configuration bits, Table 4-8
//    OC1CON2 = 0b0000;               // Clear OC1 configuration bits, Table 4-8
//    OC1CON1bits.OCTSEL = 0b000;       // Use Timer2, pg. 157
//    OC1CON1bits.OCM = 0b110;          // Edge-aligned PWM mode, pg. 158
//    OC1CON2bits.SYNCSEL = 0b01100;    // Use Timer2, pg. 160
//
//    OC1R = 1;               // Set Output Compare value to achieve desired duty
//                            // cycle. This is the number of timer counts when
//                            // the OC should send the PWM signal low. The duty
//                            // cycle as a fraction is OC1R/PR2.
//
//    // Turn on Timer2
//    T2CONbits.TON = 1;
//}


int main()
{
    pin_config_init();
    //timing_interrupt_config();
    //analog_ultrasonic_setup();
    
    // From the ir_front_percent() function
    
    // AD1CHS register
    _CH0NA = 0;         // AD1CHS<7:5> -- Use VDD as negative input
    _CH0SA = 0b00000;         // AD1CHS<4:0> -- Use AN1 as positive input

    // AD1CON1 register
    _ADON = 1;          // AD1CON1<15> -- Turn off A/D
    _ADSIDL = 0;        // AD1CON1<13> -- A/D continues while in idle mode
    _MODE12 = 1;        // AD1CON1<10> -- 12-bit A/D operation
    _FORM = 0;          // AD1CON1<9:8> -- Unsigned integer output
    _SSRC = 7;          // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;          // AD1CON1<2> -- Auto sampling

    // AD1CSSL register
    AD1CSSL = 0;        // AD1CSSL<15:0> -- Skip all channels on input scan --
                        // see the CSCNA bits in AD1CON2

    // AD1CON2 register
    _PVCFG = 0;         // AD1CON2<15:14> -- Use VDD as positive ref voltage
    _NVCFG = 0;         // AD1CON2<13> -- Use VSS as negative ref voltage
    _CSCNA = 0;         // AD1CON2<10> -- Does not scan inputs specified in
                        // AD1CSSx registers (instead uses channels specified
                        // by CH0SA bits in AD1CHS register) -- Selecting '0'
                        // here probably makes writing to the AD1CSSL register
                        // unnecessary.
    _ALTS = 0;          // AD1CON2<0> -- Sample MUXA only

    // AD1CON3 register
    _ADRC = 0;          // AD1CON3<15> -- Use system clock
    _SAMC = 1;          // AD1CON3<12:8> -- Auto sample every A/D period TAD
    _ADCS = 0x3F;       // AD1CON3<7:0> -- A/D period TAD = 64*TCY
    
    //_ADON = 1;          // AD1CON1<15> -- Turn on A/D
    
    /*****************/
    
    //return values between 0-100 for percent of IR seen
    
    static float voltageLow = 1.22;
          
    float numLow;
    numLow = (4095)*(1.22/3.3);
    
    float percent;
    float percent2;
    
    //return(percent);
    
    while(1)
    {
        percent = 100.0 * ((ADC1BUF0-numLow)/(4095.0-numLow));
        percent2 = ir_front_percent();
        
    }
}
