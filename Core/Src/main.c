/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../icode/led/led.h"
#include "../../icode/key/key.h"
#include "../../icode/oled/oled.h"
#include "../../icode/card/card.h"
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//Device Firmware identifier
#define IDENTIFIER "MCDINO"  //MemCARDuino
#define VERSION 0x04         //Firmware version byte (Major.Minor)
//Responses
#define ERROR 0xE0         //Invalid command received (error)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart1;

uint8_t USART1_NewData;

uint8_t MENU;

int8_t Block;//记忆卡第几格

int8_t draw;//是否重绘标记

uint8_t slot[15];

uint8_t foundcard;//是否找到记忆卡标记

uint16_t temp;//记忆卡祯地址

uint8_t frame[200];//记忆卡祯缓存



/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	if(GPIO_Pin==ACK_Pin){
		LED_2_Contrary();
	};
}
void Draw(int8_t block){//绘屏抽出来单独一个函数，传递block第几块作为参数

if(foundcard==1&&draw==1){
	  OLED_DISPLAY_8x16_BUFFER(0,(uint8_t*)&"BLOCK           ");//想用类似"*-------------- "来表示
	  OLED_DISPLAY_8x16(0, 48,block/10+0x30);//可以看看日文字库的符号
	  OLED_DISPLAY_8x16(0, 56,block%10+0x30);
	  temp=0x0000+block;
	  ReadFrame(frame,temp);
	  //slot[block]=frame[0];
	  switch(frame[0]){//判断目录祯第一字节存档状态
			 case 0x51 :OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"In use 1st.     ");break;
			 case 0x52 :OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"In use middle.  ");break;
			 case 0x53 :OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"In use last.    ");break;
			 case 0xa0 :OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"Free.           ");break;
			 case 0xa1 :OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"Free.Deleted 1st");break;
			 case 0xa2 :OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"Free.Deleted 2nt");break;
			 case 0xa3 :OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"Free.Deleted 3rd");break;
			 default:OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"Error.          ");break;
			 };
	  if(frame[0]<0x54&&frame[0]>=0x51){//第一字节有效则显示0C-1E位置文件名
		  for(int i=0;i<16;i++){//只显示了一行16个，应采用00h为终止判定
			  if(frame[i+0x0c]<0x80&&frame[i+0x0c]>=0x20){//确保在ASCII可显示范围内
				 OLED_DISPLAY_8x16(4, i*8,(uint16_t)frame[i+0x0c]);
			  }

		  }
	  };
	  temp=block*64;//祯地址=BLOCK*64+FRAME,title frame 偏移为 0（1-15block中）
	  ReadFrame(frame,temp);
	  for(int i=0x04;i<0x24;i+=2){//按两行显示title，一行只能16个字（8*8）
			  OLED_DISPLAY_SJIS_8x8(6,(i-0x04)*4,(frame[i]<<8)|frame[i+1]);
	  };
	  for(int i=0x24;i<0x44;i+=2){
			  OLED_DISPLAY_SJIS_8x8(7,(i-0x24)*4,(frame[i]<<8)|frame[i+1]);
	  };
	}
	draw=0;//绘制完后，即关闭绘制标志
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	//uint8_t count;
	//uint8_t buff[2];
	//buff[0]=0;
	//buff[1]='\0';
	//count=0;
	//uint8_t version;
	//uint8_t err;
	uint8_t uartbuff[200],readbuff[128],writebuff[128];
	//uint8_t AddressMSB ,AddressLSB;
	//uint16_t Address;
	draw=1;//首次需要绘制，赋初值
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  LED_ALL(0x00);
  HAL_Delay(200);
  OLED0561_Init();

  //uint16_t jtext[] =  {0x834f,0x8389,0x8393,0x8366,0x8342,0x8341,0x89fc};
  //uint16_t jtext1[]={0x8bd9,0x8b7d,0x8e96,0x91d4,0x90e9,0x8cbe};

  OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"  PSX  MEMCARD  ");
  OLED_DISPLAY_8x16_BUFFER(4,(uint8_t*)&"Reading");
  HAL_Delay(1000);


  //OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"*-------------- ");
  //OLED_DISPLAY_8x16_BUFFER(4,(uint8_t*)&"SLPS-02124 JAPAN");

