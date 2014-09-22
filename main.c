#include "main.h"
#include "spi.c"
#include "RFM22B.c"
#include "settings.h"
#include "network.c"
#include "stdlib.h"

void init();

void main()
{
   int8 a[4];
   int8 b[4];
   int1 mybool;

   init();
  
   output_high(pin_a0);
   delay_ms(1000);
   output_low(pin_a0);
   delay_ms(1000);
   output_high(pin_a0);
   delay_ms(1000);
   output_low(pin_a0);
   delay_ms(1000);
 
   /* //MODULE TESTER
   a = SPIRead(0x23);
   for(i = 0; i < 8; i++){
      if((a & 0x80) == 0)
         output_low(pin_a0);
      else
         output_high(pin_a0);
      a <<= 1;
      delay_ms(1000);
      
   }
   while (1){
      output_high(pin_a0);
      delay_ms(100);
      output_low(pin_a0);
      delay_ms(100);
   }*/
   /* RS232 TESTER
   while(1){
      putc(0xAB);
      delay_ms(1000);
      a=getc();
      for(i = 0; i < 8; i++){
         if((a & 0x80) == 0)
            output_low(pin_a0);
         else
            output_high(pin_a0);
         a <<= 1;
         delay_ms(1000);
      }
   }*/
  
#if MODULE_ID==0 //master node
   a[0]=0x01;
   a[1]=0x23;
   a[2]=0x45;
   a[3]=0x67;
   while(1) {
      mybool = sendPacket(b, 2, a);
      if(mybool){
         putc(a[0]);
         putc(a[1]);
         putc(a[2]);
         putc(a[3]);
      } else {
         putc(0x00);
      }
      delay_ms(500);
   }
#else
   while(1) {
      a[0]=0x89;
      a[1]=0xAB;
      a[2]=0xCD;
      a[3]=0xEF;
      RFM22Brxon();
      while(input(NIRQ));
      readPacket(b, a);
   }

#endif
   
}

void init()
{
   set_tris_a(0x00);
   set_tris_b(0b11110001);
   set_tris_c(0b10111111);
   
   setup_adc_ports(NO_ANALOGS|VSS_VDD);
   setup_adc(ADC_OFF);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_2);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   
   RFM22Bsetup();
}
