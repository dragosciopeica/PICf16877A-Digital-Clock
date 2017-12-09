/* 
- CPU Type & Libraries ---------------------------
*/
#include <pic16f77.h>
#include <stdlib.h>
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>


/* 
- Configuration ----------------------------------
*/
#pragma config WDTE=OFF 		//disable watch dog
#pragma config FOSC=XT 		// XT oscillator (4 MHz) ( la HS 20 MHz)
#pragma config LVP=OFF		// disable low voltage program 
#pragma config DEBUG=ON 	// RB6,RB7 can`t be used as general I/O pins


/* 
- Pin Definitions --------------------------------
*/
//#define Trigger_Out		PORTCbits.RC4     /* Define Trigger output signal for oscilloscope*/
//#define Trigger_Tris	TRISCbits.RC4     /* Define Trigger TRIS */
