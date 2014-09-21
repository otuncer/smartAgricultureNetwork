// definitions and basic functions for RFM22B

#include "settings.h"

void RFM22Bsendfifo(int8* data);
int8* RFM22Breadfifo();

void RFM22Btxon();
void RFM22Brxon();
void RFM22BtoReady();

void RFM22BclearFifos();
void RFM22BclearFlags();

void RFM22BsetDest(int8 addr);
int8 RFM22BgetSourceAddr();

int1 RFM22BisSyncWord();
int1 RFM22BisValidPacket();

void RFM22BgetRSSI();

void RFM22Bsetup(int1 isMaster);

#define RFM22B_INTSTAT1     0x03
#define RFM22B_INTSTAT2     0x04
#define RFM22B_INTEN1       0x05
#define RFM22B_INTEN2       0x06
#define RFM22B_FCTRL1       0x07
#define RFM22B_FCTRL2       0x08
#define RFM22B_GPIO0CONF    0x0B
#define RFM22B_GPIO1CONF    0x0C
#define RFM22B_IFFILTERBW   0x1C
#define RFM22B_CLKRECOSRAT  0x20
#define RFM22B_CLKRECOFF2   0x21
#define RFM22B_CLKRECOFF1   0x22
#define RFM22B_CLKRECOFF0   0x23
#define RFM22B_CLKRECTLG1   0x24
#define RFM22B_CLKRECTLG0   0x25
#define RFM22B_RSSI         0x26
#define RFM22B_AFCLIMITER   0x2A
#define RFM22B_HEADERCTRL1  0x32
#define RFM22B_HEADERCTRL2  0x33
#define RFM22B_PREAMBLELEN  0x34
#define RFM22B_SYNWRD3      0x36
#define RFM22B_SYNWRD2      0x37
#define RFM22B_TXHEADER3    0x3A
#define RFM22B_TXHEADER2    0x3B
#define RFM22B_TXPKTLEN     0x3E
#define RFM22B_CHKHEADER3   0x3F
#define RFM22B_CHKHEADER2   0x40
#define RFM22B_RCVHEADER2   0x48
#define RFM22B_TXPWR        0x6D
#define RFM22B_TXDATARATE1  0x6E
#define RFM22B_TXDATARATE0  0x6F
#define RFM22B_MODCTRL2     0x71
#define RFM22B_FRQDEV       0x72
#define RFM22B_FREQHOPSTEP  0x7A
#define RFM22B_FRQBANDSLCT  0x75
#define RFM22B_NOMCARFREQ1  0x76
#define RFM22B_NOMCARFREQ0  0x77
#define RFM22B_FRQCHANNEL   0x79
#define RFM22B_FIFO         0x7F