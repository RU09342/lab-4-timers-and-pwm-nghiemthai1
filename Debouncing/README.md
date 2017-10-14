# Read Me for Debouncing
Author: Thai Nghiem

## Description
Prevent the button from bouncing by using 2 methods: setting up a delay cycle and using timer to toggle the interrupt enable bit. 
Board general application: Interrupt a ongoing process by pushing a button. 
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

After the button is pushed, it bounces for few times at a high frequency. To prevent this from happening, a period of time 
should be set up to ignore the bounce. The first method we use is setting up delay cycle through this line:
```c
__delay_cycles(1);
```
This will delay the action of the msp430 the specified time (it's "1" in this case). However, using delay cycle might cause 
some unwanted result cause you will delay every action of the msp430, including counting up the timer. So we also implement 
a second method of debouncing, in which we using Timer_A (or B) to prolong the time Interupt Enable bit is disabled. <br />
In the first interrupt that is caused by the button, we turn of the Port 1 Interupt Enable register. The timer is then set up 
for 1/8 sec using the lines
```c
    TA0CTL = TASSEL_1 + MC_1 + ID_1; 
    TA0CCTL0 = 0x10; 
    TA0CCR0 = 2000; 
```
In the interrupt, the Port 1 Interupt Enable register is set back on.
```c
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void){
	P1IE |= BIT3; //Enable interrupt again. 
}
```

## Changes across the boards
There is not much changes on the code across the 5 boards in this project, except for the specific output pin number for each LED and button. <br />
However, the msp430FRxxx series (FR6989, FR2311, and FR5994 in this case) need to use the line PM5CTL0 = ~LOCKLPM5 to disable the default high impedance on the board. 
This high impedance serves to get rid of any cross currents, but is turned off later. <br />
Also, the FR2311 doesn't have Timer_A, so Timer_B must be used inplace of Timer_A.
# How to implement the code
To run this code, simply import it into code composer, then click build. 
After you plug in your MSP430, hit debug. When you press the button, LED1 one the board should change its state
