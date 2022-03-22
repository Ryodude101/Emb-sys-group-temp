// HINT for TASK #2 -- FALL 2022

#ifndef EMBSYSS20_H
#define EMBSYSS20_H

#ifdef __cplusplus
extern "C" {
#endif

//************** I N C L U D E S *****************

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include "embsysS20.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/timer.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


//************** G L O B A L S *****************

#define LD2_Pin GPIO5 // Connected to PA5
#define LD2_Port GPIOA
#define B1_Pin GPIO13 // Connected to PC13
#define B1_Port GPIOC


//********* S U P P O R T     F U N C T I O N S *****************

static void TURN_ON_LD2(void){
	gpio_set(LD2_Port, LD2_Pin);
}

static void TURN_OFF_LD2(void){
	gpio_clear(LD2_Port, LD2_Pin);
}

static void TOGGLE_LD2(void){
	gpio_toggle(LD2_Port, LD2_Pin);
}

static bool IS_LD2_SET(void){
	if (gpio_get(LD2_Port, LD2_Pin) == 1)
		return true;
	else
		return false;
}

static bool IS_LD2_RESET(void){
	if (gpio_get(LD2_Port, LD2_Pin) == 0)
		return true;
	else
		return false;
}

static bool IS_B1_RELEASED(void){
	if (gpio_get(B1_Port, B1_Pin) > 0)
		return true;
	else
		return false;
}

static bool IS_B1_PRESSED(void){
	if (!IS_B1_RELEASED())
		return true;
	else
		return false;
}







 
#ifdef __cplusplus
}
#endif

#endif /* EMBSYSS20_H */
