# Read Me for Extra Work
Thai Nghiem (collaborate with Ardit Pranvoku)

To run this code, simply import the project folder into code composer, build, then debug.
Uses the timer to generate a signal that will cause an LED to blink at a designated PWM.
Uses logarithms to account so the PWM is easier to see with the human eye. <br />
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
The desired led pins and bits must be set to 1 to configure it to be an output.
The desired button pin and bit must be to 0 to configure it to be an input.
Also,  PXREN |= BITX; must be used to enable the pullup resistor for that button.     


TA0CTL is configured using the desired settings. We used Timer A, SMCLK, up mode, and clear TAR so our code was:
TA0CTL = TASSEL_2 + MC_1 + TACLR;
We also set TA0CCTL to OUTMOD_7 so the timer out mode is in reset/set.

P1SEL0 |= BIT0; and P1SEL1 &= ~BIT0;
 directs the signal of the timer to LED 1.0.

The program uses a button press to change TA0CCR1 logmarithically.
taps is initialized to 10.
While the button is pressed, taps is decreased by 1.
logNum is then set to log10(taps) * 100, which will start off at log10(9) * 100 and decrease
every loop until it ends at 0.
incrementNum is set to 100 - logNum, so it will start off at 0 and decrease every loop until it ends at
at 100. 
Finally, TA0CCR1 is set equal to incrementNum, so it will behave similarly.
This means that that button will start dim and increase logmarithically to 100. 

The CPU will always wait 100,000 cycles before incrementing again.
At 100% duty cycle, taps is reset back to 10.

## Changes across the boards
There is not much changes on the code across the 5 boards in this project, except for the specific output pin number for each LED and button. <br />
However, the msp430FRxxx series (FR6989, FR2311, and FR5994 in this case) need to use the line PM5CTL0 = ~LOCKLPM5 to disable the default high impedance on the board. 
This high impedance serves to get rid of any cross currents, but is turned off later. <br />
Also, the FR2311 doesn't have Timer_A, so Timer_B must be used inplace of Timer_A.
# How to implement the code
To run this code, simply import it into code composer, then click build. 
After you plug in your MSP430, hit debug. When you press the button, LED1 one the board should change its state