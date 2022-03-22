/*
 * Indigo Team
 * Embedded Systems Milestone 4
 * 03.22.22
 * Task Synchronization applet for Milestone 4.
 */

// INCLUDEs go here  (First include the main esos.h file)
//      After that, the user can include what they need
#include    "esos.h"
#ifdef __linux
#include    "esos_pc.h"
#include    "esos_pc_stdio.h"
// INCLUDE these so our printf and other PC hacks work
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <string.h>
#include <unistd.h>
#else
#include    "esos_stm32l4.h"
#include    "esos_stm32l4_rs232.h"
#endif
#include "barrier.h"

//Globals
ESOS_SEMAPHORE(sync1Ready);
ESOS_SEMAPHORE(sync2Ready);

ESOS_USER_TASK(led_heartbeat){
  ESOS_TASK_BEGIN();
  while (TRUE) 
  {
    gpio_toggle(LED2_GPIO_Port,LED2_Pin);
    ESOS_TASK_WAIT_TICKS();
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(sync1){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_SIGNAL_SEMAPHORE(sync1Ready, 1);
    ESOS_TASK_WAIT_SEMAPHORE(sync2Ready, 1);
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Hello Worl\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_TICKS(1000);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(sync2){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_SIGNAL_SEMAPHORE(sync2Ready, 1);
    ESOS_TASK_WAIT_SEMAPHORE(sync1Ready, 1);
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Goodbye Cruel Worl\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_TICKS(1000);
  }
  ESOS_TASK_END();
}

void user_init(void){
  init_hw();
  init_sw();
  esos_RegisterTask(led_heartbeat);
  esos_RegisterTask(sync1);
  esos_RegisterTask(sync2); 
}

void init_hw(){
  rcc_periph_clock_enable(GPIOA);
  gpio_mode_setup(LED2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED2_Pin);
}

void init_sw(){
  ESOS_INIT_SEMAPHORE(sync1Ready, 0);
  ESOS_INIT_SEMAPHORE(sync2Ready, 0);
}