/* One GPIO Soft PWR switch with reverse polarity protection.
 * The GPIO pin is further usable in the code as an input pin (IPU) for button input.
 */
#include "ch32v00x.h"

void GPIO_init(void){                          // GPIOA Setup
    RCC->APB2PCENR |= RCC_IOPAEN | RCC_AFIOEN; // Clock ENABLE GPIOA a AFIO
    AFIO->PCFR1 &= ~(1 << 15);                 // PA12_RM bit clear for PA1 and PA2 -> General IO Pin

    GPIOA->CFGLR &= ~(0xF << 8);               // Reset PA2 config
    GPIOA->CFGLR |=  (0x8 << 8);               // PA2 Input-PullUp
    GPIOA->OUTDR |=  (0x1 << 2);               // PA2 pull-up ENABLE -> TURN ON PWR HOLD
                                               // GPIOC Setup
    RCC->APB2PCENR |= RCC_IOPCEN;              // Clock ENABLE GPIOC
    GPIOC->CFGLR &= ~(0xF << 8);               // Reset PC2 config
    GPIOC->CFGLR |=  (0x1 << 8);               // PC2, GPIO_Speed_10MHz, OUTPUT
}

int main(void) {
    GPIO_init();
    Delay_Init();

    GPIOC->BSHR = (1 << 2);                    // Turn ON LED on PC2
    while (!(GPIOA->INDR & (1 << 2))){         // Wait while Button is released (PA2 == 1)
        Delay_Ms(20);
    }

    while (1) {
        if (!(GPIOA->INDR & (1 << 2))) {       // If button pressed
         GPIOC->BSHR = (1 << 18);              // Turn OFF LED on PC2
         GPIOA->OUTDR &= ~(0xF << 2);          // Reset PA2 config
         GPIOA->OUTDR |=  (0 << 2);            // PA2 pull-down ENABLE -> TURN OFF PWR HOLD
          while (!(GPIOA->INDR & (1 << 2))){   // Wait while Button is released (PA2 == 1)
            Delay_Ms(20);
        }
      }
   }
}
