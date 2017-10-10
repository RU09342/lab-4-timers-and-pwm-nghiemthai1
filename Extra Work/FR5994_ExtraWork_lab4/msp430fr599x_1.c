//******************************************************************************
//   MSP430F552x Demo
//      PWM
//
//   Description: Every time you press the PWM button, increase the brightness of
//                  the LED logarithmically. Once it reaches 100% brightness, it rolls
//                  back to 0% brightness.
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
//   Thai Nghiem (collaborate with Matthew R. and Ardit P.)
//   Rowan University
//   September 2017
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>
#include <Math.h>

volatile unsigned int j;
int taps = 10;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P1DIR |=BIT1; //set Port 9.4 output ---LED
    P1OUT &= ~BIT1; //Clear P9.4

    P5DIR  &= ~BIT6;                        // Set P1.1 as input
    P5OUT |= BIT6;                          // Configure P1.1 for Pull-Up
    P5REN |= BIT6;                          // Enable Pull Up of P1.1

    TA0CCTL1 = OUTMOD_7;                    // Reset/Set Mode
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;       // SMCLK / Upmode
    TA0CCR0 = 100-1;                        // PWM Frequency 10 kHz
    TA0CCR1 = 50;                           // 50% Duty Cycle
    P1SEL0 |= BIT0;                         // PWM output to LED P1.0
    P1SEL1 &= ~BIT0;

    while(1)
    {
        if(!(P5IN & BIT6))
        {
            P1OUT |= BIT1; //Sets P9.4
            if(taps > 0)
            {
                taps--;
                double logNum  = log10(taps) * 100.0;
                int incrementNum = (100 - logNum);
                TA0CCR0 = 0;
                TA0CCR1 = incrementNum;
                TA0CCR0 = 100;
            }
            else if (taps == 0){
                TA0CCR0 = 0;
                TA0CCR1 = 0;
                TA0CCR0 = 100;
                taps = 10;
            }
        }
        if((P5IN & BIT6))
            P1OUT &= ~BIT1; //Clear P9.4
        for(j=100;j>0;j--)
        {
        __delay_cycles(1000);
        }
    }
}
