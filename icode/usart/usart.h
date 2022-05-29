/*
 * usart1.h
 *
 *  Created on: Oct 20, 2021
 *      Author: Administrator
 */

#ifndef INC_USART_H_
#define INC_USART_H_

#include "stm32f1xx_hal.h" //HAL库文件声明
#include "main.h" //IO定义与初始化函数在main.c文件中，必须引用
#include <string.h>//�����ַ�������Ŀ�


extern UART_HandleTypeDef huart1;//����USART1��HAL��ṹ��


#define USART1_REC_LEN  200//����USART1�������ֽ���


extern uint8_t  USART1_RX_BUF[USART1_REC_LEN];//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern uint16_t USART1_RX_STA;//����״̬���
extern uint8_t USART1_NewData;//��ǰ�����жϽ��յ�1���ֽ����ݵĻ���



void  HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart);//�����жϻص���������

#endif /* INC_USART_H_ */
