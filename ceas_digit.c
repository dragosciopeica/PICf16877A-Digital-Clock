
#include <xc.h>
#include <pic16f77.h>
#include <delays.h>

//--------------------variable definition----------------------------------

int vec_numere[10]= { 63,6,91,79,102,109,125,7,127,111};/*  b'0000110,b'01011011,b'0101111,b'01100110,b'01101101,b'01111101,b'00000111,b'11111111,b'01101111 */
int vec_numere_alarma[10]= { 191,134,219,207,230,237,253,135,255,239};/*  b'0000110,b'01011011,b'0101111,b'01100110,b'01101101,b'01111101,b'00000111,b'11111111,b'01101111 */
volatile int flagTimer = 0, flagBut = 0, sec_buton = 0, min_buton = 0, alarma_buton = 0, startAlarma=0, stopAlarma = 0, AlarmaON=0,buzzAlarma=0,flagEroareAlarma=0,flagEroareCeas=0;
int Scounter = 0, Mcounter=0,ZScounter = 0, ZMcounter=0; // numaratoare pentru secunde, zeciS, minute, zeciM
int AScounter = 0, AMcounter=0,AZScounter = 1, AZMcounter=0; // numaratoare pentru secunde, zeciS, minute, zeciM ALARMA
int functAlarma = 0, buzzNr=0;
int WaitCounter = 0; // numarator pentru palpait 4 secunde
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

void Buzz()
	{
		if((AScounter==Scounter) & (AZScounter==ZScounter) & (AMcounter==Mcounter) & (AZMcounter==ZMcounter) & flagEroareAlarma==1 & flagEroareCeas==0) // refa aici!!!!!!!!!
			{
				
				buzzAlarma=1;
			}
	}

 void Display(int cifra,int catod)  // functia care foloseste la afisare MULTIPLEXAREA IN TIMP
	{   
		unsigned char cat_var = 0b00001000;
		unsigned char masca_cat = 0b11110000;
		
		if(AlarmaON==1)
			{  
				if(catod == 1)
				{	
					cat_var = ~(cat_var >> (catod-1)); // shiftam la dreapta variabila default cat_var, in functie de cifra.
					cat_var = cat_var ^ masca_cat;
					PORTA = cat_var; // Vor fi aprinsi catozii pe rand, foarte rapid.
					PORTD=vec_numere_alarma[cifra]; // afisam cifrele prin intermediul vectorului, unde am definit cifre
				}
				else
				{
					cat_var = ~(cat_var >> (catod-1)); // shiftam la dreapta variabila default cat_var, in functie de cifra.
					cat_var = cat_var ^ masca_cat;
					PORTA = cat_var; // Vor fi aprinsi catozii pe rand, foarte rapid.
					PORTD=vec_numere[cifra]; // afisam cifrele prin intermediul vectorului, unde am definit cifre		
				}		
			}
		else
			{
				cat_var = ~(cat_var >> (catod-1)); // shiftam la dreapta variabila default cat_var, in functie de cifra.
				cat_var = cat_var ^ masca_cat;
				PORTA = cat_var; // Vor fi aprinsi catozii pe rand, foarte rapid.
				PORTD=vec_numere[cifra]; // afisam cifrele prin intermediul vectorului, unde am definit cifrele in DEC.
			}
	}


