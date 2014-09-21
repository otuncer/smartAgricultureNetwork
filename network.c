#include "network.h"
#include "RFM22B.h"

int1 sendPacket(int8 *msg, int8 destAddr, int1 waitACK)
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

    //wait for ack if necessary
    if(!waitACK) {
        return 1;
    } else {
        RFM22Brxon();

        //wait until ACK for timer0 loop 
        timer = get_timer0() - 1;
        while(input(NIRQ) && timer!=get_timer0());

        if(!input(NIRQ)) { //valid packet received
            RFM22BclearFlags();
            return 1;
        } else {
            return 0;
        }
    }
    RFM22BtoReady();
}

void readPacket(int8 *msg, int8 *ack)
{
    int8 srcAddr;
    msg = RFM22Breadfifo();

    if(ack != NULL){
        srcAddr = RFM22BgetSourceAddr();
        sendPacket(ack, srcAddr, 0)
    }

    RFM22BclearFlags();
    RFM22BtoReady();
}