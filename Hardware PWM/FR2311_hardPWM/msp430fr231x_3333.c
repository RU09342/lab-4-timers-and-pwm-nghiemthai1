//******************************************************************************
//   MSP430FR2311 Demo
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
//   Thai Nghiem (collaborate with Matthew Rogdrigue
//   Rowan University
//   September 2017
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>

volatile unsigned int j;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P2DIR |=BIT0;                           //set Port 2.0 output ---LED
    P2OUT &= ~BIT0;                         //Clear P2.0

    P1DIR  &= ~BIT1;                        // Set P1.1 as input
    P1OUT |= BIT1;                          // Configure P1.1 for Pull-Up
    P1REN |= BIT1;                          // Enable Pull Up of P1.1

    TB0CCTL1 = OUTMOD_7;                    // Reset/Set Mode
    TB0CTL = TBSSEL_2 + MC_1 +TBCLR ;       // SMCLK / Upmode
    TB0CCR0 = 100-1;                        // PWM Frequency 10 kHz
    TB0CCR1 = 50;                           // 50% Duty Cycle


    while(1)
    {
        if(!(P1IN & BIT1)) //If the button is pressed
        {
            P2OUT |= BIT0; //Sets P9.4
            if(TB0CCR1 <= 90) // If the brightness is <= than 90%
            {
                TB0CCR0 = 0; // Reset CCR0
                TB0CCR1 += 10; // Add 10%
                TB0CCR0 = 100; // Set CCR0 back to 10 kHz
            }
            else if (TB0CCR1 == 100){ // If the brightness is at 100%
                TB0CCR0 = 0; // Reset CCR0
                TB0CCR1 = 0; // Reset CCR1
                TB0CCR0 = 100; //  Set CCR0 back to 10 kHz
            }
        }
        if((P1IN & BIT1))
            P2OUT &= ~BIT0; //Clear P9.4
        // Debounce
        for(j=100;j>0;j--)
        {
        __delay_cycles(1000);
        }
    }
}
