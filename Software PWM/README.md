# Read Me for Software PWM
Author: Thai Nghiem (collaborated with Ardit Pranvoku)

Uses the timer to generate a signal that will cause an LED to blink at a designated PWM.
The watchdog timer must be stopped with the line 
```c
WDTCTL = WDTPW + WDTHOLD or WDTCTL = WDTPW | WDTHOLD.
```
Else, the processor will reset. <br />
The desired led pins and bits must be set to 1 to configure it to be an output.
The desired button pin and bit must be to 0 to configure it to be an input .<br />
Also,  PXREN |= BITX; must be used to enable the pullup resistor for that button. <br />    
The processor is put into LPM4 to prepare for the interrupt from the button. <br />
In the same line, GIE is enabled so the interrupt is not masked. <br />

TA0CTL is configured using the desired settings. We used Timer A, SMCLK, and up mode.
```c
TA0CTL = TASSEL_2 + MC_1
```

TA0CCR0 is initialized at 999.
TA0CCR1 is initialized at 500, so the duty cycle is 50% at 1 kHz. This is because the LED
is set at 999(CCR0) and reset at 500, (CCR1) therefore it's on half the time. <br />

The program uses a button based interrupt to change TA0CCR1.
While the button is pressed, TA0CCR1 is increased by 100, adding 10% to the duty cycle.
At 100% duty cycle, TA0CCR1 is reset to 0. 

When TAR hits CCR0, an interrupt is generated for the LED to turn on. 
When TAR hits CCR1, an interrupt is generated for the LED to turn off.
Thus, if CCR0 is at 999 and CCR1 is at 300, the LED will be on from
999 to 300, and off from 300 to 999, a 30% duty cycle.

Both interrupts generated from the timer check to make sure that 
if CCR1 is 1000 or 0, it will not turn off or will turn off, respectively.
This is done to make sure that if CCR1 is 1000, it never turns off since it should be at a 100%
duty cycle.
Similarly, if CCR1 is 0, it never turns on since it should be at a 100% duty cycle.

## Changes across the boards
There is not much changes on the code across the 5 boards in this project, except for the specific output pin number for each LED and button. <br />
However, the msp430FRxxx series (FR6989, FR2311, and FR5994 in this case) need to use the line PM5CTL0 = ~LOCKLPM5 to disable the default high impedance on the board. 
This high impedance serves to get rid of any cross currents, but is turned off later. <br />
Also, the FR2311 doesn't have Timer_A, so Timer_B must be used inplace of Timer_A.
# How to implement the code
To run this code, simply import it into code composer, then click build. 
After you plug in your MSP430, hit debug. When you press the button, LED1 one the board should change its state