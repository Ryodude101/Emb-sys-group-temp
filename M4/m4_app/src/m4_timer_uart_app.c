/*
 * "Copyright (c) 2019 J. W. Bruce ("AUTHOR(S)")"
 * All rights reserved.
 * (J. W. Bruce, jwbruce_AT_tntech.edu, Tennessee Tech University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */

// #define ESOS_USE_IRQS

// INCLUDEs go here  (First include the main esos.h file)
//      After that, the user can include what they need
#include    "esos.h"
#include  "embsysS20.h"
#ifdef __linux
#include    "esos_pc.h"
#include    "esos_pc_stdio.h"
// INCLUDE these so our printf and other PC hacks work
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib>
#else
#include    "esos_stm32l4.h"
#include    "esos_stm32l4_rs232.h"
#endif

// Defines
#define DECODE_FLASH 62 // Half of 0.125 s period for decoding flash rounded down to avoid floats
#define ENCODE_FLASH 250 // Half of 0.5 s period for encoding flash
#define SPACE_INPUT 0x00 // ASCII for [space] input

void __user_init_hw( void );


// GLOBALs go here
static uint16_t u16_encode_periodHalf;
//  Generally, the user-created semaphores will be defined/allocated here
//static uint8_t psz_CRNL[3]= {0x0D, 0x0A, 0};

// timer globals

#ifdef _linux
#endif

ESOS_USER_TASK( flash_LED ) 
{
	u16_encode_periodHalf = ENCODE_FLASH;
	ESOS_TASK_BEGIN();
	while (TRUE) 
	{
		TOGGLE_LD2();
		if (IS_B1_RELEASED())
		{
			ESOS_TASK_WAIT_TICKS( u16_encode_periodHalf );
		}
		else
		{ 
			ESOS_TASK_WAIT_TICKS( DECODE_FLASH );
		}

	} // endof while(TRUE)
	ESOS_TASK_END();
} // end flash_LED() 

