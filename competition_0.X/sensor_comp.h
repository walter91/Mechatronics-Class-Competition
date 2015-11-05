/* 
 * File: motion.h  
 * Author: Walter Coe
 * Comments: Functions and Declarations which govern the motion for the 
 *              ME330 shooter at BYU.
 * Revision history: See Github...
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


#define PIN_IR_F _RA0 //2
#define PIN_IR_B _RA1 //3

#define PIN_DIST_F _RB2  //6
#define PIN_DIST_B _RA2  //7


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

