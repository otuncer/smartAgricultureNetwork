#include "main.h"
//#include "spi.c"
//#include "RFM22B.c"
//#include "settings.h"
#include "communication_r_v2.h"

#use fast_io(a)
#use fast_io(b)
#use fast_io(c)

void initRFM();

void main()
{
   int8 a;
   int8 i;
   
   setup_adc_ports(NO_ANALOGS|VSS_VDD);
   setup_adc(ADC_OFF);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   
   set_tris_a(0x00);
   set_tris_b(0b11110001);
   set_tris_c(0b10111111);
  
   output_high(pin_a0);
   delay_ms(1000);
   output_low(pin_a0);
   delay_ms(1000);
   
   //initRFM();
   //SPIWrite(0x25, 0xAA);
   a = SPIRead(0x76);
   //output_high(pin_a0);
   //delay_ms(1000);
   //output_low(pin_a0);
   //delay_ms(1000);
   for(i = 0; i < 8; i++){
      if((a & 0x80) == 0)
         output_low(pin_a0);
      else
         output_high(pin_a0);
      a <<= 1;
      delay_ms(1000);
   }
   while(1){/*
      output_high(SCK);
      output_high(SDI);
      output_high(CSN);
      output_bit(pin_a0, SDO );
      delay_ms(1000);
      output_low(SCK);
      output_low(SDI);
      output_low(CSN);
      output_bit(pin_a0, SDO );
      delay_ms(1000);*/
   
   
   
   
      output_high(pin_a0);
      delay_ms(100);
      output_low(pin_a0); 
      delay_ms(100);
   }
   
   //duz: 0001 1100
   
}
/*
void initRFM()
{
   delay_ms(20);           //wait for POR
   SPIWrite(0x07, 0x80);   //soft reset
   delay_ms(1);            //wait for soft reset
   
   //INTERRUPTS
   SPIWrite(0x05, 0x06); //enable packet sent&received interrupt
   SPIWrite(0x06, 0x80); //enable sync word received interrupt

   spiWrite(0x0B, 0x15);   //GPIO0 set to RX state(output)
   spiWrite(0x0C, 0x12);   //GPIO1 set to TX state(output)
   spiWrite(0x6D, 0x1A);   //set power last three bits +5dBm
   spiWrite(0x7A, 0x05);   //Freq hopping step size = 50kHz
   
   
   /// TABLE DATA ///
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

}*/
