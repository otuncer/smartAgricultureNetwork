#include "RFM22B.h"
#include "spi.h"

void RFM22Bsendfifo(int8* data)
{
   SPIWrite(RFM22B_FIFO, *data);
}

int8 RFM22Breadfifo(int8* data)
{
   return SPIRead(RFM22B_FIFO); 
} 

/*
* TX mode on, ready mode on
*/
void RFM22Btxon()
{
   SPIWrite(RFM22B_FCTRL1, 0x09);
}

/*
* RX mode on, ready mode on
*/
void RFM22Brxon()
{
   SPIWrite(RFM22B_FCTRL1, 0x05);
}

/*
* only ready mode on
*/
void RFM22BtoIdle()
{
   SPIWrite(RFM22B_FCTRL1, 0x01);
}

void RFM22BclearFifos()
{
   SPIWrite(RFM22B_FCTRL2, 0x03); //clear RX/TX fifo disable autotx
   SPIWrite(RFM22B_FCTRL2, 0x00);
}

void RFM22BclearFlags()
{
   SPIRead(RFM22B_INTSTAT1);
   SPIRead(RFM22B_INTSTAT2);
}

int1 RFM22BisSyncWord()
{
   int8 reg;
   reg = SPIRead(RFM22B_INTSTAT2) & 0x80; //check interrupt 2-bit7
   if(reg==0)   
      return false;
   else
      return true;
}

int1 RFM22BisValidPacket()
{
   int8 reg;
   reg = SPIRead(RFM22B_INTSTAT1) & 0x02; //check interrupt 1-bit1
   if(reg==0)   
      return false;
   else
      return true;
}

void RFM22Bsetup(int1 isReceiver)
{

}
