//network functions

#include "settings.h"

//sends given packet
//puts the module to ready mode at return
// @msg msg to send
// @destID destination module ID
// @ack pointer for ack
// @return false if not successful
void sendPacket(int8 *msg, int8 destAddr);
int1 sendPacket(int8 *msg, int8 destAddr, int8 *ack);

//reads packet to msg
//puts the module to ready mode at return
// @msg address to write packet
// @ack ACK message to send
//      if NULL, does not send ACK, sendPacket() should be used manually
void readPacket(int8 *msg, int8* ack);
