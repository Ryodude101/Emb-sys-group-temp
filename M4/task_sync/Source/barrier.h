#ifndef __BARRIER_H
#define __BARRIER_H

    #define LED2_Pin GPIO5  //Pin number for PA5, the LED2 pin, can be found in nucleo board 64 user manual
    #define LED2_GPIO_Port GPIOA  //PORTA address, can be found in libopencm3 docs

    int numOfTask = 0;

    void init_hw();
    void init_sw();

#endif