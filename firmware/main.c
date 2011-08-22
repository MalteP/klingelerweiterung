// #############################################################################
// #                 --- Klingelerweiterung Firmware ---                       #
// #############################################################################
// # main.c - Main                                                             #
// #############################################################################
// #              Version: 1.0 - Compiler: AVR-GCC 4.5.0 (Linux)               #
// #  (c) 2011 by Malte Pöggel - www.MALTEPOEGGEL.de - malte@maltepoeggel.de   #
// #############################################################################
// #  This program is free software; you can redistribute it and/or modify it  #
// #   under the terms of the GNU General Public License as published by the   #
// #        Free Software Foundation; either version 3 of the License,         #
// #                  or (at your option) any later version.                   #
// #                                                                           #
// #      This program is distributed in the hope that it will be useful,      #
// #      but WITHOUT ANY WARRANTY; without even the implied warranty of       #
// #           MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.            #
// #           See the GNU General Public License for more details.            #
// #                                                                           #
// #  You should have received a copy of the GNU General Public License along  #
// #      with this program; if not, see <http://www.gnu.org/licenses/>.       #
// #############################################################################

 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
 #include <avr/eeprom.h>
 #include "wireless.h"
 #include "portdef.h"
 #include "main.h"
 
 uint8_t eeCode EEMEM = 0x42;
 uint8_t eeSound EEMEM = 0x00;
 
 volatile uint8_t code;
 volatile uint8_t sound;


 // ###### main() ######   
 int main(void)
  {
   uint8_t sound_key_status = 0;
   uint8_t learn_key_status = 0;
   uint8_t random_code = 0;
  
   // Initialize the wireless module
   wireless_init();  
  
   // Configure ports      
   LED_PORT_DDR |= (1<<LED_BIT);

   // Load EEP values
   code = eeprom_read_byte(&eeCode);
   sound = eeprom_read_byte(&eeSound);
   if(sound>7)
    {
     sound = 0;
     eeprom_write_byte(&eeSound, sound);
    }
   
   // Configure INT0
   TRIGGER_PORT_DDR &= ~(1<<TRIGGER_BIT);
   EICRA            |=  (1<<ISC00);
   EIMSK            |=  (1<<INT0);
  
   // Enable interrupts
   sei();
 
   // Main loop
   while(1)
    {
     // For random code generation
     random_code++;

     // Check sound key
     if(!(SOUND_PIN & (1<<SOUND_BIT)))
      {
       sound_key_status = 1;
      } else {
       if(sound_key_status==1)
        {
         sound_key_status = 0;
         sound++;
         if(sound>7) sound = 0;
         eeprom_write_byte(&eeSound, sound);
         if(!wireless_status())
          {
           wireless_bell(code, int_to_soundcode(sound));
           LED_PORT_OUTPUT |= (1<<LED_BIT);
          }
         _delay_ms(250);
        }
      }
     
     // Check learn key
     if(!(LEARN_PIN & (1<<LEARN_BIT)))
      {
       learn_key_status = 1;
      } else {
       if(learn_key_status==1)
        {
         learn_key_status = 0;
         code = random_code;
         eeprom_write_byte(&eeCode, code);         
         if(!wireless_status())
          {
           wireless_bell(code, int_to_soundcode(sound));
           LED_PORT_OUTPUT |= (1<<LED_BIT);
          }
         _delay_ms(250);
        }
      }     
     
     // Status LED
     if(wireless_status()) LED_PORT_OUTPUT &= ~(1<<LED_BIT);
      else LED_PORT_OUTPUT |= (1<<LED_BIT);
      
      _delay_ms(25);
    }  
  }


 // ###### Translate soundcode ###### 
 uint8_t int_to_soundcode( uint8_t x )
  {
   uint8_t sound = 0x0E;
   switch(x)
    {
     case 0:
      sound = 0x0E;
      break;
     case 1:
      sound = 0x0C;
      break;                  
     case 2:
      sound = 0x0A;
      break;      
     case 3:
      sound = 0x06;
      break;
     case 4:
      sound = 0x02;
      break;                  
     case 5:
      sound = 0x01;
      break;                  
     case 6:
      sound = 0x09;
      break;                  
     case 7:
      sound = 0x0D;
      break;
    }     
   return sound;
  }

  
 // ###### INT0: trigger ######  
 ISR( INT0_vect )
  {
    if(!wireless_status()) wireless_bell(code, int_to_soundcode(sound));
  }
