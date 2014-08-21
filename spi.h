// SPI functions

//check definitions
#ifndef SCK
#error SCK required for spi.h
#endif
#ifndef SDO
#error SDO required for spi.h
#endif
#ifndef SDI
#error SDI required for spi.h
#endif
#ifndef NSEL
#error NSEL required for spi.h
#endif

int8 _SPIread8bit()
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

void _SPIwrite8bit(int8 data)
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
   output_high(NSEL);
   output_low(NSEL);   
   _SPIwrite8bit(address);
   _SPIwrite8bit(data);   
   output_high(NSEL);
}

int8 SPIRead(int8 address)
{
   int8 data;
   output_high(NSEL);
   output_low(NSEL);   
   _SPIwrite8bit(address);
   data = _SPIread8bit();   
   output_high(NSEL);
   return data;
}