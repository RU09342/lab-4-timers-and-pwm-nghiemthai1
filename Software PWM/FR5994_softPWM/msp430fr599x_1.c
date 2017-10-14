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


    P1DIR |=BIT1; //set Port 1.1 output ---LED
    P1OUT &= ~BIT1; //Clear P1.1

    P5DIR  &= ~BIT5;                        // Set P5.5 as input
    P5OUT |= BIT5;                          // Configure P5.5 for Pull-Up
    P5REN |= BIT5;                          // Enable Pull Up of P5.5
    P5IE |= BIT5; //enable the interrupt on Port 5.5
    P5IES &= ~BIT5; //set as falling edge
    P5IFG &= ~(BIT5); //clear interrupt flag


    TA0CTL = TASSEL_2 + MC_1 ;       // SMCLK / Upmode
     TA0CCTL1 = (CCIE);              //CCTL1 Compare/Capture Interrupt Enable
     TA0CCTL0 = (CCIE);              //CCTL1 Compare/Capture Interrupt Enable
     TA0CCR0 = 1000-1;                        // PWM Frequency 10 kHz
     TA0CCR1 = 500;                           // 50% Duty Cycle

     __bis_SR_register(GIE);
     while(1)
     {
         if((P5IN & BIT5))
             P1OUT &= ~BIT1; //Clear P1.1

     }
 }

 #pragma vector=PORT5_VECTOR
 __interrupt void PORT5_IRS(void)
 {
     P5IE &= ~BIT5; //Port 5 interrupt enable is turned off
     __delay_cycles(1000);  //Debounce
     P5IE |= BIT5; //Port 1 interrupt enable is turned back on

     P1OUT |= BIT1; //Sets P1.1
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
     P5IFG &= ~BIT5; //Clear flag
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

