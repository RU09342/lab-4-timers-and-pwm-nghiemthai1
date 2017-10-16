//******************************************************************************
//   MSP430FR6989 Demo
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


    P9DIR |=BIT7;                           //set Port 9.4 output ---LED
    P9OUT &= ~BIT7;                         //Clear P9.4

    P1DIR  &= ~BIT1;                        // Set P1.1 as input
    P1OUT |= BIT1;                          // Configure P1.1 for Pull-Up
    P1REN |= BIT1;                          // Enable Pull Up of P1.1

    TA0CCTL1 = OUTMOD_7;                    // Reset/Set Mode
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;       // SMCLK / Upmode
    TA0CCR0 = 100-1;                        // PWM Frequency 10 kHz
    TA0CCR1 = 50;                           // 50% Duty Cycle
    P1SEL0 |= BIT0;                         // PWM output to LED P1.0
    P1SEL1 &= ~BIT0;

    while(1)
    {
        if(!(P1IN & BIT1)) //If the button is pressed
        {
            P9OUT |= BIT7; //Sets P9.4
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
        if((P1IN & BIT1))
            P9OUT &= ~BIT7; //Clear P9.4
        // Debounce
        for(j=100;j>0;j--)
        {
        __delay_cycles(1000);
        }
    }
}



