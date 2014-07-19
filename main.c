#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define PORT               	PORTB
#define DDR 				DDRB
#define DATA1			    0				// data  pin 74hc164
#define CLK1		  		1				// Clock pin 74hc164
#define DATA2			    2				// data  pin 74hc164
#define CLK2		  		3				// Clock pin 74hc164

volatile unsigned int millisekunden;
volatile unsigned int sekunde;
volatile unsigned int minute;

volatile unsigned int set_minute;
volatile unsigned int timeractivated;

volatile unsigned int segments[] = {0b01111110,0b01001000,0b00111101,0b01101101,0b01001011,0b01100111,0b01110111,0b01001100,0b01111111,0b01101111};

volatile unsigned int einer;
volatile unsigned int zehner;


void shiftregister1_loadbyte(uint8_t outbyte)
{
	PORT &=~ (1<<DATA1);
		
        for (int i=8;i>0;i--)
        {   
             if (outbyte & 0x80)
                {
                PORT |= (1<<DATA1); 
                }
                else
                {
				PORT &= ~(1<<DATA1);	
                }
            outbyte = outbyte << 1;	
            PORT |= (1<<CLK1);
            PORT &= ~(1<<CLK1);			
        }	
}

void shiftregister2_loadbyte(uint8_t outbyte)
{
	PORT &=~ (1<<DATA2);
		
        for (int i=8;i>0;i--)
        {   
             if (outbyte & 0x80)
                {
                PORT |= (1<<DATA2); 
                }
                else
                {
				PORT &= ~(1<<DATA2);	
                }
            outbyte = outbyte << 1;	
            PORT |= (1<<CLK2);
            PORT &= ~(1<<CLK2);			
        }	
}

void entprellung( volatile uint8_t *port, uint8_t maske ) 
{
  uint8_t   port_puffer;
  uint8_t   entprellungs_puffer;
 
  for( entprellungs_puffer=0 ; entprellungs_puffer!=0xff ; ) 
  {
    entprellungs_puffer<<=1;
    port_puffer = *port;
    _delay_us(500);
    if( (*port & maske) == (port_puffer & maske) )
      entprellungs_puffer |= 0x01;
  }
}

//-----------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------

int main()
{
	DDRD = (1 << PD0) | (1 << PD1);
	DDRD &= ~(1 << PD5) | (1 << PD4) | (1 << PD3);
	DDR = (1<<DATA1) | (1<<CLK1) | (1<<DATA2) | (1<<CLK2);
	
	TCCR0A = (1<<WGM01); 
	TCCR0B |= (1<<CS01); 
	// ((1000000/8)/1000) = 125
	OCR0A = 125-1;
 
	TIMSK |= (1<<OCIE0A);
 
	// sei();
	
	while(1)
	{
	
		if(timeractivated == 0)
		{
			entprellung( &PIND, (1<<PIND5) ); 
			if( PIND & (1<<PIND5) )
			{
				minute++;
				PORTD &= ~(1 << PD1);
			}
		
			entprellung( &PIND, (1<<PIND4) );
			if( PIND & (1<<PIND4) )
			{
				minute--;
				PORTD &= ~(1 << PD1);
			}
			
			entprellung( &PIND, (1<<PIND3) ); 
			if( PIND & (1<<PIND3) )
			{
				set_minute = minute;
				timeractivated = 1;
				minute = 0;
				sei();
			}

		}
		
		if(timeractivated == 1)
		{
			if(minute == set_minute)
			{
				cli();
				PORTD |= (1 << PD1);
				timeractivated = 0;
			}
		}
		
		
		if(set_minute > 99 || minute > 99 || set_minute < 0 || minute < 0 )
		{
			set_minute = 0;
			minute = 0;
		}
		
		zehner=(minute%100)/10;
		einer=minute%10;
			
		if(zehner < 10)
		{
			shiftregister1_loadbyte(segments[zehner]);
			_delay_ms(1);
		}
			
		if(einer < 10)
		{
			shiftregister2_loadbyte(segments[einer]);
			_delay_ms(1);
		}
	}
}

//------------------------------------------------------------------------------

ISR (TIMER0_COMPA_vect)
{
  millisekunden++;
  if(millisekunden == 1000)
  {
    sekunde++;
    millisekunden = 0;
	PORTD ^= (1 << PD0);
    if(sekunde == 60)
    {
      minute++;
      sekunde = 0;
    }
  }
}

