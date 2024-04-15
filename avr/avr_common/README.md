# Avr Common
Very useful functions for AVR

## UART
We started from Grisetti functions and we have modified some things:

- `UART_init`: we change init mode, disabiliting completely Interrupt [RXCIE0] from UART
- `UART_unblocking`: We have created very funny UART read, timerized and unblocking for our game.


## UTILS

Very basic functions to debug and fix module Operator (for display Shifting in Game)
