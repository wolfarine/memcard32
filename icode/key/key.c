/*
 * key.c
 *
 *  Created on: May 6, 2022
 *      Author: Angel
 */


#include "key.h"


uint8_t K2(void)
{
	uint8_t a;
	a=0;//如果未进入按键处理，则返回0
	if(HAL_GPIO_ReadPin(GPIOA,K2_Pin)==GPIO_PIN_RESET){//读按键接口的电平
		HAL_Delay(15);//延时去抖动
		if(HAL_GPIO_ReadPin(GPIOA,K2_Pin)==GPIO_PIN_RESET){ //读按键接口的电平
			a=1;//进入按键处理，返回1
		}
	}
	while(HAL_GPIO_ReadPin(GPIOA,K2_Pin)==GPIO_PIN_RESET); //等待按键松开
	return a;
}

uint8_t K3(void)
{
	uint8_t a;
	a=0;//如果未进入按键处理，则返回0
	if(HAL_GPIO_ReadPin(GPIOA,K3_Pin)==GPIO_PIN_RESET){//读按键接口的电平
		HAL_Delay(15);//延时去抖动
		if(HAL_GPIO_ReadPin(GPIOA,K3_Pin)==GPIO_PIN_RESET){ //读按键接口的电平
			a=1;//进入按键处理，返回1
		}
	}
	while(HAL_GPIO_ReadPin(GPIOA,K3_Pin)==GPIO_PIN_RESET); //等待按键松开
	return a;
}
