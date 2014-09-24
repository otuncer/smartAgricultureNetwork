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
   int8 counter;
   int1 ackReceived = 0;
   
   for(counter = 5; counter > 0 && ackReceived == 0; counter--) {
      sendPacket(msg, destAddr);
      
      //wait until ACK or timer0 loop 
      RFM22Brxon();
      timer = get_timer0() - 1;
      while(input(NIRQ) && timer!=get_timer0());
      
      ackReceived = !input(NIRQ);
      if(!ackReceived){
         delay_ms(rand()%50 * 100);
      }
   }   
   
   if(ackReceived) { //valid packet received
      readPacket(ack, NULL);
      RFM22BclearFlags();
   }
   RFM22BtoReady();
   return ackReceived;
}

int1 readPacket(int8 *msg, int8 *ack)
{
    int8 srcAddr;
    if(input(NIRQ)){
      return 0;
    } else {
       RFM22Breadfifo(msg);
   
       if(ack != NULL){
           srcAddr = RFM22BgetSourceAddr();
           sendPacket(ack, srcAddr);
       }
   
       RFM22BclearFlags();
       RFM22BtoReady();
       
       return 1;
    }
}

