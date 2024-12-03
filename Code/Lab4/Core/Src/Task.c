/*
 * Task.c
 *
 *  Created on: Nov 19, 2024
 *      Author: Mr Duc Anh
 */

#include"Task.h"
#include "main.h"


void myFunction1(void){
	HAL_GPIO_TogglePin( Led_RED_GPIO_Port, Led_RED_Pin);
}

void myFunction2(void){
	HAL_GPIO_TogglePin( Led_Yellow_GPIO_Port, Led_Yellow_Pin);
}

void myFunction3(void){
	HAL_GPIO_TogglePin( Led_Green_GPIO_Port, Led_Green_Pin);
}


void (*Task1)(void) = myFunction1;
void (*Task2)(void) = myFunction2; // Hoặc một hàm khác
void (*Task3)(void) = myFunction3;
