#include "network.h"
#include "RFM22B.h"

int1 sendPacket(int8 *msg, int16 destID, int1 waitACK)
{
	//TODO:
	//-send message, wait for ack if necessary
}

int1 readPacket(int8 *msg, int8* ack)
{
	//TODO:
	//-change fifo functions in RFM22B.c for dynamic packet size
	//-implement this function: read to msg, call sendPacket for ack if necessary
}