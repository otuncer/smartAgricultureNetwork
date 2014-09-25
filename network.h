//network functions

#include "settings.h"
 
//attempts sending given packet at most 5 times
//if an attempt is unsuccessful, waits for 100-5000ms (randomized)
//puts the module to ready mode at return
// @msg pointer to message to send
// @destID destination module ID
// @ack pointer for ack message return
// @return 0 if not successful
void sendPacket(int8 *msg, int8 destAddr); //unreliable
int1 sendPacket(int8 *msg, int8 destAddr, int8 *ack); //reliable

//reads packet to msg
//PIC needs to call RFM22Brxon() to be able to receive a message
//returns 0 if no message is received
//PIC can wait for NIRQ-low before calling this function,
//    which means a message is received
//puts the module to ready mode at return,
//    call RFM22Brxon() to receive new message
// @msg address to write packet
// @ack ACK message to send
//      if NULL, the function does not send ACK
int1 readPacket(int8 *msg, int8* ack);

