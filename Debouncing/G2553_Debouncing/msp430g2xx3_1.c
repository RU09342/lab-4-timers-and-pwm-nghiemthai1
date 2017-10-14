//******************************************************************************
//   MSP430G2xxx Demo
//      Button Interrupt
//
//   Description: Interrupt a ongoing process by pushing a button
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                MSP430F552x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.0|-->LED
//
//   Thai Nghiem
//   Rowan University
//   September 2017
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer

    P1SEL =0; //Select GPIO option
    P1DIR |=BIT0; //set Port 1.0 output ---LED

    P1DIR &=~(BIT3); //set Port 1.1 input --- pushbutton
    P1REN|=BIT3;//enable pull-up/pull-down resistor on
    P1OUT|=BIT3; //choose the pull-up resistor

    P1IE |=BIT3;//enable the interrupt on Port 1.1
    P1IES |=BIT3;//set as falling edge
    P1IFG &=~(BIT3);//clear interrupt flag

    //enter LPM0 mode and enable global interrupt
    _BIS_SR(LPM0_bits + GIE);
}
//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1OUT ^=0x01; // Change state of P1.1
    P1IE &= ~BIT3; // Disable interrupt
    //Debounce 1
    __delay_cycles(1);

    //Debounce 2
    TA0CTL = TASSEL_1 + MC_1 + ID_1; //Set up Timer A, Count up, divider 2
    TA0CCTL0 = 0x10; // Set up compare mode for CCTL
    TA0CCR0 = 2000; // Duration at which the interrupt is disable
                    // Duration 2000/16kHz = 1/8 sec.

    P1IFG &=~(BIT3); // Clear flag
}
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    P1IE |= BIT3; //Enable interrupt again.

}
