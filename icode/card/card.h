/*
 * card.h
 *
 *  Created on: May 19, 2022
 *      Author: Angel
 */

#ifndef CARD_CARD_H_
#define CARD_CARD_H_

#include "stm32f1xx_hal.h" //HAL库文件声明
#include "main.h" //IO定义与初始化函数在main.c文件中，必须引用


#define BLOCK_NUM 16
#define FRAMES_PER_BLOCK 64
#define FRAME_SIZE 128

#define MCID1 "MC"
#define MCID2 "SC"

#define JAPAN 'I'
#define EUROPE 'E'
#define AMERICA 'A'

#define ONEFRAME 0x11
#define TWOFRAME 0x12
#define THREEFRAME 0x13

#define MCACCESS 0x81
#define MCREAD 0x52
#define MCWRITE 0x57
#define MCGETID 0x53

extern SPI_HandleTypeDef hspi2;

typedef struct __packed{
	int8_t MemcardID1[2];
	uint8_t RESERVED0[125];
	uint8_t Checksum;
} Header;

typedef struct __packed{
	uint32_t State;
	uint32_t Filesize;
	uint16_t Next;
	int8_t Bchar;
	int8_t Region;
	int8_t Filename[30];
	uint8_t RESERVED0;
	uint8_t RESERVED1[95];
	uint8_t Checksum;
} Directory;

typedef struct __packed{
	uint32_t SectorNO;
	uint8_t RESERVED0[123];
	uint8_t Checksum;
} BrokenSectorList;

typedef struct __packed{
	int8_t MemcardID2[2];
	uint8_t IconFlag;
	int8_t BlockNO;
	uint16_t TText[32];
	uint8_t RESERVED0[12];
	uint8_t RESERVED1[16];
	uint16_t Palette[16];
} Title;

typedef struct __packed{
	uint8_t Bitmap[128];
} BitmapFrame;

void ReadFrame(uint8_t *frame,uint16_t Address);
uint8_t SPI2_ReadWriteByte(uint8_t TxData);
void MEMCARD_CS(uint8_t a);

#endif /* CARD_CARD_H_ */
