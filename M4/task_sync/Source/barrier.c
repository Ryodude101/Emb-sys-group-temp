/*
 * Indigo Team
 * Embedded Systems Milestone 4
 * 03.22.22
 * Task Synchronization applet for Milestone 4.
 */

#include "barrier.h"

//Globals
ESOS_SEMAPHORE(threads);
ESOS_SEMAPHORE(Consumer1);
ESOS_SEMAPHORE(Consumer2);
ESOS_SEMAPHORE(Consumer3);
ESOS_SEMAPHORE(Consumer4);
ESOS_SEMAPHORE(Consumer5);
ESOS_SEMAPHORE(Consumer6);

ESOS_USER_TASK(led_heartbeat){
  ESOS_TASK_BEGIN();
  while (TRUE) 
  {
    gpio_toggle(LED2_GPIO_Port,LED2_Pin);
    ESOS_TASK_WAIT_TICKS(500);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer1){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 1 finished task\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_SIGNAL_SEMAPHORE(Consumer1, 5);
    ESOS_SIGNAL_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer2, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer3, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer4, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer5, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer6, 1);
    ++u8_passed_programs;
    if(u8_passed_programs == 6){
      u8_passed_programs = 0;
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_WAIT_ON_SEND_STRING("New Cycle\n\r");
      ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    }
    ESOS_TASK_WAIT_TICKS(100);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer2){
  ESOS_TASK_BEGIN();
  
  while(true){
    ESOS_TASK_WAIT_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 2 finished task\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_SIGNAL_SEMAPHORE(Consumer2, 5);
    ESOS_SIGNAL_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer1, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer3, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer4, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer5, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer6, 1);
    ++u8_passed_programs;
    if(u8_passed_programs == 6){
      u8_passed_programs = 0;
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_WAIT_ON_SEND_STRING("New Cycle\n\r");
      ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    }
    ESOS_TASK_WAIT_TICKS(100);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer3){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 3 finished task\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_SIGNAL_SEMAPHORE(Consumer3, 5);
    ESOS_SIGNAL_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer1, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer2, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer4, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer5, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer6, 1);
    ++u8_passed_programs;
    if(u8_passed_programs == 6){
      u8_passed_programs = 0;
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_WAIT_ON_SEND_STRING("New Cycle\n\r");
      ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    }
    ESOS_TASK_WAIT_TICKS(100);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer4){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 4 finished task\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_SIGNAL_SEMAPHORE(Consumer4, 5);
    ESOS_SIGNAL_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer1, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer2, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer3, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer5, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer6, 1);
    ++u8_passed_programs;
    if(u8_passed_programs == 6){
      u8_passed_programs = 0;
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_WAIT_ON_SEND_STRING("New Cycle\n\r");
      ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    }
    ESOS_TASK_WAIT_TICKS(100);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer5){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 5 finished task\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_SIGNAL_SEMAPHORE(Consumer5, 5);
    ESOS_SIGNAL_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer1, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer2, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer3, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer4, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer6, 1);
    ++u8_passed_programs;
    if(u8_passed_programs == 6){
      u8_passed_programs = 0;
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_WAIT_ON_SEND_STRING("New Cycle\n\r");
      ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    }
    ESOS_TASK_WAIT_TICKS(100);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer6){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 6 finished task\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_SIGNAL_SEMAPHORE(Consumer6, 5);
    ESOS_SIGNAL_SEMAPHORE(threads, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer1, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer2, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer3, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer4, 1);
    ESOS_TASK_WAIT_SEMAPHORE(Consumer5, 1);
    ++u8_passed_programs;
    if(u8_passed_programs == 6){
      u8_passed_programs = 0;
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
      ESOS_TASK_WAIT_ON_SEND_STRING("New Cycle\n\r");
      ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    }
    ESOS_TASK_WAIT_TICKS(100);
  }
  ESOS_TASK_END();
}

ESOS_CHILD_TASK(create_consumer_producers, const uint8_t u8_numC){
  ESOS_TASK_BEGIN();
u8_max_threads = u8_numC;

  ESOS_SIGNAL_SEMAPHORE(threads, u8_numC);

  ESOS_TASK_END();
}

ESOS_USER_TASK(Initialize){
  ESOS_TASK_BEGIN();

  static ESOS_TASK_HANDLE th_child;
  static uint8_t u8_numConsumers = 3;
  

  ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();

  ESOS_TASK_WAIT_ON_SEND_STRING("Please enter the max number of Threads (1-6): ");
  ESOS_TASK_WAIT_ON_GET_UINT8(u8_numConsumers);
  ESOS_TASK_WAIT_ON_SEND_UINT8(u8_numConsumers);
  ESOS_TASK_WAIT_ON_SEND_STRING("\n\r");
  if(u8_numConsumers <= '0' || u8_numConsumers >= '7')
    ESOS_TASK_WAIT_ON_SEND_STRING("Invalid number, defaulting to 3\n\r");
  else
    u8_numConsumers -= '0';


  ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();
  ESOS_ALLOCATE_CHILD_TASK(th_child);
  ESOS_TASK_SPAWN_AND_WAIT(th_child, create_consumer_producers, u8_numConsumers);

  ESOS_TASK_END();
}

void user_init(void){
  init_hw();
  init_sw();
  esos_RegisterTask(led_heartbeat);
  esos_RegisterTask(Initialize);
  esos_RegisterTask(consumer1);
  esos_RegisterTask(consumer2);
  esos_RegisterTask(consumer3);
  esos_RegisterTask(consumer4);
  esos_RegisterTask(consumer5);
  esos_RegisterTask(consumer6);
}

void init_hw(){
  rcc_periph_clock_enable(GPIOA);
  gpio_mode_setup(LED2_GPIO_Port, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, LED2_Pin);
}

void init_sw(){
  ESOS_INIT_SEMAPHORE(threads, 0);
  ESOS_INIT_SEMAPHORE(Consumer1, 0);
  ESOS_INIT_SEMAPHORE(Consumer2, 0);
  ESOS_INIT_SEMAPHORE(Consumer3, 0);
  ESOS_INIT_SEMAPHORE(Consumer4, 0);
  ESOS_INIT_SEMAPHORE(Consumer5, 0);
  ESOS_INIT_SEMAPHORE(Consumer6, 0);
}