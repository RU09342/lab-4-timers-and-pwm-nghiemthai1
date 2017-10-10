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

//void main(void) {
//    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer
//
//    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
//                                            // to activate previously configured port settings
//
//    P1DIR |=BIT0; //set Port 1.0 output ---LED
//    P9DIR |=BIT7; //set Port 9.4 output ---LED
//    P9OUT &= ~BIT7; //Clear P9.4
//
//    P1DIR &=~(BIT1); //set Port 1.1 input --- pushbutton
//    P1REN|=BIT1;//enable pull-up/pull-down resistor on
//    P1OUT|=BIT1; //choose the pull-up resistor
//
//    P1IE |=BIT1;//enable the interrupt on Port 1.1
//    P1IES |=BIT1;//set as falling edge
//    P1IFG &=~(BIT1);//clear interrupt flag
//
//    TB0CTL = TASSEL_1 + MC_1; // Timer SMCLK Mode UP
//    TB0CCTL0 = 0x10 + OUTMOD_7; //Set up compare mode for CCTL
//    TBCCR0 = 1000-1; // Cargamos el periodo PWM
//    TBCCR1 = 100; // PWM duty cycle, 10% CCR1/(CCR0+1) * 100
//    __enable_interrupt(); //enable interrupt
//
//    //enter LPM4 mode and enable global interrupt
//    _BIS_SR(LPM4_bits + GIE);
//}
////Port 1 ISR
//#pragma vector=PORT1_VECTOR
//__interrupt void PORT_1(void)
//{
//    P9OUT ^= BIT7; // Change state of P9.7
//
//    P1IE &= ~BIT1; // Disable interrupt
//
//    //Debounce
//    TA0CTL = TASSEL_1 + MC_1 + ID_1 + TACLR; //Set up Timer A, Count up, clear TA
//    TA0CCTL0 = 0x10; // Set up compare mode for CCTL
//    TA0CCR0 = 1000; // Duration at which the interrupt is disable
//                    //
//
//    P1IES ^= BIT1; //toggle to set as rising edge
//    P1IE |= BIT1;
//    P1IFG &= ~(BIT1);
//}
//#pragma vector=TIMER0_A0_VECTOR
//__interrupt void Timer_A0(void)
//{
//
//    P1IE |= BIT1; //Enable interrupt again.
//
//}
//#pragma vector=TIMER0_B0_VECTOR
//__interrupt void Timer_B0(void)
//{
//
//    P9OUT ^= BIT7; // Change state of P1.1
//
//}

