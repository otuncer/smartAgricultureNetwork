#include "RFM22B.h"
#include "spi.h"

void RFM22Bsendfifo(int8* data)
{
   int8 byteData;
   for(int i = 0; i < DATA_SIZE; i++){
      byteData = data[i];
      SPIWrite(RFM22B_FIFO, byteData);
   }
}

int8* RFM22Breadfifo()
{
   int8 *data = (int8*)malloc(DATA_SIZE * sizeof(int8));
   for(int i = 0; i < DATA_SIZE; i++){
      data[i] = SPIRead(RFM22B_FIFO); 
   }
   return data;
} 

void RFM22Btxon()
{
   //TX mode & ready mode on
   SPIWrite(RFM22B_FCTRL1, 0x09);
}

void RFM22Brxon()
{
   //RX mode & ready mode on
   SPIWrite(RFM22B_FCTRL1, 0x05);
}

void RFM22BtoReady()
{
   //only ready mode on
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

void RFM22BsetDest(int8 addr)
{
   SPIWrite(RFM22B_TXHEADER3, addr);
}

int8 RFM22BgetSourceAddr()
{
   return SPIRead(RFM22B_RCVHEADER2);
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

void RFM22Bsetup(int1 isMaster)
{
   delay_ms(20);           //wait for POR
   spiWrite(RFM22B_FCTRL1, 0x80);   //soft reset
   delay_ms(1);            //wait for soft reset

   SPIWrite(RFM22B_IFFILTERBW, 0xAE)
   SPIWrite(RFM22B_CLKRECOSRAT, 0x3C)
   SPIWrite(RFM22B_CLKRECOFF2, 0x02)
   SPIWrite(RFM22B_CLKRECOFF1, 0x22)
   SPIWrite(RFM22B_CLKRECOFF0, 0x22)
   SPIWrite(RFM22B_CLKRECTLG1, 0x04)
   SPIWrite(RFM22B_CLKRECTLG0, 0x46)
   SPIWrite(RFM22B_AFCLIMITER, 0x30)
   SPIWrite(0x0B, 0x15);   //GPIO0 set to RX state(output)
   SPIWrite(0x0C, 0x12);   //GPIO1 set to TX state(output)
   SPIWrite(0x2C, 0x28)
   SPIWrite(0x2D, 0x19)
   SPIWrite(0x2E, 0x29)
   SPIWrite(0x30, 0x8C)
   SPIWrite(0x35, 0x42)
   SPIWrite(0x38, 0x00)
   SPIWrite(0x39, 0x00)
   SPIWrite(0x3C, 0x00)
   SPIWrite(0x3D, 0x00)
   SPIWrite(0x41, 0x00)
   SPIWrite(0x42, 0x00)
   SPIWrite(0x43, 0xFF)
   SPIWrite(0x44, 0x00)
   SPIWrite(0x45, 0x00)
   SPIWrite(0x46, 0x00)
   SPIWrite(RFM22B_TXDATARATE1, 0x0C)
   SPIWrite(RFM22B_TXDATARATE0, 0xCD)
   SPIWrite(0x70, 0x0D)
   SPIWrite(RFM22B_MODCTRL2, 0x23)
   SPIWrite(RFM22B_FRQDEV, 0x50)
   SPIWrite(RFM22B_FRQBANDSLCT, 0x53)
   SPIWrite(RFM22B_NOMCARFREQ1, 0x00)
   SPIWrite(RFM22B_NOMCARFREQ0, 0x00)

   //modifiable:

   SPIWrite(RFM22B_TXPWR, 0x18) //power at LSB-3bits
   SPIWrite(RFM22B_TXPKTLEN, DATA_SIZE)
   SPIWrite(RFM22B_HEADERCTRL1, 0x08)
   SPIWrite(RFM22B_HEADERCTRL2, 0x28)
   SPIWrite(RFM22B_PREAMBLELEN, 0x1C)
   SPIWrite(RFM22B_SYNWRD3, 0x2D)
   SPIWrite(RFM22B_SYNWRD2, 0xD4)

   //use header-2 as own address
   SPIWrite(RFM22B_TXHEADER2, MODULE_ID);

   SPIWrite(RFM22B_CHKHEADER3, MODULE_ID)

   //only interrupts for packet sent & valid packet received
   SPIWrite(RFM22B_INTEN1, 0x06);
   SPIWrite(RFM22B_INTEN2, 0x00);

   RFM22BtoReady();
   RFM22BclearFlags();
}
