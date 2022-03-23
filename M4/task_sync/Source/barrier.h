#ifndef __BARRIER_H
#define __BARRIER_H

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

    uint8_t u8_max_threads = 3;
    uint8_t u8_passed_programs = 0;

    void init_hw();
    void init_sw();

#endif