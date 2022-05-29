# memcard32

## Description
### If you got tons of psx memcards, there would be problems to find out one specific.
### Now i make a memcard reader, with which you can view the content inside without even turn on your ps1/2 console.

## Feature
+ Read title and name of the saves.
+ Circulate the blocks inside a memcard.
+ Japanes kanji(shift-jis) 8x8 font implemented, based on [Misaki]
(https://littlelimit.net/misaki.htm) font.
![font-view](https://littlelimit.net/img/font/misaki_mincho.png)
![pc-e500](https://littlelimit.net/img/font/misaki_u6000.jpg)
+ sh1106 12864 oled display

## Issue
### Card Ack/int not implemented, use HAL_Delay(), still got some timing issue.

## Todo
+ TF card support.
+ Memcarduino compatible.
+ Misterfpga 8x8 asiic font support.
+ Psx pad support.
+ Psx pad emulate to switch page on 3rd party multi-memcard.

## Hardware
+ stm32f103c8t6
+ PA0 pullup button
+ PA1 pullup button
+ PB0 pullpush LED
+ PB1 pullpush LED
+ PB7 SDA
+ PB6 SCL i2c sh1106 12864 oled
+ PB11 memcard ack
+ PB12 memcard DAT
+ PB13 memcard CMD
+ PB14 memcard CLK
+ PB15 memcard ATT [memcard pinout](https://psx-spx.consoledev.net/pinouts/#pinouts-controller-ports-and-memory-card-ports)