//  OLED_DISPLAY_SJIS_8x8(7,0,jtext[0]);
//  OLED_DISPLAY_SJIS_8x8(7,8,jtext[1]);
//  OLED_DISPLAY_SJIS_8x8(7,16,jtext[2]);
//  OLED_DISPLAY_SJIS_8x8(7,24,jtext[3]);
//  OLED_DISPLAY_SJIS_8x8(7,32,jtext[4]);
//  OLED_DISPLAY_SJIS_8x8(7,40,jtext[5]);

  int i=0;
  do{
	ReadFrame(frame,0x0000);
	OLED_DISPLAY_8x16(4,56+i*8,'.');
	HAL_Delay(500);
	i++;
  }while(!(frame[0]=='M'&&frame[1]=='C'&&frame[127]==0x0e)&&(i<9));
  //判定0000祯是否有效，多次未成功则跳出


  if(i<9){//8次以内，
	  OLED_DISPLAY_8x16_BUFFER(0,(uint8_t*)&"                ");
	  OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"  FOUND   CARD  ");
	  OLED_DISPLAY_8x16_BUFFER(4,(uint8_t*)&"                ");
	  foundcard=1;
	  }else{
	  OLED_DISPLAY_8x16_BUFFER(0,(uint8_t*)&"                ");

	  OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"    NO   CARD   ");//可以用日文
	  OLED_DISPLAY_8x16_BUFFER(4,(uint8_t*)&"                ");
	  foundcard=0;
	  }
  HAL_Delay(1000);



  MENU=0;//0:wait-cmd,1:GETID,2:GETVER,3:MCREAD,4:MCWRITE,5:ERROR
  Block=1;
  //version=VERSION;
  //err=ERROR;
  memset(uartbuff,0,200);
  memset(readbuff,0,128);
  memset(writebuff,0,128);






  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//	  if(MENU==1){//GETID
//		  HAL_UART_Transmit(&huart1, (uint8_t*)&IDENTIFIER, 6, 0x000f);
//		  OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"MCDINO");
//		  MENU=0;
//		  HAL_UART_Receive_IT(&huart1,&USART1_NewData,1);
//	  	  };
//	  if(MENU==2){//GETVER
//		  HAL_UART_Transmit(&huart1, &version, 1, 0x000f);
//		  OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"Ver.04");
//		  MENU=0;
//		  HAL_UART_Receive_IT(&huart1,&USART1_NewData,1);
//	  	  	  };
//	  if(MENU==3){//MCREAD
//		  HAL_UART_Receive(&huart1, readbuff, 2, 0x000f);
//		  AddressMSB=readbuff[0];
//		  AddressLSB=readbuff[1];
//		  Address=(AddressMSB<<8)|AddressLSB;
//		  OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"MCREAD");
//		  ReadFrame(frame,Address);
//		  HAL_UART_Transmit(&huart1, frame, 130, 0xffff);
//		  MENU=0;
//		  HAL_UART_Receive_IT(&huart1,&USART1_NewData,1);
//
//	  	  	  };
//	  if(MENU==4){//MCWRITE
////		  HAL_UART_Receive(&huart1, buff, 2, 0xffff);
////		  AddressMSB=buff[0];
////		  AddressLSB=buff[1];
////		  HAL_UART_Receive(&huart1, buff, 2, 0xffff);
////		  WriteFrame(&frame,AddressMSB,AddressLSB);
////		  HAL_UART_Transmit(&huart1, &writebuff, 128, 0xffff);
////		  MENU=0;
////		  HAL_UART_Receive_IT(&huart1,&USART1_NewData,1);
//
////		  HAL_UART_Receive(&huart1, buff, 2, 0xffff);
////		  AddressMSB=buff[0];
////		  AddressLSB=buff[1];
//		  OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"MCWRIT");
//		  		  //ReadFrame(frame,AddressMSB,AddressLSB);
//		  		  //HAL_UART_Transmit(&huart1, frame, 130, 0xffff);
//		  MENU=0;=y
//		  HAL_UART_Receive_IT(&huart1,&USART1_NewData,1);
//
//
//
//	  	  	  };		`
//	  if(MENU==5){//ERROR
//		  HAL_UART_Transmit(&huart1, &version, 1, 0x000f);
//		  OLED_DISPLAY_8x16_BUFFER(2,(uint8_t*)&"ERROR.");
//		  MENU=0;
//		  HAL_UART_Receive_IT(&huart1,&USART1_NewData,1);
//	  	  	  	  };
	  Draw(Block);


	  if(K2()){

				  Block++;
				  if(Block>15){Block=15;};
				  draw=1;

	  };
	  if(K3()){
		  		Block--;
		  		if(Block<1){Block=1;};
		  		draw=1;


	  };

	  HAL_Delay(50);


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_LSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED2_Pin|ATT_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : K2_Pin K3_Pin */
  GPIO_InitStruct.Pin = K2_Pin|K3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED1_Pin LED2_Pin */
  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : ACK_Pin */
  GPIO_InitStruct.Pin = ACK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ACK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ATT_Pin */
  GPIO_InitStruct.Pin = ATT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(ATT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
