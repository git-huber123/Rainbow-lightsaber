#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t spi_buf[16];
volatile uint32_t addr = 0;
volatile uint8_t last = 0;

void tx_spi(uint8_t len) {
    PORTB &= 0b11111110;
    for (uint8_t i=0; i < len; i++) {
        SPDR = spi_buf[i];
        while (!(SPSR & 0b10000000));
        spi_buf[i] = SPDR;
    }
    PORTB |= 0b00000001;
}

void delay_us(uint16_t us) {
    TCCR3B = 0b00000010;    // divide by 8
    OCR3A  = 2*us-1;             // 1us
    TCNT3  = 0;
    TIFR3 |= 0b00000010;
    while (!(TIFR3 & 0b00000010));
}

ISR(USART0_RX_vect) {
    uint8_t in_val = UDR0;
    // Write enable
    spi_buf[0] = 0x06;
    tx_spi(1);
    // Write
    spi_buf[0] = 0x02;
    spi_buf[1] = addr >> 16;
    spi_buf[2] = addr >> 8;
    spi_buf[3] = addr;
    spi_buf[4] = in_val;
    tx_spi(5);
    delay_us(20);
    spi_buf[0] = 0x03;
    spi_buf[1] = addr >> 16;
    spi_buf[2] = addr >> 8;
    spi_buf[3] = addr;
    spi_buf[4] = 0x00;
    tx_spi(5);
    UDR0 = spi_buf[4];
    addr++;
}

void delay_ms(uint16_t ms) {
    TCCR3B = 0b00000011;    // divide by 64
    OCR3A  = 249;           // 1ms
    TCNT3  = 0;
    TIFR3 |= 0b00000010;
    for (; ms>0; ms--) {
        while (!(TIFR3 & 0b00000010));
        TIFR3 |= 0b00000010;
    }
}

int main(void) {
    DDRB  |= 0b00000111;
    PORTB |= 0b00000001;
    
    UBRR0  = 16;             // 115200 UART connection to computer
    UCSR0A = 0b00000010;
    UCSR0B = 0b10011000;
    UCSR0C = 0b00000110;

    SPCR   = 0b01010000;
    SPSR   = 0b00000001;
    
    // misc. delay counter
    TCCR3A = 0b00000000;
    TCCR3B = 0b00001010;    // divide by 8, CTC mode
    TCCR3C = 0b00000000;
    
    
    // Read status register
    spi_buf[0] = 0x05;
    spi_buf[1] = 0x00;
    tx_spi(2);
    UDR0 = 'r';
    while (!(UCSR0A & 0b00100000));
    
    // Write enable
    spi_buf[0] = 0x06;
    tx_spi(1);
    UDR0 = 'u';
    while (!(UCSR0A & 0b00100000));
    
    // Write status register
    spi_buf[0] = 0x01;
    spi_buf[1] &= 0b00000011;
    tx_spi(2);
    UDR0 = 'w';
    while (!(UCSR0A & 0b00100000));
    
    for (uint8_t i=0; i < 16; i++) {
        // Write enable
        spi_buf[0] = 0x06;
        tx_spi(1);
        UDR0 = 'u';
        while (!(UCSR0A & 0b00100000));
        
        // Erase
        spi_buf[0] = 0xD8;
        spi_buf[1] = i;
        spi_buf[2] = 0x00;
        spi_buf[3] = 0x00;
        tx_spi(4);
        UDR0 = 'e';
        while (!(UCSR0A & 0b00100000));
        delay_ms(5);
    }
    
    sei();

    while (1);
    return 0;
}
