// #############################################################################
// #                 --- Klingelerweiterung Firmware ---                       #
// #############################################################################
// # main.h - Main (Header)                                                    #
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

#ifndef MAIN_H
 #define MAIN_H
 
 #include <avr/io.h>

 #define BELL_HX 0
 #define BELL_IT 1
 
 #define PT_SWITCH_ON 0x01
 #define PT_SWITCH_OFF 0x02

 void led_mode_out( void );
 uint8_t int_to_hx_soundcode( uint8_t x );
 uint8_t int_to_pt_housecode( uint8_t x );
 uint8_t int_to_pt_code( uint8_t x );

#endif
