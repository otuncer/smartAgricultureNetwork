//network functions

#include "settings.h"

//sends given packet
// @msg msg to send
// @destID destination module ID
// @waitACK waits for acknowledgement
// @return false if not successful
int1 sendPacket(int8 *msg, int16 destID, int1 waitACK);

//reads packet to msg
// @msg address to write packet
// @ack ACK message to send
//		if NULL, does not send ACK, sendPacket() should be used manually
// @return false if not successful
int1 readPacket(int8 *msg, int8* ack);

#ifndef NIRQ
	#error NIRQ not defined
#endif
#ifndef PACKET_SIZE
	#error PACKET_SIZE not defined
#endif