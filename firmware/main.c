// #############################################################################
// #                 --- Klingelerweiterung Firmware ---                       #
// #############################################################################
// # main.c - Main                                                             #
// #############################################################################
// #              Version: 1.1 - Compiler: AVR-GCC 4.5.0 (Linux)               #
// #  (c) 2012 by Malte Pöggel - www.MALTEPOEGGEL.de - malte@maltepoeggel.de   #
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
 
 // Use HX or Intertechno code?
 volatile uint8_t global_mode;
 uint8_t eeGlobal_Mode EEMEM = BELL_HX;

 // HX
 volatile uint8_t hx_code;
 volatile uint8_t hx_sound;
 uint8_t eeHX_Code EEMEM = 0x42;
 uint8_t eeHX_Sound EEMEM = 0x00;

 // Duewi / Intertechno
 volatile uint8_t it_code;
 uint8_t eeIT_Code EEMEM = 0x00;


 // ###### main() ######   
 int main(void)
  {
   uint8_t sound_key_status = 0;
   uint8_t learn_key_status = 0;
   uint8_t random_code = 0;
   uint8_t key_hold_ctr = 0;
  
   // Initialize the wireless module
   wireless_init();  
  
   // Configure ports      
   LED_PORT_DDR |= (1<<LED_BIT);

   // Load EEP values
   global_mode = eeprom_read_byte(&eeGlobal_Mode);
   hx_code = eeprom_read_byte(&eeHX_Code);
   hx_sound = eeprom_read_byte(&eeHX_Sound);
   it_code = eeprom_read_byte(&eeIT_Code);

   // Global mode out of range? 
   if(global_mode!=BELL_HX&&global_mode!=BELL_IT)
    {
     global_mode = BELL_HX;
     eeprom_write_byte(&eeGlobal_Mode, global_mode);
    }

   // HX Sound out of range?
   if(hx_sound>7)
    {
     hx_sound = 0;
     eeprom_write_byte(&eeHX_Sound, hx_sound);
    }

   // IT code out of range?
   if(it_code>15)
    {
     it_code = 0;
     eeprom_write_byte(&eeIT_Code, it_code);
    }

   // Show mode
   led_mode_out();

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
         if(global_mode==BELL_HX) // Change sound only in HX mode; IT does not transmit sound, so we can't change it!
          {
           hx_sound++;
           if(hx_sound>7) hx_sound = 0;
           eeprom_write_byte(&eeHX_Sound, hx_sound);
           if(!wireless_status())
            {
             LED_PORT_OUTPUT &= ~(1<<LED_BIT);
             wireless_bell(hx_code, int_to_hx_soundcode(hx_sound));
            }
          } else {
           LED_PORT_OUTPUT &= ~(1<<LED_BIT);
           wireless_switch_4(int_to_pt_housecode(it_code), int_to_pt_code(it_code), PT_SWITCH_ON);
          }
         _delay_ms(250);
        }
      }
     
     // Check learn key
     if(!(LEARN_PIN & (1<<LEARN_BIT)))
      {
       learn_key_status = 1;
       if(key_hold_ctr<=32) 
        { 
         key_hold_ctr++;
        }
      } else {
       if(learn_key_status==1)
        {
         learn_key_status = 0;
         if(key_hold_ctr<=32) 
          {
           // Short pressed
           if(global_mode==BELL_HX)
            {
             hx_code = random_code;
             eeprom_write_byte(&eeHX_Code, hx_code);         
             if(!wireless_status())
              {
               LED_PORT_OUTPUT &= ~(1<<LED_BIT);
               wireless_bell(hx_code, int_to_hx_soundcode(hx_sound));
              }
            } else {
             it_code++;
             if(it_code>15) it_code=0;
             eeprom_write_byte(&eeIT_Code, it_code);
             if(!wireless_status())
              {
               LED_PORT_OUTPUT &= ~(1<<LED_BIT);
               wireless_switch_4(int_to_pt_housecode(it_code), int_to_pt_code(it_code), PT_SWITCH_ON);
              }
            }
           _delay_ms(250);
          } else {
           // Key hold, change mode!
           if(global_mode==BELL_HX) global_mode=BELL_IT; else global_mode=BELL_HX;
           eeprom_write_byte(&eeGlobal_Mode, global_mode);
           led_mode_out();
          }
        }
       key_hold_ctr=0;
      }     
     
     // Status LED
     if(wireless_status()) LED_PORT_OUTPUT &= ~(1<<LED_BIT);
      else LED_PORT_OUTPUT |= (1<<LED_BIT);
      
      _delay_ms(25);
    }  
  }

 
 // ###### Flash LED for mode: HX = 1x, IT = 2x ######
 void led_mode_out( void )
  {
   LED_PORT_OUTPUT |= (1<<LED_BIT);
   _delay_ms(50);
   LED_PORT_OUTPUT &= ~(1<<LED_BIT);
   _delay_ms(250);
   LED_PORT_OUTPUT |= (1<<LED_BIT);
   _delay_ms(250);
   if(global_mode==BELL_HX) return;
   LED_PORT_OUTPUT &= ~(1<<LED_BIT);
   _delay_ms(250);
   LED_PORT_OUTPUT |= (1<<LED_BIT);
   _delay_ms(250);
  }


 // ###### Translate HX soundcode ###### 
 uint8_t int_to_hx_soundcode( uint8_t x )
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


 // ###### Translate PT housecode ###### 
 uint8_t int_to_pt_housecode( uint8_t x )
  {
   return 15-(x/4); // A, B, C, D
  }


 // ###### Translate PT code ######
 uint8_t int_to_pt_code( uint8_t x )
  {
   return 15-(x%4); // 1, 2, 3, 4
  }


 // ###### INT0: trigger ######  
 ISR( INT0_vect )
  {
   if(!wireless_status()) 
    {
     LED_PORT_OUTPUT &= ~(1<<LED_BIT);
     if(global_mode==BELL_HX)
      {
       wireless_bell(hx_code, int_to_hx_soundcode(hx_sound));
      } else {
       wireless_switch_4(int_to_pt_housecode(it_code), int_to_pt_code(it_code), PT_SWITCH_ON);
      }
    }
  }
