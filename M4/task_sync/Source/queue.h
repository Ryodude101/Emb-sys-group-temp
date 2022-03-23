#ifndef __QUEUE_H
#define __QUEUE_H

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
    #include <stdlib.h>
    #else
    #include    "esos_stm32l4.h"
    #include    "esos_stm32l4_rs232.h"
    #endif

    #define LED2_Pin GPIO5  //Pin number for PA5, the LED2 pin, can be found in nucleo board 64 user manual
    #define LED2_GPIO_Port GPIOA  //PORTA address, can be found in libopencm3 docs

    /*#define PRODUCER1 ESOS_USER_FLAG_0
    #define PRODUCER2 ESOS_USER_FLAG_1
    #define PRODUCER3 ESOS_USER_FLAG_2
    #define PRODUCER4 ESOS_USER_FLAG_3
    #define PRODUCER5 ESOS_USER_FLAG_4
    #define PRODUCER6 ESOS_USER_FLAG_5
    #define CONSUMER1 ESOS_USER_FLAG_6
    #define CONSUMER2 ESOS_USER_FLAG_7
    #define CONSUMER3 ESOS_USER_FLAG_8
    #define CONSUMER4 ESOS_USER_FLAG_9
    #define CONSUMER5 ESOS_USER_FLAG_10
    #define CONSUMER6 ESOS_USER_FLAG_11*/

    uint8_t u8_numOfProducts = 0;

    void init_hw();
    void init_sw();

#endif