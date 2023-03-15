#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

unsigned char hours=0;
unsigned char minutes=0;
unsigned char seconds=0;

ISR(TIMER1_COMPA_vect)
{
	seconds++;
	if (seconds==60)
	{
		minutes++;
		seconds=0;
	}
	if (minutes==60)
	{
		hours++;
		minutes=0;
		seconds=0;
	}
	if (hours==60)
		{
			hours=0;
			minutes=0;
			seconds=0;
		}
}

ISR(INT0_vect)
{
	seconds=0;
	minutes=0;
	hours=0;
}

ISR(INT1_vect)
{
	TCCR1B &= ~(1<<1) & ~(1<<2) & ~(1<<0);
}


ISR(INT2_vect)
{
	TCCR1B |= (1<<0) | (1<<2);
}

void Timer1(void)
{
	TCCR1A |= 1<<FOC1A;
	TCCR1B |= 1<<WGM12 | 1<<CS12 | 1<<CS10;
	TCNT1=0;
	OCR1A= 976;
	TIMSK= (1<<4);

}

void interrupt0()
{
	DDRD &=~ 1<<PD2;
	PORTD |= 1<<PD2;
	GICR |= 1<<INT0;
	MCUCR |= 1<<ISC01;
}

void interrupt1()
{
	DDRD = ~ (1<<3);
	MCUCR = (1<<3)|(1<<2);
	GICR= (1<<7);

}

void interrupt2()
{
	DDRB &=~ (1<<2);
	PORTB |= (1<<2);
	GICR |= (1<<5);
	MCUCSR &=~ (1<<6);
}

int main (void)
{
	SREG |= (1<<7);
	DDRC |= 0x0F;
	PORTC = 0x00;
	DDRA |= 0x3F;




	interrupt0();
	interrupt1();
	interrupt2();
	Timer1();
	while(1)
	{
		PORTA = (1<<0);
		PORTC = (seconds %10);
		_delay_ms(4);


		PORTA = (1<<1);
		PORTC = seconds/10;
		_delay_ms(4);


		PORTA = (1<<2);
		PORTC = (minutes %10);
		_delay_ms(4);


		PORTA = (1<<3);
		PORTC = minutes/10;
		_delay_ms(4);


		PORTA = (1<<4);
		PORTC = (hours %10);
		_delay_ms(4);

		PORTA = (1<<5);
		PORTC = hours/10;
		_delay_ms(4);

	}
}

