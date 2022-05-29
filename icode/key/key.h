/*
 * key.h
 *
 *  Created on: May 6, 2022
 *      Author: Angel
 */

#ifndef KEY_KEY_H_
#define KEY_KEY_H_

#include "stm32f1xx_hal.h" //HAL库文件声明
#include "main.h" //IO定义与初始化函数在main.c文件中，必须引用

uint8_t K2(void);//按键1
uint8_t K3(void);//按键2

#endif /* KEY_KEY_H_ */
