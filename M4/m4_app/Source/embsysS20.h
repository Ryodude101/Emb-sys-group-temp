#ifndef embsysS20_H
    #define embsysS20_H

    #ifndef STM32L4
        #define STM32L4
    #endif

    #include <libopencm3/stm32/rcc.h>
    #include <libopencm3/stm32/gpio.h>
    #include <libopencm3/stm32/usart.h>
    #include <libopencm3/cm3/nvic.h>
    #include <libopencm3/stm32/timer.h>
    #include "circularBuffer.h"

    #define LED2_Pin GPIO5  //Pin number for PA5, the LED2 pin, can be found in nucleo board 64 user manual
    #define LED2_GPIO_Port GPIOA  //PORTA address, can be found in libopencm3 docs
    #define BUT1_GPIO_Port GPIOC  //PORTC address, can be found in libopencm3 docs
    #define BUT1_Pin GPIO13  //Pin number for PC2, the B1 pin, can be found in the nucleo board 64 user manual

    #define USART_CONSOLE USART2 //PA2 (tx) and PA3 (rx), STM32-L452RE Reference Manual
    #define USART_GPIO_Port GPIOA
    #define USART_GPIO_TXPin GPIO2
    #define USART_GPIO_RXPin GPIO3

    #define TOGGLE_LED gpio_toggle(LED2_GPIO_Port,LED2_Pin)
    #define LED_ON gpio_set(LED2_GPIO_Port,LED2_Pin)
    #define LED_OFF gpio_clear(LED2_GPIO_Port,LED2_Pin)
    #define IS_LD2_SET  (gpio_get(LED2_GPIO_Port,LED2_Pin))
    #define IS_LD2_RESET  ((IS_LD2_SET)^LED2_Pin)
    #define IS_B1_RELEASED (gpio_get(BUT1_GPIO_Port,BUT1_Pin))
    #define IS_B1_PRESSED ((IS_B1_RELEASED)^BUT1_Pin)

    #define BUFFER_SIZE 256
    #define CIPHER_SIZE 13

    const char ac_CIPHER[CIPHER_SIZE] = {'T','E','N','N','E','S','S','E','E','T','E','C','H'};

    struct st_circBuff st_circularBufferInput;
    struct st_circBuff st_circularBufferOutput;
    char bufferI[BUFFER_SIZE];
    char bufferO[BUFFER_SIZE];

    uint16_t u16_globalPeriod = 1000;

    //Will only convert a character to caps if it is in the alphabet and lowercase, stores in the param it received
    void fn_convertToCaps(char* pc_Convert){
    if(*pc_Convert >= 'a' && *pc_Convert <= 'z')
        *pc_Convert = (*pc_Convert - 'a') + 'A';  
    }

    //Encrypt using the cipher and return the encrypted key
    char fn_encryptText(char c_toEncrypt){
    static uint8_t u8_cipherEPosition = 0;

    c_toEncrypt = (((c_toEncrypt - 'A') + (ac_CIPHER[u8_cipherEPosition] - 'A')) % 26) + 'A';
    ++u8_cipherEPosition;

    u8_cipherEPosition = (u8_cipherEPosition == CIPHER_SIZE) ? 0 : u8_cipherEPosition;

    return c_toEncrypt;
    }

    //Decrypt using the cipher and return the decrypted key
    char fn_decryptText(char c_toDecrypt){
    static uint8_t u8_cipherDPosition = 0;

    c_toDecrypt = ((c_toDecrypt - ac_CIPHER[u8_cipherDPosition] + 26) % 26) + 'A';

    ++u8_cipherDPosition;

    u8_cipherDPosition = (u8_cipherDPosition == CIPHER_SIZE) ? 0 : u8_cipherDPosition;
    
    return c_toDecrypt;
    }
    
    void fn_clock_setup(void){
        rcc_osc_on(RCC_HSI16);

        //16 MHz / 4 => 4 * 20 = 80 MHz / 2 = 40 MHz
        rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, 4, 40, 0, 0, RCC_PLLCFGR_PLLR_DIV2);

        rcc_osc_on(RCC_PLL);

        rcc_periph_clock_enable(RCC_TIM2);
        rcc_periph_clock_enable(RCC_GPIOA);
	    rcc_periph_clock_enable(RCC_GPIOC);
	    rcc_periph_clock_enable(RCC_USART2);
	    return;
    }

    void fn_usart_setup(void){
        nvic_enable_irq(NVIC_USART2_IRQ);

        /* Setup GPIO pins for USART */
         gpio_mode_setup(USART_GPIO_Port, GPIO_MODE_AF, GPIO_PUPD_NONE, USART_GPIO_TXPin | USART_GPIO_RXPin);
        
         /* Setup USART pins as alternate function. */
         gpio_set_af(USART_GPIO_Port, GPIO_AF7, USART_GPIO_TXPin | USART_GPIO_RXPin); //AF7 = USART2, STM32L54RE Family Datasheet Table

         /* Setup UART parameters. */
         usart_set_baudrate(USART_CONSOLE, 57600);
         usart_set_databits(USART_CONSOLE, 9);  // check the effect of parity on this fcn.
         usart_set_stopbits(USART_CONSOLE, USART_STOPBITS_1);
         usart_set_mode(USART_CONSOLE, USART_MODE_TX_RX);
         usart_set_parity(USART_CONSOLE, USART_PARITY_NONE);
         usart_set_flow_control(USART_CONSOLE, USART_FLOWCONTROL_NONE);

         usart_enable_rx_interrupt(USART_CONSOLE);

         /* everything is setup, so turn UART on */  
         usart_enable(USART_CONSOLE);
    }

    void usart2_isr(void){
        /* Check if we were called because of RXNE. */
        if (((USART_CR1(USART_CONSOLE) & USART_CR1_RXNEIE) != 0) && (usart_get_flag(USART_CONSOLE,USART_ISR_RXNE))){
            /* Retrieve the data from the peripheral. */
            fn_circBuffPut(&st_circularBufferInput, usart_recv(USART_CONSOLE));
        }

        /* Check if we were called because of TXE. */
        if (((USART_CR1(USART_CONSOLE) & USART_CR1_TXEIE) != 0) && (usart_get_flag(USART_CONSOLE,USART_ISR_TXE))){
            char c_charToSend = '\0';
            fn_circBuffGet(&st_circularBufferOutput, &c_charToSend);
            /* Put data into the transmit register to send data */
            usart_send(USART_CONSOLE, c_charToSend);
            /* Disable the TXE interrupt as we don't need it anymore. */
            if(fn_circBuffEmpty(&st_circularBufferOutput))
                usart_disable_tx_interrupt(USART_CONSOLE);
        }
    }

    void fn_setup_tim2(){
        nvic_enable_irq(NVIC_TIM2_IRQ);
        rcc_periph_reset_pulse(RST_TIM2);
	    timer_set_prescaler(TIM2, 2000);
        timer_set_period(TIM2, 1000);
	    timer_enable_counter(TIM2);
	    timer_enable_irq(TIM2, TIM_DIER_CC1IE);
    }

    void tim2_isr(void){
        if(timer_get_flag(TIM2,TIM_SR_CC1IF)){
            timer_set_period(TIM2, u16_globalPeriod);
            timer_clear_flag(TIM2, TIM_SR_CC1IF);
            
            TOGGLE_LED;
        }
}
#endif
