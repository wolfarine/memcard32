/*
 * usart1.c
 *
 *  Created on: Oct 20, 2021
 *      Author: Administrator
 */

#include "usart.h"

uint8_t USART1_RX_BUF[USART1_REC_LEN];
uint16_t USART1_RX_STA=0;


extern uint8_t MENU;
extern uint8_t USART1_NewData;

//Commands
#define GETID 0xA0          //Get identifier
#define GETVER 0xA1         //Get firmware version
#define MCREAD 0xA2         //Memory Card Read (frame)
#define MCWRITE 0xA3        //Memory Card Write (frame)



void  HAL_UART_RxCpltCallback(UART_HandleTypeDef  *huart)//�����жϻص�����
{
	if(huart ==&huart1)//�ж��ж���Դ������1��USBת���ڣ�
    {
       switch(USART1_NewData){
       case GETID :MENU=1;break;
       case GETVER :MENU=2;break;
       case MCREAD :MENU=3;break;
       case MCWRITE :MENU=4;break;
       default:MENU=5;break;
       };
    }

}
