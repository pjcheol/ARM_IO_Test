#include "board.h"
#include "AT91SAM7x.h"
#include "myLIB.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <pio/pio.h>
#include <aic/aic.h>
#include <utility\trace.h>
#include <cs8900a/cs8900a.h>

//For Switch
#define LEFT	1
#define RIGHT	2

unsigned int 	Key_Count=0,Pre_Key_Data=0;
unsigned char Switch_Check(void);
unsigned char Port_Flag=0;
unsigned int	Count=0; 
//============================================================================
//  Function  : PIT Interrupt
//============================================================================
void Isr_PIT(void)
{
    volatile unsigned int pit_pivr;
	if((rPIT_SR & 1) != 0)  //The Periodic Interval timer has reached PIV since the last read of PIT_PIVR
    {
		pit_pivr = rPIT_PIVR;    //Reads Periodic Interval Timer Value Register - Clears PITS in PIT_SR
//		Count++;
//		if(Count==100)
//		{
//			Count=0;
			if(Port_Flag==0)
			{
				rPIO_SODR_B=(LED1|LED2|LED3);
				Port_Flag=1;
			}
			else
			{
				
				rPIO_CODR_B=(LED1|LED2|LED3);
				Port_Flag=0;
			}	
//		}		
	}
}
void 	PIT_Interrupt_Setup(void) 
{
	unsigned int	tmp=0;

     rAIC_IECR = (1<<1);
     rAIC_SMR1 = (1<<5) +  (7<<0);  //Edge Trigger, Prior 7
     rAIC_SVR1 = (unsigned)Isr_PIT;
     tmp=(48000000/16/100)&0xFFFFF;         // T=30Hz
     rPIT_MR=(1<<25)+(1<<24)+(tmp<<0);      // Enable PIT, Disable Interrupt
}


void Port_Setup(void)
{
	AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOB );
	AT91F_PMC_EnablePeriphClock ( AT91C_BASE_PMC, 1 << AT91C_ID_PIOA );
	
	
	AT91F_PIO_CfgOutput( AT91C_BASE_PIOA,  PORTA);

	
	AT91F_PIO_CfgOutput( AT91C_BASE_PIOB, LED1|LED2|LED3 );
	AT91F_PIO_CfgPullup( AT91C_BASE_PIOB, LED1|LED2|LED3 );

	AT91F_PIO_CfgInput( AT91C_BASE_PIOA, SW1|SW2 );
	AT91F_PIO_CfgPullup( AT91C_BASE_PIOA, SW1|SW2 );

//AT91F_PIO_SetOutput(AT91C_BASE_PIOA, (1<<13));
//AT91F_PIO_ClearOutput(AT91C_BASE_PIOA, (1<<13));
	
}
/*
void Read_For_Setup_CMOS(void)
{

	//Read address Reset
	rPIO_CODR_A=FIFO_RD_RST;			
	rPIO_SODR_A=FIFO_RD;
	rPIO_CODR_A=FIFO_RD;
	rPIO_SODR_A=FIFO_RD;
	rPIO_CODR_A=FIFO_RD;
	rPIO_SODR_A=FIFO_RD_RST;

	//Write On		
	//rPIO_SODR_A=XCLK_ON;
	rPIO_SODR_A=HREF_SYNC;
			
	//Until Wait Low
	while((rPIO_PDSR_A & 0x00000004)){}
	//Until Wait High
	while(!(rPIO_PDSR_A & 0x00000004)){}
			
	//Write Off		
	//rPIO_CODR_A=HREF_SYNC;
			
	//CS_LOW
	rPIO_CODR_A=FIFO_CS;

}

void CMOS_Read_Clk(void)
{
	rPIO_SODR_A=FIFO_RD;
	rPIO_CODR_A=FIFO_RD;
}
*/

unsigned char Switch_Check(void)
{
unsigned char Result=0;

	if(!(rPIO_PDSR_A & SW1)) Result=LEFT;
	else if(!(rPIO_PDSR_A & SW2)) Result=RIGHT;
	
	
	if(Pre_Key_Data==Result) Key_Count++;
	else Key_Count=0;
	
	Pre_Key_Data=Result;
	return	Result;
}
//-----------------------------------------------------------------------------
/// Main Procedure
//-----------------------------------------------------------------------------
                   
int main()
{
	char	Count=0;
	unsigned int	Key_Data=0;

  	Port_Setup();
	
  	DBG_Init();
	PIT_Interrupt_Setup();
	//Uart_Printf("LCD Initial Complete\n\r");

	while(1) 
	{
	
//		rPIO_CODR_B=(LED1|LED2|LED3);
		rPIO_CODR_B=(LED2|LED3);
		Delay(60000);
		Delay(60000);
		Delay(60000);		
		Delay(60000);		
//		rPIO_SODR_B=(LED1|LED2|LED3);
		rPIO_SODR_B=(LED2|LED3);
		Delay(60000);
		Delay(60000);
		Delay(60000);		
		Delay(60000);
	
	}
		

/*	
	rPIO_CODR_A=PORTA;
	rPIO_SODR_A = (1<<Count);

	while(1) 
	{

		Key_Data=Switch_Check();
		if(Key_Data==LEFT && Key_Count==100)		//SW1 Push
		{
			rPIO_CODR_A=PORTA;
			if(Count==7) Count=0;
			else Count++;
			rPIO_SODR_A = (0x01<<Count);
		}
		else if(Key_Data==RIGHT && Key_Count==100) //SW2 Push
		{
			rPIO_CODR_A=PORTA;
			if(Count==0) Count=7;
			else Count--;
			rPIO_SODR_A = (0x80>>(7-Count));		
		}

	}

*/

	
}