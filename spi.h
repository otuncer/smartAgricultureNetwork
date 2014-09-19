// SPI functions

#include "settings.h"

int8 _SPIreadByte();

void _SPIwriteByte(int8 data);

void SPIWrite(int8 address, int8 data);

int8 SPIRead(int8 address);