//******************************************************************************
//   MSP430F5529 Demo
//      Button Interrupt
//
//   Description: Interrupt a ongoing process by pushing a button
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
//   Thai Nghiem (collaborate with Matthew Rodriguez)
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************
#include <msp430.h>
#include <Math.h>

volatile unsigned int j;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P1DIR |= BIT0;                          // Set P1.0 to output direction
    P1OUT &= ~BIT0;                         // Switch LED off


    P4DIR |=BIT7; //set Port 4.7 output ---LED
    P4OUT &= ~BIT7; //Clear P4.7

    P1DIR  &= ~BIT1;                        // Set P1.1 as input
    P1OUT |= BIT1;                          // Configure P1.1 for Pull-Up
    P1REN |= BIT1;                          // Enable Pull Up of P1.1
    P1IE |= BIT1; //enable the interrupt on Port 1.1
    P1IES &= ~BIT1; //set as falling edge
    P1IFG &= ~(BIT1); //clear interrupt flag


    TA0CTL = TASSEL_2 + MC_1 ;       // SMCLK / Upmode
    TA0CCTL1 = (CCIE);              //CCTL1 Compare/Capture Interrupt Enable
    TA0CCTL0 = (CCIE);              //CCTL1 Compare/Capture Interrupt Enable
    TA0CCR0 = 1000-1;                        // PWM Frequency 10 kHz
    TA0CCR1 = 500;                           // 50% Duty Cycle

    __bis_SR_register(GIE);
    while(1)
    {
        if((P1IN & BIT1))
            P4OUT &= ~BIT7; //Clear P4.7

    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
    P1IE &= ~BIT1; //Port 1 interrupt enable is turned off
    __delay_cycles(1000);  //Debounce
    P1IE |= BIT1; //Port 1 interrupt enable is turned back on

    P4OUT |= BIT7; //Sets P4.7
    if(TA0CCR1 >= 1000) // If the brightness is at 100%
    {
        TA0CCR0 = 0; // Reset CCR0
        TA0CCR1 = 0;// Reset CCR1
        TA0CCR0 = 1000; //  Set CCR0 back to 10 kHz
    }
    else if (TA0CCR1 < 1000){ // If the brightness is <= than 90%
        TA0CCR0 = 0; // Reset CCR0
        TA0CCR1 += 100; // Add 10%
        TA0CCR0 = 1000;//  Set CCR0 back to 10 kHz


    }
    P1IFG &= ~BIT1; //Clear flag
}

//Timer A interrupt vectors
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR (void)
{
    if(TA0CCR1 != 1000)
    {
       P1OUT &= ~(BIT0); //turns off red led
    }
    TA0CCTL1 &= ~BIT0; //clears flag
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
{
    if(TA0CCR1 != 0){
        P1OUT |= (BIT0); //turns on red led
    }
    TA0CCTL0 &= ~BIT0;  //clears flag
}
