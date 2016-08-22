
// avrdude -p atmega8 -c usbasp -t
// dump eeprom 0 512
// http://www.protostack.com/blog/2011/01/reading-and-writing-atmega168-eeprom/
#include <avr/eeprom.h>
#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 8000000UL
#define EEPROM_ADDRESS 0x00

//Define functions
//======================
void ioinit(void);
void led_on(void);
void led_off(void);
//======================
 
uint16_t i=0, valueRead = 0;
int main (void)
{
  ioinit(); //Setup IO pins and defaults
  //write eeprom
  eeprom_write_word((uint16_t*)EEPROM_ADDRESS,5);
  //read eeprom
  valueRead = eeprom_read_word((uint16_t*)EEPROM_ADDRESS);
  while (1)
  {
    for(; i<valueRead; i++){
      _delay_ms(500);
      led_on();
      _delay_ms(500);
      led_off();
    }
  }
}
 
 
void ioinit (void)
{
  DDRC  = 0b11011111; //1 = output, 0 = input
  PORTC = 0b00100000; //Enable pin 5 internal pullup
}
 
void led_on(void)
{
   PORTC |= _BV(PC4);
}
 
void led_off(void)
{
   PORTC &= ~_BV(PC4);
}
