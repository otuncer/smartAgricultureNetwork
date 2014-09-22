//definitions for program setting

#ifndef _SETTINGS_H
   #define _SETTINGS_H
   
   #define MODULE_ID 2 //0 stands for master node
   
   #define NIRQ pin_b0  //interrupt pin of PIC
   #define SDI pin_b3   //SPI output of PIC
   #define SCK pin_b2   //SPI clock pin
   #define CSN pin_b1   //SPI communication CSN pin
   #define SDO pin_b4   //SPI input of PIC
   
   #define DATA_SIZE 4  //in bytes
   
#endif // _SETTINGS_H
