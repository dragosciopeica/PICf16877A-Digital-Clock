
#include <xc.h>
#include <pic16f77.h>
#include <delays.h>

//--------------------variable definition----------------------------------

int vec_numere[10]= { 63,6,91,79,102,109,125,7,127,111};/*  b'0000110,b'01011011,b'0101111,b'01100110,b'01101101,b'01111101,b'00000111,b'11111111,b'01101111 */
volatile int flagTimer = 0;


//------------------function declaration-----------------------------
	
 void init (void)
{
	TRISB=0x07; 	// PINII 0,1,2 AI PORTULUI B SUNT SETATI CA INTRARI ( BUTOANE) SI PINUL 4 CA IESIRE ( BUZZER )
	TRISA=0x00;		// PINII 0,1,2,3 AI PORTULUI A SUNT SETATI CA IESIRI ( CATOZI )
	PORTA=0b11111111;	    // FIIND CATOD COMUN, SE COMANDA CU 1, LA 1 SUNT STINSE ( catodul se leaga la 0 ) !!!
	TRISD=0x00; 	// PINII PORTULUI D SUNT SETATI CA SI IESIRI, LED-URILE ( A, B,C...DP )
	PORTD=0x00;		// LEDURILE SUNT STINSE
	
	OPTION_REG=0b00000000; // pornim rezistentele de pull up ale portului B pt butoane


	INTCONbits.GIE=1; 	// pornim intreruperile globale
	INTCONbits.PEIE=1; 	// periferic interupt enable
	INTCONbits.RBIE=1; 	// PORT B interupt enable
	INTCONbits.RBIF=0; 	// stergem flagul de intrerupere
    ADCON1=0x06;		// Port B bits 0-4 sunt intrari digitale  UITA_TE !!!

// TIMER

	PIR1= 0x00; // NU activam intreruperea
	PIE1= 0x01; // activam intrerupt enable
	TMR1L = 0xEF; // setam de unde sa porneasca timer LOW
	TMR1H= 0xD8; // setam de unde sa porneasca timer HIGH
	T1CON= 0x01;  // DAM START LA TIMER (s59)

}

 void secunde(int cifra,int catod)
{   
unsigned char cat_var = 0b00001000;
unsigned char masca_cat = 0b11110000;
	
	cat_var = ~(cat_var >> (catod-1));
    cat_var = cat_var ^ masca_cat;
    PORTA = cat_var;
	PORTD=vec_numere[cifra];
	
}


void main (void)
{   int counterIMP=0;
	int numCat = 0;
	 int Scounter = 0, ZScounter = 6
	 , Mcounter=5, ZMcounter=1;
	init();
	while (1)
	{
                     
        if(flagTimer)
			{   flagTimer = 0;
				counterIMP++;
				
				if(numCat >= 4)
					{ numCat = 1; }
			    else
					{ numCat++; }
					
					switch(numCat) {
						case 1: secunde(Scounter,numCat);  break; 
						case 2: secunde(ZScounter,numCat);  break; 
						case 3: secunde(Mcounter,numCat); break;
						case 4: secunde(ZMcounter,numCat); break;
						default: break;
								}
				
				if(counterIMP == 99)
					{     
						if(Scounter >= 9)
							{ counterIMP = 0;
							  Scounter = 0;                     
							  PORTBbits.RB6=1;  // debug
									
									if(ZScounter >= 5)
										{ 	ZScounter = 0;
											
											if(Mcounter >= 9)
											
											{ 
												Mcounter =0;											
												if(ZMcounter >= 5)
													{ ZMcounter =0;  }
												else
													{  ZMcounter++; }
											}
											
										    else
												{  Mcounter++; }
										  
										}
									else
										{ ZScounter++; }
									
							}
						else
							{                                                          
							  Scounter++;
							  counterIMP = 0;
                              PORTBbits.RB6=0; // debug
							}
					}
			
			
			
			}
			
		
	}
	
}









void interrupt tc_int (void)  // intreruperea, de aici primesc timpul

