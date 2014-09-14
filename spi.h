// SPI functions

#include "settings.h"

int8 _SPIreadByte();

void _SPIwriteByte(int8 data);

void SPIWrite(int8 address, int8 data);

int8 SPIRead(int8 address);

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
