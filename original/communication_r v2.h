/*
* data bytes reserved for communication protocol: 0xFF, 0x00
*/

#define NIRQ pin_b0          // input of the PIC
#define SDI pin_b6          // output of the PIC
#define SCK pin_b5
#define NSEL pin_b4
#define SDO pin_b7
#define NOCONTH 600

int8 RSSI=0;                //RSSI value
int8 freq_index=0;
int8 freq_counter=4;
int1 connected=0;
int1 phase=1;
int16 noConnection=NOCONTH;   //counter decreases when there is no connection

//declarations
void reset_fh(void);
void  change_pwm(void);

   ///***SPI***///


int8 read8bit()
{
   int8 n=8;
   int8 result=0;
   output_low(SCK);   
   while(n--)
   {
      output_high(SCK);
      result <<=1;
      result |= input(SDO);
      output_low(SCK);
   }
   return result;
}

void write8bit(int8 data)
{
   int8 n=8;
   while(n--)
   {
      output_low(SCK); 
      if(data&0x80) 
        output_bit(SDI,1); 
      else 
        output_bit(SDI,0);
      output_high(SCK);     
       data = data << 1; 
   }
   output_low(SCK);
}

//copy function for spiWrite_main
void write8bit_main(int8 data)
{
   int8 n=8;
   while(n--)
   {
      output_low(SCK); 
      if(data&0x80) 
        output_bit(SDI,1); 
      else 
        output_bit(SDI,0);
      output_high(SCK);     
       data = data << 1; 
   }
   output_low(SCK);
}

void spiWrite(int8 address, int8 data)
{
   address|=0x80;
   output_high(NSEL);
   output_low(NSEL);   
   write8bit(address);
   write8bit(data);   
   output_high(NSEL);
}

//copy function for set_freq_main
void spiWrite_main(int8 address, int8 data)
{
   address|=0x80;
   output_high(NSEL);
   output_low(NSEL);   
   write8bit_main(address);
   write8bit_main(data);   
   output_high(NSEL);
}

int8 spiRead(int8 address)
{
   int8 data;
   output_high(NSEL);
   output_low(NSEL);   
   write8bit(address);
   data = read8bit();   
   output_high(NSEL);
   return data;
}


      ///***COMMON COMMANDS***///


void sendfifo(int8 data)
{
   spiWrite(0x7F, data);
}

int8 readfifo()
{
   return spiRead(0x7F); 
} 

/*
* TX mode on, ready mode on
*/
void txon()
{
   spiWrite(0x07, 0x09);
}

/*
* RX mode on, ready mode on
*/
void rxon()
{
   spiWrite(0x07, 0x05);    
}

/*
* only ready mode on
*/
void toIdle()
{
   spiWrite(0x07, 0x01);
}

void clearFifos()
{
   spiWrite(0x08,0x03); //clear RX/TX fifo disable autotx
   spiWrite(0x08,0x00);
}

void clearFlags()
{
   spiRead(0x03);
   spiRead(0x04);     
}

//returns if valid sync word interrupt is detected
int1 isSyncWord()
{
   int8 reg;
   reg = spiRead(0x04) & 0x80; //check interrupt 2-bit7
   if(reg==0)   
      return false;
   else
      return true;
}

//returns if valid packet interrupt is detected
int1 isValidPacket()
{
   int8 reg;
   reg = spiRead(0x03) & 0x02; //check interrupt 1-bit1
   if(reg==0)   
      return false;
   else
      return true;
}


      ///***TRASMIT/RECEIVE***///


/*
* Reads RSSI and saves it
*/
void getRSSI()
{
   RSSI=spiRead(0x26);
}

/*
* waits for a timer0 loop for ACK
* if transmitted, returns ACK value
*   else, returns 0xFF
* returns in idle mode
*/
int8 reliableTransmit()
{
   int8 timer;
   rxon();             //wait for ACK
   timer=get_timer0()-1;        //////////OPTIMIZE ET, KISALT!!!/////////////////////////////////////////////////////////
   while(input(NIRQ) && timer!=get_timer0());  //wait until ACK for timer0 loop 
   rxon();
   if(!input(NIRQ))    //if valid packet received
   {
      reset_fh();
      return readfifo();
   }
   return 0xFF;
}

/*
* returns 0xFF
* returns in rx mode
*/
int8 unreliableTransmit()
{
   rxon();
   return 0xFF;
}

/*
* activates transmit mode 
* returns in idle mode
*/
//packet content:5B preamble,2B syncword,1B header,1B data,2B CRC: 88bits total
int8 transmit(int8 data, int1 reliable)
{
   int8 timer;
   clearFlags();       //clear flags
   sendfifo(data);     //write data to FIFO
   txon();             //send data
   timer=get_timer0()-1;
   while(input(NIRQ) && timer!=get_timer0());  //wait until ACK for timer0 loop
   clearFlags();       //clear flags
   if(reliable)
      return reliableTransmit();
   else
      return unreliableTransmit();
}


