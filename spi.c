#include "spi.h"

int8 _SPIreadByte()
{
   int8 n=8;
   int8 result=0;
   output_low(SCK);   
   while(n--)
   {
      output_high(SCK);
      result <<=1;
      result |= input(SDO);
      output_low(SCK);
   }
   return result;
}

void _SPIwriteByte(int8 data)
{
   int8 n=8;
   while(n--)
   {
      output_low(SCK); 
      if(data&0x80) 
        output_bit(SDI,1); 
      else 
        output_bit(SDI,0);
      output_high(SCK);     
      data = data << 1; 
   }
   output_low(SCK);
}

void SPIWrite(int8 address, int8 data)
{
   address|= 0x80;
   output_high(CSN);
   output_low(CSN);   
   _SPIwriteByte(address);
   _SPIwriteByte(data);   
   output_high(CSN);
}

int8 SPIRead(int8 address)
{
   int8 data;
   output_high(CSN);
   output_low(CSN);   
   _SPIwriteByte(address);
   data = _SPIreadByte();   
   output_high(CSN);
   return data;
}
