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
#else
#include    "esos_stm32l4.h"
#include    "esos_stm32l4_rs232.h"
#endif

// Defines
#define DECODE_FLASH 62 // Half of 0.125 s period for decoding flash rounded down to avoid floats
#define ENCODE_FLASH 500 // Half of 0.5 s period for encoding flash
#define SPACE_INPUT 0x00 // ASCII for [space] input

void __user_init_hw( void );


// GLOBALs go here
//  Generally, the user-created semaphores will be defined/allocated here
//static uint8_t psz_CRNL[3]= {0x0D, 0x0A, 0};

// timer globals

#ifdef _linux
#endif

ESOS_USER_TASK( flash_LED ) 
{
	ESOS_TASK_BEGIN();
	while (TRUE) 
	{
		TOGGLE_LD2();
		if (IS_B1_RELEASED())
		{
			ESOS_TASK_WAIT_TICKS( ENCODE_FLASH );
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

	ESOS_TASK_BEGIN();
	while (TRUE) {
	
	// ESOS UART comms uses 57.6k/8/N/1
		ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
		ESOS_TASK_WAIT_ON_GET_UINT8( u8_incoming );
		ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

		if (u8_incoming >= 'a' && u8_incoming <='z') // Capitalizes if input is a letter
			u8_incoming = u8_incoming - 'a' + 'A';
			
		// FIXME: Here, input processing should be added (cipher and user command system)
			

		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_UINT8( u8_incoming );
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

		} // endof while(TRUE)
	ESOS_TASK_END();
} // end echo()

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

