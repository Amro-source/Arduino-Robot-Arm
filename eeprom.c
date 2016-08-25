
// avrdude -p atmega8 -c usbasp -t -B 3
// dump eeprom 0 512
// http://www.protostack.com/blog/2011/01/reading-and-writing-atmega168-eeprom/
#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define F_CPU 1000000UL
#define EEPROM_ADDRESS 0x05

//Define functions
//======================
void ioinit(void);
void led_on(void);
void led_off(void);
void timer0_init(void);
//======================
 
volatile uint16_t timeRemaining = 10;
uint16_t i=0, valueRead = 10;
int main (void)
{
  ioinit(); //Setup IO pins and defaults
  // eeprom_write_word((uint16_t*)EEPROM_ADDRESS,10);//write eeprom
  timeRemaining = eeprom_read_word((uint16_t*)EEPROM_ADDRESS);//read eeprom
  timer0_init();//init the timer and interrupt
  sei();//Enable Global Interrupts
  // timeRemaining = 10;
  // PORTB |= (1 << PB2); 
  while (1)
  {
    // timeRemaining--;
    // if (timeRemaining <1){
    //   // if (PIND & (1 << PD0)){
    //     timeRemaining = 10;
    //     // PORTB |= (1 << PB0); 
    //     PORTB ^=  (1 << PB2); 
    //   // }
    // }else{
    //   // PORTB &= ~(1 << PB2);
    // }
    if (timeRemaining < 1){
      PORTB &= ~(1 << PB2);
    }else{
      PORTB |= (1 << PB2);
    }

    if ((PIND & (1 << PD0))){//if switch pressed
      timeRemaining = 10;
      cli();
      eeprom_write_word((uint16_t*)EEPROM_ADDRESS,10);//write eeprom
      sei();
    }

    // PORTB ^= (1 << PB1);    // toggles the led
    // _delay_ms(500);
  //   }else{
  //     PORTB ^= (1 << PB1);    // toggles the led
  //   }
  //   // for(; i<valueRead; i++){
  //     _delay_ms(500);
  //     led_on();
  //     _delay_ms(500);
  //     led_off();
  //   // }
  }
}
 
void timer0_init(void)
{
  TCCR0 |= (1 << CS02)|(1 << CS00);//Prescaler = FCPU/1024 = 976 hz
  TIMSK |= (1<<TOIE0);//timer overflow interrupt enable
  TCNT0 = 0; //initialize counter
}
 
void ioinit (void)
{
  DDRC  = 0b11011111; //1 = output, 0 = input
  PORTC = 0b00100000; //Enable pin 5 internal pullup
  DDRB |= (1 << PB0)|(1 << PB1)|(1 << PB2); //pb1 is output
  // DDRD  = 0b00000111;
  // PORTD = 0b11111000; //enable internal pullup on these pins
  DDRD &= ~(1 << PD0);    // switch on pin (PD0)
  PORTD |= (1 << PD0);    // enable pull-up resistor
}
 


ISR(TIMER0_OVF_vect)
{
  // cli();//Enable Global Interrupts
  if(timeRemaining > 0){
    timeRemaining--;
  }
  PORTB ^= (1 << PB1);    // toggles the led
  // sei();//Enable Global Interrupts
}
  //This is the interrupt service routine for TIMER0 OVERFLOW Interrupt.
  //CPU automatically call this when TIMER0 overflows.
  // PORTB ^= (1 << PB1);    // toggles the led
  // if (timeRemaining > 1){
    // PORTB ^= (1 << PB2);    // toggles the led
    // timeRemaining--;
  // }
  
// }








void led_on(void)
{
   PORTB |= _BV(PB2);
}
 
void led_off(void)
{
   PORTB &= ~_BV(PB2);
}
