// RFM22B definitions

#include spi.h

#define RFM22B_INTSTAT1 	0x03
#define RFM22B_INTSTAT2 	0x04
#define RFM22B_INTEN1 		0x05
#define RFM22B_INTEN2 		0x06
#define RFM22B_FCTRL1 		0x07
#define RFM22B_FCTRL2 		0x08
#define RFM22B_GPIO0CONF 	0x0B
#define RFM22B_GPIO1CONF 	0x0C
#define RFM22B_IFFILTERBW	0x1C
#define RFM22B_CLKRECOSRAT	0x20
#define RFM22B_CLKRECOFF2	0x21
#define RFM22B_CLKRECOFF1	0x22
#define RFM22B_CLKRECOFF0	0x23
#define RFM22B_CLKRECTLG1	0x24
#define RFM22B_CLKRECTLG0	0x25
#define RFM22B_RSSI			0x26
#define RFM22B_AFCLIMITER	0x2A
#define RFM22B_HEADERCTRL1  0x32
#define RFM22B_HEADERCTRL2  0x33
#define RFM22B_PREAMBLELEN  0x34
#define RFM22B_SYNWRD3     	0x36
#define RFM22B_SYNWRD2     	0x37 
#define RFM22B_TXHEADER3   	0x3A
#define RFM22B_TXPKTLEN    	0x3E
#define RFM22B_CHKHEADER3  	0x3F
#define RFM22B_TXPWR 		0x6D
#define RFM22B_TXDATARATE1 	0x6E
#define RFM22B_TXDATARATE0 	0x6F
#define RFM22B_MODCTRL2    	0x71
#define RFM22B_FRQDEV	 	0x72
#define RFM22B_FREQHOPSTEP 	0x7A
#define RFM22B_FRQBANDSLCT 	0x75
#define RFM22B_NOMCARFREQ1 	0x76
#define RFM22B_NOMCARFREQ0 	0x77
#define RFM22B_FRQCHANNEL 	0x79
#define RFM22B_FIFO 		0x7F

void RFM22Bsendfifo(int8 data)
{
   SPIWrite(RFM22B_FIFO, data);
}

int8 RFM22Breadfifo()
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

void RFM22BgetRSSI()
{
   RSSI = SPIRead(RFM22B_RSSI);
}