coinspi
=======

This is firmware for AVR microcontrollers with SPI and a UART to
intercept the CH-926 coin acceptor's communication with its 7-segment
display and write non-zero numbers to the UART.

Rationale
---------

If you put coins into the CH-926 in rapid succession, it is impossible
to know which coin a pulse belongs to (so two pulses might be one coin
with two pulses or two coins with one pulse).

Because of this, the number of pulses has to be proportional to the
value of the coin.  Using everything from 5 cents to 2 Euro coins
means that the 5 cent coin has one pulse and the 2 Euro coin 40 - but
every pulse needs 130 milliseconds, so this will take over 5 seconds
to read!

The device also flashes the number of pulses on a 7-segment
display. Reading this display is much faster than waiting for the
pulses, and you don't need to make the coin pulses proportional to
their value.

How does it work?
-----------------

The 7-segment display is controlled through a shift register, which
gets its values from the CH-926's microcontroller through SPI. We
simply attach our own microcontroller to the SPI bus and listen to the
communication. After we've decoded the digits, we write them to the
UART. After that we sleep for a few microseconds so we don't read any
numbers twice - the display gets the same signal three times in a row,
and you wouldn't want to end up with only a third of the money at the
end of the day :)

"Installation"
--------------

Flash the firmware to e.g. an ATMega8 (making sure to set it to run on
its own internal oscillator, and that your CPU frequency's set
correctly), connect its SPI pins to the shift register's inputs, and
connect its UART's TX pin to where you need it. In our coin acceptor
we cut the trace of the now useless signal wire that used to output
the pulses and put the UART signal in its place instead. You could
even output everything with a bluetooth module.

TODO: Document the SPI pins on the shift register properly.