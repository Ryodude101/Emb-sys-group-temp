/*
 * Indigo Team
 * Embedded Systems Milestone 4
 * 03.22.22
 * Task Synchronization applet for Milestone 4.
 */

#include "queue.h"

//Globals
ESOS_SEMAPHORE(product);
bool PRODUCER1 = false;
bool PRODUCER2 = false;
bool PRODUCER3 = false;
bool PRODUCER4 = false;
bool PRODUCER5 = false;
bool PRODUCER6 = false;
bool CONSUMER1 = false;
bool CONSUMER2 = false;
bool CONSUMER3 = false;
bool CONSUMER4 = false;
bool CONSUMER5 = false;
bool CONSUMER6 = false;

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
    ESOS_TASK_WAIT_WHILE(CONSUMER1 == false);
    ESOS_TASK_WAIT_SEMAPHORE(product, 1);
    --u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 1 consumed a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer2){
  ESOS_TASK_BEGIN();
  
  while(true){
    ESOS_TASK_WAIT_WHILE(CONSUMER2 == false);
    ESOS_TASK_WAIT_SEMAPHORE(product, 1);
    --u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 2 consumed a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer3){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_WHILE(CONSUMER3 == false);
    ESOS_TASK_WAIT_SEMAPHORE(product, 1);
    --u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 3 consumed a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer4){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_WHILE(CONSUMER4 == false);
    ESOS_TASK_WAIT_SEMAPHORE(product, 1);
    --u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 4 consumed a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer5){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_WHILE(CONSUMER5 == false);
    ESOS_TASK_WAIT_SEMAPHORE(product, 1);
    --u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 5 consumed a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(consumer6){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_WHILE(CONSUMER6 == false);
    ESOS_TASK_WAIT_SEMAPHORE(product, 1);
    --u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Consumer 6 consumed a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(producer1){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_WHILE(PRODUCER1 == false);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_SIGNAL_SEMAPHORE(product, 1);
    ++u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Producer 1 produced a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(producer2){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_WHILE(PRODUCER2 == false);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_SIGNAL_SEMAPHORE(product, 1);
    ++u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Producer 2 produced a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(producer3){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_WHILE(PRODUCER3 == false);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_SIGNAL_SEMAPHORE(product, 1);
    ++u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Producer 3 produced a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(producer4){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_WHILE(PRODUCER4 == false);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_SIGNAL_SEMAPHORE(product, 1);
    ++u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Producer 4 produced a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(producer5){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_WHILE(PRODUCER5 == false);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_SIGNAL_SEMAPHORE(product, 1);
    ++u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Producer 5 produced a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(producer6){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_WHILE(PRODUCER6 == false);
    ESOS_TASK_WAIT_TICKS((esos_GetRandomUint32() >> 28) * 1000);
    ESOS_SIGNAL_SEMAPHORE(product, 1);
    ++u8_numOfProducts;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Producer 6 produced a product\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(product_report){
  ESOS_TASK_BEGIN();
  while(true){
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("Product Report: ");
    ESOS_TASK_WAIT_ON_SEND_UINT8_AS_DEC_STRING(u8_numOfProducts);
    ESOS_TASK_WAIT_ON_SEND_STRING(" products\n\r");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_TICKS(2000);
  }
  ESOS_TASK_END();
}

ESOS_CHILD_TASK(create_consumer_producers, const uint8_t u8_numP, const uint8_t u8_numC){
  ESOS_TASK_BEGIN();

  static uint8_t u8_numProducers;
  static uint8_t u8_numConsumers;

  u8_numProducers = u8_numP;
  u8_numConsumers = u8_numC;

  if(u8_numProducers == 6)
    PRODUCER6 = true;
  if(u8_numProducers >= 5)
    PRODUCER5 = true;
  if(u8_numProducers >= 4)
    PRODUCER4 = true;
  if(u8_numProducers >= 3)
    PRODUCER3 = true;
  if(u8_numProducers >= 2)
    PRODUCER2 = true;
  if(u8_numProducers >= 1)
    PRODUCER1 = true;

  if(u8_numConsumers == 6)
    CONSUMER6 = true;
  if(u8_numConsumers >= 5)
    CONSUMER5 = true;
  if(u8_numConsumers >= 4)
    CONSUMER4 = true;
  if(u8_numConsumers >= 3)
    CONSUMER3 = true;
  if(u8_numConsumers >= 2)
    CONSUMER2 = true;
  if(u8_numConsumers >= 1)
    CONSUMER1 = true;

  ESOS_TASK_END();
}

ESOS_USER_TASK(Synchronize){
  ESOS_TASK_BEGIN();

  static ESOS_TASK_HANDLE th_child;

  static uint8_t u8_numProducers = 3;
  static uint8_t u8_numConsumers = 3;
  

  ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();

  ESOS_TASK_WAIT_ON_SEND_STRING("Please enter the number of producers (1-6): ");
  ESOS_TASK_WAIT_ON_GET_UINT8(u8_numProducers);
  ESOS_TASK_WAIT_ON_SEND_UINT8(u8_numProducers);
  ESOS_TASK_WAIT_ON_SEND_STRING("\n\r");
  if(u8_numProducers <= '0' || u8_numProducers >= '7')
    ESOS_TASK_WAIT_ON_SEND_STRING("Invalid number, defaulting to 3\n\r");
  else
    u8_numProducers -= '0';

  ESOS_TASK_WAIT_ON_SEND_STRING("Please enter the number of Consumers (1-6): ");
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
  ESOS_TASK_SPAWN_AND_WAIT(th_child, create_consumer_producers, u8_numProducers, u8_numConsumers);

  ESOS_TASK_END();
}

void user_init(void){
  init_hw();
  init_sw();
  esos_RegisterTask(led_heartbeat);
  esos_RegisterTask(Synchronize);
  esos_RegisterTask(product_report);
  esos_RegisterTask(producer1);
  esos_RegisterTask(producer2);
  esos_RegisterTask(producer3);
  esos_RegisterTask(producer4);
  esos_RegisterTask(producer5);
  esos_RegisterTask(producer6);
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
  ESOS_INIT_SEMAPHORE(product, 0);
}