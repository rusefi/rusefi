#include "stm32f4xx.h"

/**
 * SPI 2
 *
 * Chip Select: PD11
 *
 *
 * http://www.ti.com/lit/ds/symlink/tpic8101.pdf
 * http://www.intersil.com/content/dam/Intersil/documents/an97/an9770.pdf
 * http://e2e.ti.com/cfs-file/__key/telligent-evolution-components-attachments/00-26-01-00-00-42-36-40/TPIC8101-Training.pdf
 *
 */


#define NO_DATA 0x0100

#define RX_BUF_SIZE 128
#define TX_BUF_SIZE 128 

uint8_t rx_buf[RX_BUF_SIZE];
uint8_t tx_buf[TX_BUF_SIZE];

volatile uint16_t rx_head;
volatile uint16_t rx_tail;
volatile uint16_t tx_head;
volatile uint16_t tx_tail;

void uart_putc(uint8_t c);

uint16_t uart_getc(void);

uint16_t spi(uint16_t data);

int main(void)
{
    RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN);
    RCC->APB1ENR |= (RCC_APB1ENR_SPI2EN | RCC_APB1ENR_USART3EN);

    // PB11 / INT/HOLD
    GPIOB->MODER |= GPIO_MODER_MODER11_0;
	// open drain output
    GPIOB->OTYPER |= GPIO_OTYPER_OT_11;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11_1;

    // PD11 / Chip Select
    GPIOD->MODER |= GPIO_MODER_MODER11_0;
	// open drain output
    GPIOD->OTYPER |= GPIO_OTYPER_OT_11;
    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11_1;

    // PB13 / SCK
    GPIOB->MODER |= GPIO_MODER_MODER13_1;
	// open drain output
    GPIOB->OTYPER |= GPIO_OTYPER_OT_13;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13_1;
    GPIOB->AFR[1] |= (0x05 << 20);

    // PB14 / MISO
    GPIOB->MODER |= GPIO_MODER_MODER14_1;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR14_1;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR14_1;
    GPIOB->AFR[1] |= (0x05 << 24);

    // PB15 / MOSI
    GPIOB->MODER |= GPIO_MODER_MODER15_1;
	// open drain output
    GPIOB->OTYPER |= GPIO_OTYPER_OT_15;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15_1;
    GPIOB->AFR[1] |= (0x05 << 28);

    // PD12 / LED
    GPIOD->MODER |= GPIO_MODER_MODER12_0;
    GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12_1;

    // SPI
    SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_CPHA | SPI_CR1_MSTR | (SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0);
    SPI2->CR1 |= SPI_CR1_SPE;

    // PC10 / TX
    GPIOC->MODER |= GPIO_MODER_MODER10_1;
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10_0;
    GPIOC->AFR[1] |= (0x07 << 8);

    // PC11 / RX
    GPIOC->MODER |= GPIO_MODER_MODER11_1;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPDR11;
    GPIOC->AFR[1] |= (0x07 << 12);

    // USART
    USART3->BRR = 0x00D9; // 50.0 MHz / 0x00D9 = 115200
    USART3->CR3 |= USART_CR3_DMAT;
    USART3->CR1 |= (USART_CR1_RXNEIE | USART_CR1_TE | USART_CR1_RE);
    USART3->CR1 |= USART_CR1_UE;

    NVIC_SetPriority(USART3_IRQn, 15);
    NVIC_EnableIRQ(USART3_IRQn);

    __enable_irq();

    volatile uint32_t i;
    uint16_t data;

    uart_putc(spi(0b01001100));
    uart_putc(spi(0b01001100));
    uart_putc(spi(0b11100001));

    // SET_ADVANCED_MODE
    uart_putc(spi(0b01110001));

    for (;;) {
        /*data = uart_getc();

        if ((data & 0xFF00) == 0) {
            uart_putc(spi(data));
        }*/

    	// LED on PD12 goes LOW
        GPIOD->BSRRL = GPIO_ODR_ODR_12;
        // delay
        for (i = 0; i < 10000; i++);

        // BAND_PASS_CMD
        uart_putc(spi(0b00000000 | (40 & 0x3F)));
        // Set the gain
        uart_putc(spi(0b10000000 | (49 & 0x3F)));
        // Set the integration time constant
        uart_putc(spi(0b11000000 | (31 & 0x1F)));

        // SET_ADVANCED_MODE
        uart_putc(spi(0b01110001));


        // int/hold LOW
        GPIOB->BSRRL = GPIO_ODR_ODR_11;

    	// LED on PD12 goes HIGH
        GPIOD->BSRRH = GPIO_ODR_ODR_12;
        // delay
        for (i = 0; i < 10000; i++);

        // int/hold HIGH
        GPIOB->BSRRH = GPIO_ODR_ODR_11;
    }

    return 0;
}

uint16_t spi(uint16_t data)
{
    volatile uint16_t i;

    // Chip Select PD11 goes HIGH
    GPIOD->BSRRH = GPIO_ODR_ODR_11;

    for (i = 0; i < 10; i++);

    SPI2->DR = data;

    while (!(SPI2->SR & SPI_SR_TXE));
    while (!(SPI2->SR & SPI_SR_RXNE));
    while ((SPI2->SR & SPI_SR_BSY));

    for (i = 0; i < 10; i++);

    // Chip Select PD11 goes LOW
    GPIOD->BSRRL = GPIO_ODR_ODR_11;

    for (i = 0; i < 100; i++);

    return SPI2->DR;
}

void uart_putc(uint8_t c)
{
    uint16_t tmphead;

    tmphead = (tmphead < (TX_BUF_SIZE - 1)) ? (tx_head + 1) : 0;
    tx_buf[tmphead] = c;
    tx_head = tmphead;

    USART3->CR1 |= USART_CR1_TXEIE;
}

uint16_t uart_getc(void)
{
    uint16_t data, tmptail;

    if (rx_tail == rx_head)
    {
        data = NO_DATA;
    } 
    else 
    {
        tmptail = (tmptail < (RX_BUF_SIZE - 1)) ? (rx_tail + 1) : 0;
        rx_tail = tmptail;
        data = rx_buf[tmptail];
    }

    return data;
}

void USART3_IRQHandler(void)
{
    if ((USART3->SR & USART_SR_RXNE))
    {
        uint8_t data, tmphead;

        USART3->SR = ~USART_SR_RXNE;

        data = USART3->DR;
        tmphead = (rx_head < (RX_BUF_SIZE - 1)) ? (rx_head + 1) : 0;

        if (rx_tail == tmphead)
        {
            // Buffer overflow
        }
        else
        {
            rx_head = tmphead;
            rx_buf[tmphead] = data;
        }
    }

    if ((USART3->SR & USART_SR_TXE))
    {
        uint16_t tmptail;

        USART3->SR = ~USART_SR_TXE;

        if (tx_tail != tx_head)
        {
            tmptail = (tx_tail < (TX_BUF_SIZE - 1)) ? (tx_tail + 1) : 0;
            tx_tail = tmptail;
            USART3->DR = tx_buf[tmptail];
        } 
        else
        {
            USART3->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}

