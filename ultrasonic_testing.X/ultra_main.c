#include <p24f16KA301.h>

_FOSCSEL (FNOSC_FRCDIV);   //8mHz with Post-scaling

#include "C:\Users\Walter\Documents\Walter\School\Fall 2015\Mechatronics\Mechatronics-Class-Competition\competition.X\board.h" //set up pin names, hold all additional functions

int STATE = 0;

void config_pwm_4()
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
    OC2CON1 = 0b0000;               // Clear OC1 configuration bits, Table 4-8
    OC2CON2 = 0b0000;               // Clear OC1 configuration bits, Table 4-8
    OC2CON1bits.OCTSEL = 0b000;       // Use Timer2, pg. 157
    OC2CON1bits.OCM = 0b110;          // Edge-aligned PWM mode, pg. 158
    OC2CON2bits.SYNCSEL = 0b01100;    // Use Timer2, pg. 160

    // Set period and duty cycle
    PR2 = 3999;                // Period of Timer2 to achieve FPWM = 1 kHz
                            // See Equation 15-1 in the data sheet
    OC2R = 3999;               // Set Output Compare value to achieve desired duty
                            // cycle. This is the number of timer counts when
                            // the OC should send the PWM signal low. The duty
                            // cycle as a fraction is OC1R/PR2.

    // Turn on Timer2
    T2CONbits.TON = 1;
}


void config_ad(void)
{
    //------------------------------------------------------------------------
    // A /D Configuration Function
    //
    // This function configures the A/D to read from two channels in auto
    // conversion mode.
    //------------------------------------------------------------------------
    

    // AD1CHS register
    _CH0NA = 0;         // AD1CHS<7:5> -- Use VDD as negative input

    // AD1CON1 register
    _ADON = 1;          // AD1CON1<15> -- Turn on A/D
    _ADSIDL = 0;        // AD1CON1<13> -- A/D continues while in idle mode?
    _MODE12 = 1;        // AD1CON1<10> -- 12-bit or 10-bit?
    _FORM = 0b00;          // AD1CON1<9:8> -- Output format, pg. 211
    _SSRC = 0b0111;          // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;          // AD1CON1<2> -- Auto sampling

    // AD1CSSL registers
    AD1CSSL = 0xffff;        // AD1CSSL<15:0> -- Select lower channels to scan, turn on AN0 and AN1
    AD1CSSH = 0x0000;        // AD1CSSH<15:0> -- Select upper channels to scan, NOT USED!

    // AD1CON2 register
    _PVCFG = 0b00;         // AD1CON2<15:14> -- Use VDD as positive ref voltage
    _NVCFG = 0;         // AD1CON2<13> -- Use VSS as negative ref voltage
    _BUFREGEN = 1;      // AD1CON2<11> -- Results stored using channel indexed
                        // mode -- AN1 result is stored in ADC1BUF1, AN2 result
                        // is stored in ADC1BUF2, etc.
    _CSCNA = 1;         // AD1CON2<10> -- Scans inputs specified in AD1CSSx
                        // registers instead of using channels specified
                        // by CH0SA bits in AD1CHS register
    _ALTS = 0;          // AD1CON2<0> -- Sample MUXA only
    _SMPI = 0b00001;          // AD1CON2<6:2> -- Interrupts at the conversion for
                        // every other sample

    // AD1CON3 register
    _ADRC = 0;          // AD1CON3<15> -- Use system clock
    _SAMC = 0x00001;          // AD1CON3<12:8> -- Auto sample every A/D period TAD
    _ADCS = 0x00111111;          // AD1CON3<7:0> -- A/D period TAD = 64*TCY
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
    
    //config_ad();
    analog_ultrasonic_setup();
    pin_config_init();
    timing_interrupt_config();
    //ultrasonic_setup();
    config_pwm_4();
    
    _ANSA0;
    ANSB = 1;   //Digital buffer is active
    
    //_TRISA0 = 1;
    _TRISB2 = 1;
    _TRISA2 = 1;
    _TRISA0 = 1;
    _TRISA1 = 1;
    
    _TRISA6 = 0;
    
    //analog_ultrasonic_setup();
    
    while(1)
    {
        
        
        
        
    }
}

