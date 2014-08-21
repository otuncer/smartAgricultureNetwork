#include "C:\Users\Tolga\Dropbox\BLACKBODY CORP\teknik döküman\wireless module\code\PC side v1\main.h"
#include "C:\Users\Tolga\Dropbox\BLACKBODY CORP\teknik döküman\wireless module\code\communication_t v2.h"

#use fast_io(b)
#use fast_io(c)
#define LED pin_c2

#priority rda, timer1

int1 new=0;
int8 data=0;
int8 scanFreq=0;

void init(void)
{
   //init PIC
   setup_adc_ports(NO_ANALOGS|VSS_VDD);
   setup_adc(ADC_OFF);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_64|RTCC_8_bit);
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_2);
   setup_timer_2(T2_DISABLED,0,1);
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   //pins
   set_tris_b(0x11);
   set_tris_c(0x80);
   //init RFM
   initRFM();
   
   //init interrupts
   set_timer1(0);
   enable_interrupts(INT_RDA);
   enable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
}

void scanRSSI()
{
   int8 temp;
   set_freq(&scanFreq);
   spiWrite(0x06,0x60); //enable invalid/valid preamble interrupt
   while(get_timer1()<0xE800)
   {
      clearFlags();
      while(input(NIRQ) && get_timer1()<0xE800 ); //wait until NIRQ or timer
      if(!input(NIRQ))
      {
         getRSSI();
         temp = (0x80 | scanFreq);
         putc(temp); //kanal numarasi gonder
         temp=RSSI;
         temp>>=2;
         temp|=0xC0;
         putc(temp);  // o kanalin MS 6 bit RSSIini yolla
      }
      else
      {
         putc((0x80 | scanFreq)); //kanal numarasi gonder
         temp=00;       //bos kanal RSSI threshold
         temp>>=2;
         temp|=0xC0;
         putc(temp);  // bos kanal RSSI yolla 
      }
   }
   scanFreq++;
   if(scanFreq==16)
      scanFreq=0;
   spiWrite(0x06,0x00); //disable interrupt2
   set_freq(&freq_index);
}


/*
*transmits data to wireless module
*if ack is not 0xFF transmits ack to RS232
*clears data and returns 1 if transmission is successful
*/
int1 air2pc()
{
   int8 ack, data_last;
   data_last=data;
   ack = transmit(data,1);
   if(ack!=0xFF)
   {
      if(data_last==data)  //if command from PC is not changed, reset data
         data=0;
      putc(ack); //acki bilgisayara yolla
      return 1;
   }
   else
      return 0;
}

#int_TIMER1
void  TIMER1_isr(void) 
{   
   disable_interrupts(INT_TIMER1);
   enable_interrupts(GLOBAL);
   if(connected==1)
   {
      if(phase==0)
      {
         phase=1;
      }
      else//phase==1
      {
         connected=0;
         phase=0;
         air2pc();
      }
   }
   else//connected==0
   {
       if(phase==0)
       {
         phase=1;
         air2pc();
       }
       else//phase==1
       {
         phase=0;
         if(!air2pc())   //transmit and check ack
         {              //if not successful
            if(!inc_freq(0))  //if all fequencies are scanned
            {
               scan_all_freqs(&data);
            }
            putc(0x90 | freq_index); //bilgisayara su anki frekans kanal numarasýný yolla
         }
            //do nothing if transmission is successful
       }
   }
   scanRSSI();
   enable_interrupts(INT_TIMER1);
}


#int_RDA
void RDA_isr(void)
{
   data=getc();
   new=!new;
   if(new)
      data|=0x80;
   //putc(0xBE); //rs232 basarili ack BE - DISABLED
}


void main()
{
   //variables
   //initialization
   output_low(LED);
   delay_ms(50);
   output_high(LED);
   init();

   while(1);
}
