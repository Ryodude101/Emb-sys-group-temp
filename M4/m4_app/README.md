# Milestone 4 M4_Application


## How to Build and Run

To build, simply type "make" while you are in the directory with the Makefile in the terminal. The hex file should be in the ./Build directory as "m4_app.hex".
		
The program uses a UART that uses serial communication. The UART uses the settings 57.6k baud, 8 data bits, 1 stop bit, and no parity. The moment you load up your serial client, the program can be used.


## Basic Functionality

The program has two modes: encoding and decoding. Encoding occurs while B1 is not pressed, and decoding while B1 is pressed. While encoding and decoding messages, input using ONLY lowercase letters. They will become uppercase and encode/decode. Capital letters will not work.

In addition to the encoding and decoding functions, there are two commands: !!!L and !!!S####. !!!L outputs the current period. If B1 is not pressed, it outputs the encode period, or the decode period if B1 is pressed. !!!S#### changes the encode period, where #### is the new 4-digit period in milliseconds. The range is therefore 1 <= x <= 9999 ms for the LED flash. Both commands MUST be activated by inputting L or S as an uppercase. Inputting as a lowercase letter will encode/decode the input and cause the commands to not work properly.