/*
* receive function for receiver
* Reads rx fifo
* sends ACK with given value
*/
int8 receive(int8 ack)
{
   reset_fh();       //reset freq hopping
   transmit(ack,0);
   return readfifo();
}


          ///***FREQ HOPPING***///
      
/*
* sets the frequency to the given value
* prevents overflow
*/
void set_freq(int8 *freq)
{
   if(*freq>15)  //prevent overflow
      *freq-=16;     
   spiWrite(0x79, *freq); //adjust frequency 
   delay_us(200);
}

/*
* copy function for inc_freq_int
*/
void set_freq_main(int8 *freq)
{
   int8 dly_cntr;
   if(*freq>15)  //prevent overflow
      *freq-=16;     
   spiWrite_main(0x79, *freq); //adjust frequency 
   for(dly_cntr=0;dly_cntr<0xAA;dly_cntr++);    //delay 200us
}


/*
* increments freq index
* if ifScan parameter is false, increments 2 times
* returns 0 if all frequencies are scanned
*/
int1 inc_freq(int1 ifScan)
{
   if(!ifScan)
   {
      freq_counter++;
      if(freq_counter>3)
         return 0;
      freq_index+=2;
   }
   freq_index+=2;
   set_freq(&freq_index);
   return 1;
}

//copy for timer interrupt of reciever
int1 inc_freq_int(int1 ifScan)
{
   if(!ifScan)
   {
      freq_counter++;
      if(freq_counter>3)
         return 0;
      freq_index+=2;
   }
   freq_index+=2;
   set_freq_main(&freq_index); //brain damage
   return 1;
}

/*
*resets frequency hopping variables
*/
void reset_fh()
{
   connected=1;
   phase=0;
   freq_counter=0;
   set_timer1(0);
   noConnection=NOCONTH;
}

/*
* scans all freqs, 1 message per freq with 100% duty cycle
* returns when transmission is successful
*/
void scan_all_freqs(int8 *data)
{
   int8 ack,data_last;
   while(1)
   {
      data_last=*data;
      ack = transmit(*data,1); //reliable transmit data
      if(ack!=0xFF)  //if transmission successful, return
      {
         if(data_last==*data)  //if command from PC is not changed, reset data
            *data=0;
         return;
      }
      inc_freq(1);
   }
}


      ///***CONFIG***///


/*
* Configures RFM22B with common features for transmitter and receiver
* configuration changes according to isReceiver bit
*/
void initRFM()
{
   delay_ms(20);           //wait for POR
   spiWrite(0x07, 0x80);   //soft reset
   delay_ms(1);            //wait for soft reset
   
   //INTERRUPTS
   spiWrite(0x05, 0x06); //enable packet sent&received interrupt
   spiWrite(0x06, 0x80); //enable sync word received interrupt

   spiWrite(0x0B, 0x15);   //GPIO0 set to RX state(output)
   spiWrite(0x0C, 0x12);   //GPIO1 set to TX state(output)
   spiWrite(0x6D, 0x1A);   //set power last three bits +5dBm
   spiWrite(0x7A, 0x05);   //Freq hopping step size = 50kHz
   
   
   ///***TABLE DATA***///
   spiWrite(0x1C, 0x05); //
   spiWrite(0x20, 0x50); //
   spiWrite(0x21, 0x01); //
   spiWrite(0x22, 0x99); //
   spiWrite(0x23, 0x9A); //
   spiWrite(0x24, 0x06); //
   spiWrite(0x25, 0x68); //
   spiWrite(0x2A, 0x28); //
   spiWrite(0x32, 0x08);
   spiWrite(0x33, 0x1A);
   spiWrite(0x34, 0x0A);
   spiWrite(0x36, 0xD2);
   spiWrite(0x37, 0x4D);
   spiWrite(0x3A, 0xA1);
   spiWrite(0x3E, 0x01);   //PACKET LENGTH set to 1
   spiWrite(0x3F, 0xA1);
   spiWrite(0x6E, 0x0C); //hiz = 50k
   spiWrite(0x6F, 0xCD); //hiz
   spiWrite(0x71, 0x23);   //MODULATION MODE CONTROL 2: set to GFSK
   spiWrite(0x72, 0x28); ////deltaf
   spiWrite(0x75, 0x53);   //FREQ BAND SELECT: 430MHz
   spiWrite(0x76, 0x65);   //NOMINAL CARRIER FREQ: +4MHz
   spiWrite(0x77, 0xC0);   //... 

   clearFlags();
}