{
    if(PIR1bits.TMR1IF)
    {
	flagTimer = 1;
    PORTBbits.RB7 = PORTBbits.RB7^1;
	TMR1L = 0xEF; // setam de unde sa porneasca timer LOW
	TMR1H= 0xD8; // setam de unde sa porneasca timer HIGH
        PIR1bits.TMR1IF=0;
    }
}




















/* switch(cifra)
                {
		case 0 :		PORTDbits.RD0 = 1;
                        PORTDbits.RD1 = 1;
                        PORTDbits.RD2 = 1;
                        PORTDbits.RD3 = 1;
                        PORTDbits.RD4 = 1;
                        PORTDbits.RD5 = 1;
                        PORTDbits.RD6 = 0;
                        break;
		case 1: 		PORTDbits.RD0 = 0;
                        PORTDbits.RD1 = 1;
                        PORTDbits.RD2 = 1;
                        PORTDbits.RD3 = 0;
                        PORTDbits.RD4 = 0;
                        PORTDbits.RD5 = 0;
                        PORTDbits.RD6 = 0;
                        break;             
		case 2:			 PORTDbits.RD0 = 1;
                        PORTDbits.RD1 = 1;
                        PORTDbits.RD2 = 0;
                        PORTDbits.RD3 = 1;
                        PORTDbits.RD4 = 1;
                        PORTDbits.RD5 = 0;
                        PORTDbits.RD6 = 1;
                        break;
		case 3: 		PORTDbits.RD0 = 1;
                        PORTDbits.RD1 = 1;
                        PORTDbits.RD2 = 1;
                        PORTDbits.RD3 = 1;
                        PORTDbits.RD4 = 0;
                        PORTDbits.RD5 = 0;
                        PORTDbits.RD6 = 1;
                        break;
		case 4: 		PORTDbits.RD0 = 0;
                        PORTDbits.RD1 = 1;
                        PORTDbits.RD2 = 1;
                        PORTDbits.RD3 = 0;
                        PORTDbits.RD4 = 0;
                        PORTDbits.RD5 = 1;
                        PORTDbits.RD6 = 1;
                        break;
		case 5: 		PORTDbits.RD0 = 1;
                        PORTDbits.RD1 = 0;
                        PORTDbits.RD2 = 1;
                        PORTDbits.RD3 = 1;
                        PORTDbits.RD4 = 0;
                        PORTDbits.RD5 = 1;
                        PORTDbits.RD6 = 1;
                        break;
		case 6: 		PORTDbits.RD0 = 1;
                        PORTDbits.RD1 = 0;
                        PORTDbits.RD2 = 1;
                        PORTDbits.RD3 = 1;
                        PORTDbits.RD4 = 1;
                        PORTDbits.RD5 = 1;
                        PORTDbits.RD6 = 1;
                        break;
		case 7: 		PORTDbits.RD0 = 1;
                        PORTDbits.RD1 = 1;
                        PORTDbits.RD2 = 1;
                        PORTDbits.RD3 = 0;
                        PORTDbits.RD4 = 0;
                        PORTDbits.RD5 = 0;
                        PORTDbits.RD6 = 0;
                        break;
		case 8: 		PORTDbits.RD0 = 1;
                        PORTDbits.RD1 = 1;
                        PORTDbits.RD2 = 1;
                        PORTDbits.RD3 = 1;
                        PORTDbits.RD4 = 1;
                        PORTDbits.RD5 = 1;
                        PORTDbits.RD6 = 1;
                        break;
		case 9: 		PORTDbits.RD0 = 1;
                        PORTDbits.RD1 = 1;
                        PORTDbits.RD2 = 1;
                        PORTDbits.RD3 = 1;
                        PORTDbits.RD4 = 0;
                        PORTDbits.RD5 = 1;
                        PORTDbits.RD6 = 1;
                        break;
                default:PORTDbits.RD0 = 1;
                        PORTDbits.RD1 = 1;
                        PORTDbits.RD2 = 1;
                        PORTDbits.RD3 = 1;
                        PORTDbits.RD4 = 1;
                        PORTDbits.RD5 = 1;
                        PORTDbits.RD6 = 0;
                        
			} */