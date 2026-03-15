#include "../../../Drivers/CMSIS/Device/ST/STM32F1xx/Include/stm32f103xb.h"
//#include "../../../Drivers/CMSIS/Device/ST/STM32F1xx/Include/stm32f1xx.h"

int main(void) {
   volatile uint32_t delay = 200000;

    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    GPIOB->CRL &= ~GPIO_CRL_MODE2_0;
    GPIOB->CRL |= GPIO_CRL_MODE2_1;
    GPIOB->CRL &= ~GPIO_CRL_CNF2;
    
    while(1) {
        GPIOB->BSRR |= GPIO_BSRR_BS2;
        for(int i = 0; i < delay; i++);
        GPIOB->BSRR |= GPIO_BSRR_BR2;
        for(int i = 0; i < delay; i++);
    }
}   