#include "../../../Drivers/CMSIS/Device/ST/STM32F1xx/Include/stm32f103xb.h"

/**
 * @brief  Задержка
 */
void Delay(volatile uint32_t count)
{
    while (count--) { __NOP(); }
}

/**
 * @brief  Инициализация GPIO
 * @note   Кнопка подключена к GND (один вывод на PA0, другой на GND)
 *         Используем внутренний pull-up резистор
 */
void GPIO_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* PB2 - выход для светодиода */
    GPIOB->CRL &= ~GPIO_CRL_MODE2;
    GPIOB->CRL |= GPIO_CRL_MODE2_1;      /* 50 МГц */
    GPIOB->CRL &= ~GPIO_CRL_CNF2;        /* Push-pull */
    GPIOB->BSRR = GPIO_BSRR_BR2;         /* LED выключен */

    /* PA0 - вход с pull-up (кнопка на GND)
     * CNF[1:0] = 10 - вход с pull-up/pull-down
     * MODE[1:0] = 00 - вход
     * ODR = 1 - включить pull-up
     */
    GPIOA->CRL &= ~GPIO_CRL_MODE0;       /* Вход */
    GPIOA->CRL &= ~GPIO_CRL_CNF0;        /* Очистка CNF */
    GPIOA->CRL |= GPIO_CRL_CNF0_1;       /* CNF[1:0] = 10 - pull-up/pull-down */
    GPIOA->BSRR = GPIO_BSRR_BS0;         /* Включить pull-up (запись 1 в ODR) */
}

/**
 * @brief  Чтение кнопки
 * @retval 0 - кнопка нажата (соединена с GND)
 * @retval 1 - кнопка отпущена (pull-up тянет вверх)
 */
uint8_t Button_Read(void)
{
    return (GPIOA->IDR & GPIO_IDR_IDR0) ? 1 : 0;
}

/**
 * @brief  Чтение кнопки с антидребезгом
 */
uint8_t Button_Read_Debounced(void)
{
    static uint8_t last_state = 1;
    uint8_t current = Button_Read();
    
    if (current != last_state) {
        Delay(50000);  /* ~50 мс */
        current = Button_Read();
        last_state = current;
    }
    return current;
}

void LED_On(void)  { GPIOB->BSRR = GPIO_BSRR_BS2; }
void LED_Off(void) { GPIOB->BSRR = GPIO_BSRR_BR2; }
void LED_Toggle(void) { GPIOB->ODR ^= GPIO_ODR_ODR2; }

int main(void)
{
    volatile uint32_t delay = 300000;
    uint8_t btn;

    GPIO_Init();
    Delay(100000);

    while (1)
    {
        /* ===== ТЕСТ 1: Мигание ===== */
        /*
        LED_On();
        Delay(delay);
        LED_Off();
        Delay(delay);
        */
        /* ===== ТЕСТ 2: LED горит при нажатой кнопке ===== */
        /* Раскомментировать для проверки */
        
        btn = Button_Read();
        if (btn != 0) {      // 0 = нажата (GND)
            LED_On();
        } else {
            LED_Off();
        }
        

        /* ===== ТЕСТ 3: Переключение по кнопке ===== */
        /* Раскомментировать для проверки */
        /*
        if (Button_Read_Debounced() == 0) {  // Нажатие
            LED_Toggle();
            Delay(200000);  // Задержка после нажатия
        }
        */
    }
}
