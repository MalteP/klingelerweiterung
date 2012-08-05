Fusebits: 
avrdude -c usbasp -p m48 -U lfuse:w:0xe2:m -U hfuse:w:0xdc:m -U efuse:w:0x01:m


Flash: 
avrdude -c usbasp -p m48 -U flash:w:main.hex


EEProm:
 avrdude -c usbasp -p m48 -U eeprom:w:main.eep
