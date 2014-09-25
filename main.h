#include <18F2550.h>
#device adc=8

#FUSES NOWDT                     //WDT OFF
#FUSES HS                       //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD)
#FUSES NOPROTECT                //Code not protected from reading
#FUSES BROWNOUT                 //Reset when brownout detected
#FUSES BORV43                   //Brownout reset at 4.3V
#FUSES PUT                      //Power Up Timer
#FUSES NOCPD                    //No EE protection
#FUSES STVREN                   //Stack full/underflow will cause reset
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOWRT                    //Program memory not write protected
#FUSES NOWRTD                   //Data EEPROM not write protected
#FUSES NOIESO                   //Internal External Switch Over mode disabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES NOPBADEN                 //PORTB pins are configured as digital I/O on RESET
#FUSES NOWRTC                   //configuration not registers write protected
#FUSES NOWRTB                   //Boot block not write protected
#FUSES NOEBTR                   //Memory not protected from table reads
#FUSES NOEBTRB                  //Boot block not protected from table reads
#FUSES NOCPB                    //No Boot Block code protection
#FUSES NOMCLR                   //Master Clear pin disabled
#FUSES LPT1OSC                  //Timer1 configured for low-power operation
#FUSES NOXINST                  //Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES PLL1                     //No PLL PreScaler
#FUSES CPUDIV1                  //No System Clock Postscaler
#FUSES NOUSBDIV                 //USB clock source comes from primary oscillator
#FUSES NOVREGEN                 //USB voltage regulator disabled

#use delay(clock=20000000)
#use rs232(baud=9600,parity=E,xmit=PIN_C6,rcv=PIN_C7,bits=8,stop=1)

#use fast_io(a)
#use fast_io(b)
#use fast_io(c)