ESOS_USER_TASK( usart_comms )
{
	static uint8_t u8_incoming;
	static uint8_t au8_key[] = {'T', 'E', 'N', 'N', 'E', 'S', 'S', 'E', 'E', 'T', 'E', 'C', 'H'};
	static uint8_t u8_countEncode = 0;
	static uint8_t u8_countDecode = 0;
	static uint8_t u8_exclamCount = 0;
	static bool b_changeFlag = FALSE;
	static uint8_t u8_digitCount = 0;
	static uint16_t u16_changePeriod = 0;
	static bool b_skipPrint = FALSE;
	static char ac_charArr[10];
	static uint8_t u8_charCount = 0;
	
	
	ESOS_TASK_BEGIN();
	while (TRUE) {
	
	// ESOS UART comms uses 57.6k/8/N/1
		ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
		ESOS_TASK_WAIT_ON_GET_UINT8( u8_incoming );
		ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();
		
		if(u8_countEncode > 12) //reset encode position in key when at the end
		{
			u8_countEncode = 0;
		}
		
		if(u8_countDecode > 12) //reset decode position in key when at the end
		{
			u8_countDecode = 0;
		}
		
		if (u8_incoming >= 'a' && u8_incoming <='z') //If incoming is a letter
		{
			u8_incoming = u8_incoming - 'a' + 'A'; //convert to capital
			
			if(IS_B1_RELEASED()) //encoding mode -- works!
			{
				//add key character to letter
				u8_incoming = ((((u8_incoming - 'A') + (au8_key[u8_countEncode] - 'A')) % 26) + 'A');
				//increment count
				u8_countEncode++;
			}
			
			else //decoding mode -- works!
			{
				//subtract key character from letter
				u8_incoming = (((u8_incoming - au8_key[u8_countDecode]) + 26) % 26) + 'A';
				//increment count
				u8_countDecode++;
			}
		}
		
		if(u8_incoming == '!') //if incoming is an exclamation
		{
			u8_exclamCount++;
		}
		
		//report period !!!L
		if((u8_exclamCount == 3) && ((u8_incoming == 'L')))
		{
			if(IS_B1_RELEASED())
			{
				u16_encode_periodHalf *= 2;
				itoa(u16_encode_periodHalf, ac_charArr, 10);
				
				ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
				ESOS_TASK_WAIT_ON_SEND_UINT8( 'L' );
				
				u8_charCount =0;
				
				while(ac_charArr[u8_charCount] != '\0')
				{
					ESOS_TASK_WAIT_ON_SEND_UINT8( ac_charArr[u8_charCount] );
					ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
					u8_charCount++;
				}
			}
			
			else if(IS_B1_PRESSED())
			{
				u8_incoming = (DECODE_FLASH * 2);
				
				ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
				ESOS_TASK_WAIT_ON_SEND_UINT8( 'L' );
				ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING( u8_incoming );
				ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
			}
			
			b_skipPrint = TRUE;
			u8_exclamCount = 0;
		}
		
		//change period !!!S####
		else if((u8_exclamCount == 3) && (u8_incoming == 'S'))
		{
			b_changeFlag = TRUE;
			u16_changePeriod = 0;
		}
		
		
		if(b_changeFlag)
		{
			++u8_digitCount;
			
			//if #---
			if(u8_digitCount == 1)
			{
				u16_changePeriod = u16_changePeriod + ((u8_incoming - '0') * 1000);
			}
			
			//if -#--
			else if(u8_digitCount == 2)
			{
				u16_changePeriod = u16_changePeriod + ((u8_incoming - '0')* 100);
			}
			
			
			//if --#-
			else if(u8_digitCount == 3)
			{
				u16_changePeriod = u16_changePeriod + ((u8_incoming - '0') * 10);
			}
			
			//if ---#
			else if(u8_digitCount == 4)
			{
				u16_changePeriod = u16_changePeriod + (u8_incoming - '0');
				
				ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
				ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING( u16_changePeriod );
				ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
				u16_encode_periodHalf = u16_changePeriod / 2;
				
				b_changeFlag = FALSE;
				u8_digitCount = 0;
			}
			
			u8_exclamCount = 0;
		}
		
		if(!b_skipPrint)
		{
			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_UINT8( u8_incoming );
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		}
		
		else
		{
			b_skipPrint = FALSE;
		}
		
		} // endof while(TRUE)
	ESOS_TASK_END();
} // end usart_comms()

/************************************************************************
 * User supplied functions
 ************************************************************************
 */
void __user_init_hw(void) {
  /* GPIO Ports Clock Enable */
  rcc_periph_clock_enable(GPIOA);
  rcc_periph_clock_enable(GPIOC);

  /*Configure GPIO pin : PC13 */
  gpio_mode_setup(B1_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, B1_Pin);

  /*Configure GPIO pin : LED2_Pin */
  gpio_mode_setup(LD2_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LD2_Pin);

}



/******************************************************************************
 * Function:        void user_init(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        user_init is a centralized initialization routine where
 *                  the user can setup their application.   It is called
 *                  automagically by ES_OS during the operating system
 *                  initialization.
 *
 * Note:            The user should set up any state machines and init
 *                  all application variables.  They can also turn on
 *                  any needed peripherals here.
 *
 *                  The user SHALL NOT mess with the interrupt hardware
 *                  directly!!!  The ES_OS must be aware of the interrupts
 *                  and provides osXXXXXXX functions for the user to use.
 *                  Using these ES_OS-provided functions, the user may
 *                  (and probably should) initialize, register, and enable
 *                  interrupts in this routine.
 *
 *                  Furthermore, the user should register AT LEAST one
 *                  user application task here (via esos_RegisterTask) or
 *                  the ES_OS scheduler will have nothing to schedule
 *                  to run when this function returns.
 *
 *****************************************************************************/
void user_init(void) {

  // setup our software structures here
  
  // call our function to setup the hardware we like it.
  __user_init_hw();

  // Drive the UART directly.....  to print the HELLO_MSG
  __esos_unsafe_PutString( HELLO_MSG );

  /*
   * Now, let's get down and dirty with ESOS and our user tasks
   *
   *   Once tasks are registered, they will start executing in
   *   the ESOS scheduler.
   */
  esos_RegisterTask( flash_LED );
  esos_RegisterTask( usart_comms );

} // end user_init()

