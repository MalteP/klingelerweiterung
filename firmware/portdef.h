// #############################################################################
// #                 --- Klingelerweiterung Firmware ---                       #
// #############################################################################
// # portdef.h - Port definitions                                              #
// #############################################################################
// #              Version: 1.2 - Compiler: AVR-GCC 4.5.0 (Linux)               #
// # (c) '12-'13 by Malte Pöggel - www.MALTEPOEGGEL.de - malte@maltepoeggel.de #
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

#ifndef PORTDEF_H
 #define PORTDEF_H

 #define LED_PORT_DDR        DDRD
 #define LED_PORT_OUTPUT     PORTD
 #define LED_BIT             PD7
 
 #define TRIGGER_PORT_DDR    DDRD
 #define TRIGGER_PIN         PIND		
 #define TRIGGER_BIT         PD2
 
 #define SOUND_PORT_DDR      DDRD
 #define SOUND_PIN           PIND
 #define SOUND_BIT           PD1
 
 #define LEARN_PORT_DDR      DDRD
 #define LEARN_PIN           PIND
 #define LEARN_BIT           PD0

 #define TX_PORT_DDR         DDRB
 #define TX_PORT_OUTPUT      PORTB
 #define TX_BIT              PB0
 
#endif
