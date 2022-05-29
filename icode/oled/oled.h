/*
 * oled.h
 *
 *  Created on: May 12, 2022
 *      Author: Angel
 */

#ifndef OLED_OLED_H_
#define OLED_OLED_H_

#define OLED0561_ADD	0x78  // OLED的I2C地址（禁止修改）
#define COM				0x00  // OLED 指令（禁止修改）
#define DAT 			0x40  // OLED 数据（禁止修改）

#include "stm32f1xx_hal.h" //HAL库文件声明
#include "main.h" //IO定义与初始化函数在main.c文件中，必须引用


void OLED0561_Init(void);//初始化
void OLED_DISPLAY_ON (void);//OLED屏开显示
void OLED_DISPLAY_OFF (void);//OLED屏关显示
void OLED_DISPLAY_LIT (uint8_t x);//OLED屏亮度设置（0~255）
void OLED_DISPLAY_CLEAR(void);//清屏操作
void OLED_DISPLAY_8x16(uint8_t x,uint8_t y,uint16_t w);//显示8x16的单个字符
void OLED_DISPLAY_SJIS_8x8(uint8_t x,uint8_t y,uint16_t w);//显示8x8的单个日文字符
void OLED_DISPLAY_8x16_BUFFER(uint8_t row,uint8_t *str);//显示8x16的字符串
void I2C_SAND_BUFFER(uint8_t SlaveAddr,uint8_t WriteAddr,uint8_t* pBuffer,uint16_t NumByteToWrite);
void I2C_SAND_BYTE(uint8_t SlaveAddr,uint8_t writeAddr,uint8_t pBuffer);
void I2C_READ_BUFFER(uint8_t SlaveAddr,uint8_t WriteAddr,uint8_t* pBuffer,uint16_t NumByteToWrite);
uint8_t I2C_READ_BYTE(uint8_t SlaveAddr,uint8_t readAddr);


#endif /* OLED_OLED_H_ */