void  check_buttons() // functia care verifica butoanele. odata la 10ms
	{ 
		
					
	if(startAlarma==1)
		{	
			if(PORTBbits.RB0 == 0) // buton pentru setare secunde
			{ 
						
				if(sec_buton^PORTBbits.RB0) // detectez frontul
					{ 
						WaitCounter = 0;
						AlarmaON=1;				
						if(AScounter >= 9)
							{ 
								AScounter = 0;                     
								PORTBbits.RB6=1;  // debug
													
									if(AZScounter >= 5)
										{ 	AZScounter = 0;
															
											if(AMcounter >= 9)
															
											{ 
												AMcounter =0;											
												if(AZMcounter >= 5)
													{ AZMcounter =0;  }
												else
													{  AZMcounter++; }
											}
															
												else
													{  AMcounter++; }
														
													}
												else
												 { AZScounter++; }
													
							}
						else
							{                                                          
								AScounter++;
								PORTBbits.RB6=0; // debug
							}												
					}
			}	
							
// ------------------------------------------------------------------------------------------								
			
		if(PORTBbits.RB2 ==0)  // buton pentru setare MINUTE	
			{					
				if(min_buton^PORTBbits.RB2) // detectez frontul
					{ 										
						AlarmaON=1;
						WaitCounter=0;										
						if(AMcounter >= 4)
																
							{ 
								AMcounter =0;											
								if(AZMcounter >= 2)
									{ AZMcounter =0;  }
								else
									{  AZMcounter++; }
							}
																
						else
							{  AMcounter++; }
					}
			}
		
		}// end if startAlarma			
						
		else // daca e modul ceas
		{												
			if(PORTBbits.RB0 == 0) // buton pentru setare secunde
				{ 
					
					if(sec_buton^PORTBbits.RB0) // detectez frontul
						{ 
							WaitCounter=0;
							flagBut = 1;
							if(Scounter >= 9)
								{ 
								Scounter = 0;                     
								PORTBbits.RB4=1;  // debug
										
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
								
								PORTBbits.RB4=0; // debug
								}												
						}
				}
						
							
// ------------------------------------------------------------------------------------------							
						
		if(PORTBbits.RB2 ==0)  // buton pentru setare MINUTE
			{					
				if(min_buton^PORTBbits.RB2) // detectez frontul
					{ 										
						flagBut = 1;	
						WaitCounter=0;										
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
			}
		
			
// --------------------------------------------------------------------------------------					
		if(PORTBbits.RB1==0) // buton pentru ALAMRA
			{ 	
									
				if(alarma_buton^PORTBbits.RB1)
					{
						functAlarma++;
						
						
						if(functAlarma>=2)
							{
								
								stopAlarma=1;
								functAlarma=0;
								
							}
						if(functAlarma<2)
							{
								startAlarma=1;
									
							}
					
					}
						
				
			}
        }// end else modul ceas	

			
		sec_buton = PORTBbits.RB0;
		min_buton = PORTBbits.RB2;
	    alarma_buton = PORTBbits.RB1;
	} // end functie butoane

//------------------------------------------------------------ MAIN -----------------------------



void main (void)
{   int counterIMP=0; // IMP de la impulsuri
	int Displaycounter=0;
	int numCat = 0; // numar catozi
	
	
	init();
	
	while (1) // se intampla mereu
	{            
        if(flagTimer) // intra in if-u asta de fiecare data cand vine interuperea, una la 10ms.
			{   flagTimer = 0;				
				counterIMP++;	// numarator pentru impulsuri.			
				check_buttons(); // apelez functia de check buttons
				Buzz();
				
				if(buzzAlarma==1) // sunet alarma
					{
						if(buzzNr >=299)
							{ 
								PORTBbits.RB4=0;  // pin catre BUZZER, il fac 0, nu mai suna
								buzzAlarma=0; // flag pentru BUZZER ( vine de la functia BUZZ)
								functAlarma=0; // counterul pentru modul Alarma ( ON sau OFF)
								buzzNr=0; // NUmarator de 4 secunde alarma
								startAlarma=0; // clipit alarma
								flagEroareAlarma=0; // flag pus pentru a evita eroarea care se produce cand programez alarma.
								AlarmaON=0; // NU mai e activa, dar ramane programata!
							}
						else
							{
								buzzNr++;
								PORTBbits.RB4=1;
							}
					}	
				
				if(AlarmaON==1) PORTBbits.RB3=1;     // LED semnalare alarma
				else	PORTBbits.RB3=0;
					
				if(stopAlarma==1) // daca apas pe STOP, dau reset la alarma si nici nu o mai activez
					{
						
						startAlarma=0;
						stopAlarma=0;
						buzzAlarma=0;
						buzzNr=0;
						functAlarma=0;
						AlarmaON=0; // sting LED
						PORTBbits.RB3=0; // LED care semnalizeaza ca am alarma ON
						flagEroareAlarma=0;
					
						AScounter=0;
						AZScounter=0;
						AMcounter=0;
						AZMcounter=0;
					}
					
				if(startAlarma==1)  // Ceasul clipeste in modul  setare ALARMA
					{
						
									
						counterIMP=0;   // nu mai numara ceasul
						WaitCounter++;  // se incrementeaza un contor
						Displaycounter++; // counter pentru "clipit"
						if(Displaycounter < 20)
							{							
							if(numCat >= 4)   // am 4 catozi, numar doar 4
								{ numCat = 1; }
							else
								{ numCat++; }					
						    switch(numCat) {
												case 1: Display(AScounter,numCat);  break; 
												case 2: Display(AZScounter,numCat);  break; 
												case 3: Display(AMcounter,numCat); break;
												case 4: Display(AZMcounter,numCat); break;
												default: break;
										   }
							}
						else if (Displaycounter == 49)  Displaycounter = 0;
						
					
					}
				
				if(flagBut==1) // Ceasul clipeste in modul  setare CEAS
					{	
						
						flagEroareCeas=1;
						counterIMP=0;   // nu mai numara ceasul
						WaitCounter++;  // se incrementeaza un contor
						Displaycounter++; // counter pentru "clipit"
						
						if(Displaycounter < 20)
							{							
								if(numCat >= 4)   // am 4 catozi, numar doar 4
									{ numCat = 1; }
								else
									{ numCat++; }					
								switch(numCat) {
													case 1: Display(Scounter,numCat);  break; 
													case 2: Display(ZScounter,numCat);  break; 
													case 3: Display(Mcounter,numCat); break;
													case 4: Display(ZMcounter,numCat); break;
													default: break;
											}
							}
						else 
							{
								if (Displaycounter == 49)  Displaycounter = 0; 
							}
									
					}
				if((flagBut == 1) & (startAlarma==1)) 	   // If-ul asta e pentru eroare, in caz ca apas pe program ceas si dupa pe program alarma!
					{
						
						flagBut=0;  // sterg flag, nu mai clipeste
						startAlarma=0; // sterg flag, nu mai clipeste
						functAlarma=0; // reset counter pt modul alarma
						
					}
					
				if((flagBut == 0) & (startAlarma==0) )
					{									
						if(numCat >= 4)   // am 4 catozi, numar doar 4
							{ numCat = 1; }
						else
							{ numCat++; }
							
							switch(numCat) {
								case 1: Display(Scounter,numCat);  break; 
								case 2: Display(ZScounter,numCat);  break; 
								case 3: Display(Mcounter,numCat); break;
								case 4: Display(ZMcounter,numCat); break;
								default: break;
										} 
					}
				
					
				
					if(WaitCounter == 399)  // cand contorul a ajuns la 400 ( adica 4 secunde ), CAND AJUNGE LA FINAL!!!
						{
	
							if(AlarmaON==0) // daca nu a fost apasat butonul de programare, counterul de functAlarma se pune pe 0 ( se reseteaza )
								{
									functAlarma=0;
								}
							else
								{
									flagEroareAlarma=1;  // cand ajunge WAITCOUNTER la final, si AlarmaON e 1, adica a fost setata o ora la alarma, flagEroareAlarma se pune pe 1!
								}
								
								flagEroareCeas=0;
								flagBut=0;  // sterg flag
								startAlarma=0; // sterg flag;
								WaitCounter=0;   // sterg contor
								counterIMP++;  // ceasul reincepe sa numere
							}
					
					
					
					
				if(counterIMP == 99) // cand counter-ul a ajuns la 100, adica 100*10=1000ms, 1 secunda. intra in if-ul asta.
					{     
						if(Scounter >= 9)
							{ counterIMP = 0;
							  Scounter = 0;                     
																
									if(ZScounter >= 5)
										{ 	ZScounter = 0;
											
											if(Mcounter >= 4)
											
											{ 
												Mcounter =0;											
												if(ZMcounter >= 2)
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
							}
					}
					
			} // end flagTimer ( intrerupere )
					
	} // end while
	
} // end void main


void interrupt tc_int (void)  // intreruperea, de aici primesc timpul. AM o intrerupere la 10ms.

{
    if(PIR1bits.TMR1IF) // e de 16 biti, cand ajunge la final. adica 65536-1
    {
	flagTimer = 1;
	TMR1L = 0xEF; // setam de unde sa porneasca timer LOW
	TMR1H= 0xD8; // setam de unde sa porneasca timer HIGH
    PIR1bits.TMR1IF=0; // punem interrupt flag pe 0
    }
}




