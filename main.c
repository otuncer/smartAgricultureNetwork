#include "main.h"
#include "spi.c"
#include "RFM22B.c"
#include "settings.h"

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
