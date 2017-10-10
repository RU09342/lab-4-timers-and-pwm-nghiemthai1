//  MSP430G2xx3 Demo
//      PWM
//
//   Description: Every time you press the PWM button, increase the brightness of
//                  the LED by 10%. Once it reaches 100% brightness, it rolls back
//                  to 0% brightness.
//   ACLK = 32.768kHz, MCLK = SMCLK = default DCO~1MHz
//
//                MSP430
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |                 |
//            |             P1.0|-->LED
//
//   Thai Nghiem (collaborate with Matthew Rogdrigue)
//   Rowan University
//   September 2017
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

volatile unsigned int j;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P1DIR |=BIT6;                           //set Port 9.4 output ---LED
    P1OUT &= ~BIT6;                         //Clear P9.4

    P1DIR  &= ~BIT3;                        // Set P1.3 as input
    P1OUT |= BIT3;                          // Configure P1.3 for Pull-Up
    P1REN |= BIT3;                          // Enable Pull Up of P1.3

    TA0CCTL1 = OUTMOD_7;                    // Reset/Set Mode
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;       // SMCLK / Upmode
    TA0CCR0 = 100-1;                        // PWM Frequency 10 kHz
    TA0CCR1 = 50;                           // 50% Duty Cycle


    while(1)
    {
        if(!(P1IN & BIT3)) //If the button is pressed
        {
            P1OUT |= BIT6; //Sets P1.6
            if(TA0CCR1 <= 90) // If the brightness is <= than 90%
            {
                TA0CCR0 = 0; // Reset CCR0
                TA0CCR1 += 10; // Add 10%
                TA0CCR0 = 100; // Set CCR0 back to 10 kHz
            }
            else if (TA0CCR1 == 100){ // If the brightness is at 100%
                TA0CCR0 = 0; // Reset CCR0
                TA0CCR1 = 0; // Reset CCR1
                TA0CCR0 = 100; //  Set CCR0 back to 10 kHz
            }
        }
        if((P1IN & BIT3)) // If button is not pressed
            P1OUT &= ~BIT6; //Clear P4.7
        // Debounce
        for(j=100;j>0;j--)
        {
        __delay_cycles(1000);
        }
    }
}
