#include "network.h"
#include "RFM22B.h"


void sendPacket(int8 *msg, int8 destAddr)
{
   int8 timer;
   RFM22BclearFlags();
   RFM22Bsendfifo(msg);
   RFM22BsetDest(destAddr);
   RFM22Btxon();

   //wait until ACK for timer0 loop
   timer = get_timer0() - 1;
   while(input(NIRQ) && timer != get_timer0());
   RFM22BclearFlags();
   RFM22BtoReady();
}

int1 sendPacket(int8 *msg, int8 destAddr, int8* ack)
{
   int8 timer;
   sendPacket(msg, destAddr);
   
   //wait until ACK for timer0 loop 
   RFM22Brxon();  
   timer = get_timer0() - 1;
   while(input(NIRQ) && timer!=get_timer0());
   
   if(!input(NIRQ)) { //valid packet received
      readPacket(ack, NULL);
      RFM22BclearFlags();
      return 1;
   } else {
      return 0;
   }
   RFM22BtoReady();
}

void readPacket(int8 *msg, int8 *ack)
{
    int8 srcAddr;
    RFM22Breadfifo(msg);

    if(ack != NULL){
        srcAddr = RFM22BgetSourceAddr();
        sendPacket(ack, srcAddr);
    }

    RFM22BclearFlags();
    RFM22BtoReady();
}
