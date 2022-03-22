/*
 * Ryan Colon
 * Embedded Systems Milestone 3
 * 02.22.22
 * Program takes user input from the serial monitor and encrypts the text
 * While running, if the user holds B1, the output will switch to decrypt from encrypt.
 * This program also adds the functionality to change the led blink speed.
 */

#include "embsysS20.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

void fn_handleChar(char c_toHandle);
void fn_encDecChar(char c_toDo);
void fn_commitPeriod(char* ac_toCommit);

//Available on the libopencm3 github example page
//Essentially makes the USART2 port the STDOUT stream
int _write(int file, char* ptr, int len){
    int i;

    if (file == STDOUT_FILENO || file == STDERR_FILENO){
	    for(i = 0; i < len; i++){
		    if(ptr[i] == '\n')
			    usart_send_blocking(USART_CONSOLE, '\r');

		    usart_send_blocking(USART_CONSOLE, ptr[i]);
	    }

	    return i;
    }

    errno = EIO;
    return -1;
}

// Below is a standard template for just about any embedded
//  system main() loop. 
int main(void)  {
  // setup software structures need for application
  fn_circBuffInit(&st_circularBufferInput, bufferI, BUFFER_SIZE);
  fn_circBuffInit(&st_circularBufferOutput, bufferO, BUFFER_SIZE);

  // initialize the hardware
  fn_clock_setup();
  fn_usart_setup(); 
  fn_setup_tim2();

  gpio_mode_setup(LED2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED2_Pin);
  gpio_mode_setup(BUT1_GPIO_Port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, BUT1_Pin);

  LED_OFF;

  while(true){
    if(!fn_circBuffEmpty(&st_circularBufferInput)){
      char c_charToSend = '\0';
      fn_circBuffGet(&st_circularBufferInput, &c_charToSend);
      fn_handleChar(c_charToSend);
    }
  }
  return 0;    // NEVER EXECUTES
} 

/*
 * Handling the incoming characters for this program can become very complex, there are a lot of cases upon cases. This function has been made to handle ANY
 * case at all. For each case it will do the bare minimum to keep itself aware of what is going on and then call corresponding functions to actually deal
 * with the incoming character appropriately. 
 */
void fn_handleChar(char c_toHandle){
  static uint8_t u8_case = 0;
  static char ac_period[4] = {'1','0','0','0'};
  static char ac_lastPeriod[4] = {'1','0','0','0'};

  // Consider past characters and context to determine current character context
  if(c_toHandle == '!' && u8_case != 3)
    ++u8_case;
  else if(u8_case == 3 && c_toHandle == 'L')
    u8_case = 4;
  else if(u8_case == 3 && c_toHandle == 'S')
    u8_case = 5;
  else if(u8_case == 3 && c_toHandle == '!'){
    u8_case = 10;
  }
  else if(u8_case < 6)
    u8_case = 0;
  else if(u8_case >= 6 &&(c_toHandle < 0x30 || c_toHandle > 0x39)){
    u8_case = 0;
    printf("Invalid period, change not commited.\n");
    for(int i = 0; i < 4; i++)
      ac_period[i] = ac_lastPeriod[i];
    return;
  }

  //Act upon the incoming character based on it's context
  switch(u8_case){
    case 0:
      fn_encDecChar(c_toHandle);
      break;
    case 1:
      fn_circBuffPut(&st_circularBufferOutput, c_toHandle);
      usart_enable_tx_interrupt(USART_CONSOLE);
      break;
    case 2:
      fn_circBuffPut(&st_circularBufferOutput, c_toHandle);
      usart_enable_tx_interrupt(USART_CONSOLE);
      break;
    case 3:
      fn_circBuffPut(&st_circularBufferOutput, c_toHandle);
      usart_enable_tx_interrupt(USART_CONSOLE);
      break;
    case 4:
      fn_circBuffPut(&st_circularBufferOutput, c_toHandle);
      for(int i = 0; i < 4; i++)
        fn_circBuffPut(&st_circularBufferOutput, ac_period[i]);  
      usart_enable_tx_interrupt(USART_CONSOLE);
      u8_case = 0;
      break;
    case 5:
      ++u8_case;
      fn_circBuffPut(&st_circularBufferOutput, c_toHandle);
      usart_enable_tx_interrupt(USART_CONSOLE);
      break;
    case 6:
      ++u8_case;
      ac_period[0] = c_toHandle;
      fn_circBuffPut(&st_circularBufferOutput, c_toHandle);
      usart_enable_tx_interrupt(USART_CONSOLE);
      break;
    case 7:
      ++u8_case;
      ac_period[1] = c_toHandle;
      fn_circBuffPut(&st_circularBufferOutput, c_toHandle);
      usart_enable_tx_interrupt(USART_CONSOLE);
      break;
    case 8:
      ++u8_case;
      ac_period[2] = c_toHandle;
      fn_circBuffPut(&st_circularBufferOutput, c_toHandle);
      usart_enable_tx_interrupt(USART_CONSOLE);
      break;
    case 9:
      u8_case = 0;
      ac_period[3] = c_toHandle;
      fn_circBuffPut(&st_circularBufferOutput, c_toHandle);
      fn_commitPeriod(ac_period);
      for(int i = 0; i < 4; i++)
        ac_lastPeriod[i] = ac_period[i];
      usart_enable_tx_interrupt(USART_CONSOLE);
      break;
    case 10:
      u8_case = 3;
      fn_circBuffPut(&st_circularBufferOutput, c_toHandle);
      usart_enable_tx_interrupt(USART_CONSOLE);
      break;
  }  
}

//Function to either encrypt/decrypt char and then stage to print it
void fn_encDecChar(char c_toDo){
  fn_convertToCaps(&c_toDo);

  if(IS_B1_PRESSED) //DECRYPT
      c_toDo = fn_decryptText(c_toDo);

  else //ENCRYPT
      c_toDo = fn_encryptText(c_toDo);

  fn_circBuffPut(&st_circularBufferOutput, c_toDo);
  /* Enable transmit interrupt so it sends back the data. */
  usart_enable_tx_interrupt(USART_CONSOLE);
}

//Function sets the period of the LED
void fn_commitPeriod(char* ac_toCommit){
  char ac_convertThis[5] = {'0','0','0','0','\0'};

  for(int i = 0; i < 4; i++)
    ac_convertThis[i] = ac_toCommit[i];

  uint16_t u16_period = atoi(ac_convertThis);

  u16_globalPeriod = u16_period;
}