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
#include <Math.h>

volatile unsigned int j;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P2DIR |=BIT0; //set Port 2.0 output ---LED
    P2OUT &= ~BIT0; //Clear P2.0

    P1DIR  &= ~BIT1;                        // Set P1.1 as input
    P1OUT |= BIT1;                          // Configure P1.1 for Pull-Up
    P1REN |= BIT1;                          // Enable Pull Up of P1.1
    P1IE |= BIT1; //enable the interrupt on Port 1.1
    P1IES &= ~BIT1; //set as falling edge
    P1IFG &= ~(BIT1); //clear interrupt flag


    TB0CTL = TBSSEL_2 + MC_1 ;       // SMCLK / Upmode
    TB0CCTL1 = (CCIE);              //CCTL1 Compare/Capture Interrupt Enable
    TB0CCTL0 = (CCIE);              //CCTL1 Compare/Capture Interrupt Enable
    TB0CCR0 = 1000-1;                        // PWM Frequency 10 kHz
    TB0CCR1 = 500;                           // 50% Duty Cycle

    __bis_SR_register(GIE);
    while(1)
    {
        if((P1IN & BIT1))
            P2OUT &= ~BIT0; //Clear P2.0

    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
    P1IE &= ~BIT1; //Port 1 interrupt enable is turned off
    __delay_cycles(1000);  //Debounce
    P1IE |= BIT1; //Port 1 interrupt enable is turned back on

    P2OUT |= BIT0; //Sets P2.0
    if(TB0CCR1 >= 1000) // If the brightness is at 100%
    {
        TB0CCR0 = 0; // Reset CCR0
        TB0CCR1 = 0;// Reset CCR1
        TB0CCR0 = 1000; //  Set CCR0 back to 10 kHz
    }
    else if (TB0CCR1 < 1000){ // If the brightness is <= than 90%
        TB0CCR0 = 0; // Reset CCR0
        TB0CCR1 += 100; // Add 10%
        TB0CCR0 = 1000;//  Set CCR0 back to 10 kHz


    }
    P1IFG &= ~BIT1; //Clear flag
}

//Timer A interrupt vectors
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer0_B1_ISR (void)
{
    if(TB0CCR1 != 1000)
    {
       P1OUT &= ~(BIT0); //turns off red led
    }
    TB0CCTL1 &= ~BIT0; //clears flag
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR (void)
{
    if(TB0CCR1 != 0){
        P1OUT |= (BIT0); //turns on red led
    }
    TB0CCTL0 &= ~BIT0;  //clears flag
}
