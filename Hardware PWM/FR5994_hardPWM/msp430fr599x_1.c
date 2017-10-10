//******************************************************************************
//  MSP430FR5x9x Demo
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
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P1DIR |=BIT1;                           //set Port 1.1 output ---LED
    P1OUT &= ~BIT1;                         //Clear P1.1

    P5DIR  &= ~BIT6;                        // Set P5.6 as input
    P5OUT |= BIT6;                          // Configure P5.6 for Pull-Up
    P5REN |= BIT6;                          // Enable Pull Up of P5.6

    TA0CCTL1 = OUTMOD_7;                    // Reset/Set Mode
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;       // SMCLK / Upmode
    TA0CCR0 = 100-1;                        // PWM Frequency 10 kHz
    TA0CCR1 = 50;                           // 50% Duty Cycle
    P1SEL0 |= BIT0;                         // PWM output to LED P1.0
    P1SEL1 &= ~BIT0;

    while(1)
    {
        if(!(P5IN & BIT6)) //If the button is pressed
        {
            P1OUT |= BIT1; //Sets P1.1
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
        if((P5IN & BIT6))
            P1OUT &= ~BIT1; //Clear P1.1
        // Debounce
        for(j=100;j>0;j--)
        {
        __delay_cycles(1000);
        }
    }
}
