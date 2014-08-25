#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD - 1)

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

char hex[16] = "0123456789abcdef";

int main(void) {
        // set everything to input without pullups
        DDRB = DDRC = 0;
        PORTB = PORTC = PORTD = 0;

        // Enable SPI
        SPCR  = 1<<SPE;

        // Set baud rate
        UBRRH = (unsigned char) 0;
        UBRRL = (unsigned char) 52;
        // Enable transmitter
        UCSRB = 1<<TXEN;
        // Set frame format
        UCSRC = (1<<URSEL) | (3<<UCSZ0);
        

#ifdef DEBUG
        char buffer[6];
        int buflen = 0;
#endif

        while(1) {
                int8_t i = -1;
                int r = 0;

                // Read the LCD input through SPI
                while(!(SPSR & (1<<SPIF)));
                r = SPDR;

#ifdef DEBUG
                // if debug is enabled, write the SPI bytes into a buffer
                buffer[buflen++] = r;
                if(buflen == 2) {
                        // write the buffer to the uart in hexadecimal
                        for(i = 0; i < 2; i++) {
                                while(!(UCSRA & (1<<UDRE)));
                                UDR = hex[buffer[i] / 16];
                                while(!(UCSRA & (1<<UDRE)));
                                UDR = hex[buffer[i] & 0x0F];
                                while(!(UCSRA & (1<<UDRE)));
                                UDR = ' ';
                        }
                        // line break
                        while(!(UCSRA & (1<<UDRE)));
                        UDR = '\r';
                        while(!(UCSRA & (1<<UDRE)));
                        UDR = '\n';
                        buflen = 0;

                        // wait - the numbers get flashed to the display a few times,
                        // and we don't want to read anything twice.
                        _delay_ms(120);
                }
#else
                // Decode the number
                switch(r) {
                case 0x0C: i = 9; break;
                case 0x04: i = 8; break;
                case 0xB8: i = 7; break;
                case 0x44: i = 6; break;
                case 0x4C: i = 5; break;
                case 0x1D: i = 4; break;
                case 0x8C: i = 3; break;
                case 0x86: i = 2; break;
                case 0xBD: i = 1; break;
                case 0x24: i = 0; break;
                default: break;
                }
                if(i > 0) {
                        // Write the number to the uart, in ascii
                        i += '0'; 
                        while(!(UCSRA & (1<<UDRE)));
                        UDR = i;
                        while(!(UCSRA & (1<<UDRE)));

                        // Add a linebreak
                        UDR = '\r';
                        while(!(UCSRA & (1<<UDRE)));
                        UDR = '\n';

                        // wait - the numbers get flashed to the display a few times,
                        // and we don't want to read anything twice.
                        _delay_ms(120);
                }
#endif
        }
        return 0;
}
