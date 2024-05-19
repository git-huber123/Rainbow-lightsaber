#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>


         char       txbuf[64];
volatile uint8_t    txbuflen;
volatile uint8_t    txidx;

         uint8_t    i2c_buf[8];

         uint8_t    amp_buzz = 100;
         uint8_t    amp_flash = 100;
         
         uint16_t   average = 0;

void printdec(uint32_t num, uint8_t digits, uint8_t offset) {
    for (uint8_t x=0; x<digits; x++) {
        txbuf[offset+digits-1-x] = '0' + (num % 10);
        num = num / 10;
    }
}

uint16_t isqrt(uint32_t x) {
    uint32_t d = 1UL<<30;
    uint32_t c = 0;
    while (d > x) d = d >> 2;
    while (d != 0) {
        if (x >= c+d) {
            x -= c+d;
            c = (c >> 1) + d;
        } else c = c >> 1;
        d = d >> 2;
    }
    return (uint16_t)c;
}

/*void communicate(uint16_t data) {
    asm(
        "in r16, 0x3f\n\t"
        "push r16\n\t"
        
        "wait_cs_low:\n\t"
        "sbic 0x03, 1\n\t"
        "rjmp wait_cs_low\n\t"
        
        "wait_cs_high:\n\t"
        "sbis 0x03, 1\n\t"
        "rjmp wait_cs_high\n\t"
        
        
        "check_cs:\n\t"
        "sbic 0x03, 1\n\t"
        "rjmp check_cs\n\t"
        //"sbi 0x0A, 7\n\t"
        "in r16, 0x0B\n\t"
        "ori r16, 0b10000000\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "bst %B0, 7\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "nop\n\t"
        "in r20, 0x03\n\t"
        "in r21, 0x03\n\t"
        "in r22, 0x03\n\t"
        "sbrs r20, 0\n\t"
        "rjmp .+0\n\t"
        "sbrs r21, 0\n\t"
        "rjmp .+0\n\t"
        "sbrs r22, 0\n\t"
        "rjmp .+0\n\t"
        
        "nop\n\t"
        "bst %B0, 6\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %B0, 5\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %B0, 4\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %B0, 3\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %B0, 2\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %B0, 1\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %B0, 0\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        
        "nop\n\t"
        "bst %A0, 7\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %A0, 6\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %A0, 5\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %A0, 4\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %A0, 3\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %A0, 2\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %A0, 1\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\t"
        "nop\n\t"
        "bst %A0, 0\n\t"
        "bld r16, 7\n\t"
        "out 0x0B, r16\n\t"
        "nop\n\tnop\n\tnop\n\tnop\n\t"
        "andi r16, 0b01111111\n\t"
        "out 0x0B, r16\n\t"
        //"cbi 0x0A, 7\n\t"
        
        "pop r16\n\t"
        "out 0x3f, r16\n\t"
        :: "w" (data)
        : "r16", "r17", "r18", "r19", "r20", "r21", "r22", "r23");
}*/

void communicate(uint16_t data) {
    uint8_t sreg = SREG;
    cli();
    while (PINB & 0b00000100);
    while (!(PINB & 0b00000100));
    SPCR = 0b01000000;
    while (PINB & 0b00000100);
    SPDR = 0xf5;
    asm ("nop\n\tnop\n\tnop\n\tnop\n\t"
         "nop\n\tnop\n\tnop\n\tnop\n\t"
         "nop\n\tnop\n\tnop\n\tnop\n\t"
         "nop\n\tnop\n\tnop\n\tnop\n\t");
    SPDR = data>>8;
    SPSR |= 0b10000000;
    while (!(SPSR & 0b10000000));
    SPDR = data;
    SPSR |= 0b10000000;
    while (!(SPSR & 0b10000000));
    SPCR = 0b00000000;
    SREG = sreg;
}

void start_at_addr(uint8_t addr) {
    TWCR = 0b10100100;
    while (!(TWCR & 0b10000000));
    TWDR = (0x6A<<1);
    TWCR = 0b10000100;
    while (!(TWCR & 0b10000000));
    if (TWSR == 0x20) {
        // NACK received, transmit stop signal
        UDR0 = 'n';
        TWCR = 0b10010100;
        return;
    }
    // Write address
    TWDR = addr;
    TWCR = 0b10000100;
    while (!(TWCR & 0b10000000));
}

void write_bytes(volatile uint8_t *src, uint8_t n_bytes) {
    for (uint8_t i=0; i < n_bytes; i++) {
        TWDR = src[i];
        TWCR = 0b10000100;
        while (!(TWCR & 0b10000000));
    }
}

void read_bytes(volatile uint8_t *dest, uint8_t n_bytes) {
    // Repeated start
    TWCR = 0b10100100;
    while (!(TWCR & 0b10000000));
    TWDR = (0x6A<<1) | 1;
    TWCR = 0b10000100;
    while (!(TWCR & 0b10000000));
    if (TWSR == 0x48) {
        // NACK received, transmit stop signal
        UDR0 = 'n';
        TWCR = 0b10010100;
        return;
    }
    // NACK
    uint8_t i=0;
    do {
        i++;
        TWCR = (i==n_bytes ? 0b10000100 : 0b11000100);
        while (!(TWCR & 0b10000000));
        dest[i-1] = TWDR;
    } while (i < n_bytes);
}

ISR(USART_UDRE_vect) {
    if (txidx < txbuflen) {
        UDR0 = txbuf[txidx];
        txidx++;
    } else {
        txbuflen = 0;
        UCSR0B &= 0b11011111;
    }
}

ISR(TIMER2_COMPA_vect) {
    start_at_addr(0x22);
    read_bytes(i2c_buf, 4);
    int16_t x = i2c_buf[0] | (i2c_buf[1] << 8);
    int16_t y = i2c_buf[2] | (i2c_buf[3] << 8);
    //int32_t z = ((int16_t)read16(0xA6));
    printdec((uint16_t)x, 5, 7);
    printdec((uint16_t)y, 5, 21);
    //printdec(z, 5, 35);
    int32_t mag = (int32_t)x*(int32_t)x + (int32_t)y*(int32_t)y;
    printdec(isqrt(mag), 5, 46);
    txbuflen = 53;
    txidx = 0;
    UCSR0B |= 0b00100000;
    average = ((uint32_t)average * 200 + (uint32_t)isqrt(mag)*56)>>8;
    if (average > (150<<5)) average = 150<<5;
    amp_buzz = 50 + (average>>5);
    communicate(amp_buzz);
}

int main() {
    DDRD  |= 0b10000000;
    DDRB  |= 0b00010000;
    PORTB |= 0b00000100;
    
    UBRR0  = 3;             // 500K UART connection to computer
    UCSR0A = 0b00000010;
    UCSR0B = 0b00011000;
    UCSR0C = 0b00000110;
    
    TCCR2A = 0b00000010;
    TCCR2B = 0b00000111;
    OCR2A  = 155;
    TIMSK2 = 0b00000010;
    
    // I2C bitrate is 100kHz
    TWBR   = 72;
    TWSR   = 0b00000000;
    
    SPCR   = 0b01000000;
    
    
    i2c_buf[0] = 0x48;
    i2c_buf[1] = 0x4C;
    start_at_addr(0x10); write_bytes(i2c_buf, 2);
    strcpy(txbuf, "acc x: -----, acc y: -----, acc z: -----, mag -----\r\n");
    txbuflen = 53;
    
    sei();
    
    while (1) {
        if (UCSR0A & 0b10000000) {
            uint8_t t = UDR0;
            if (t == '+') {
                amp_buzz++;
                communicate(amp_buzz);
                txbuf[0] = 'b';
                printdec(amp_buzz, 3, 1);
                txbuf[4] = '\r';
                txbuf[5] = '\n';
                txbuflen = 6;
                txidx = 0;
                UCSR0B |= 0b00100000;
            }
            if (t == '-') {
                amp_buzz--;
                communicate(amp_buzz);
                txbuf[0] = 'b';
                printdec(amp_buzz, 3, 1);
                txbuf[4] = '\r';
                txbuf[5] = '\n';
                txbuflen = 6;
                txidx = 0;
                UCSR0B |= 0b00100000;
            }
            if (t == 't') {
                communicate(0xa5a5);
            }
            if (t == 'p') {
                communicate(0x4000);
                communicate(0x5000);
            }
            if (t == '[') {
                amp_flash--;
                communicate(0x1000 | amp_flash);
                txbuf[0] = 'f';
                printdec(amp_flash, 3, 1);
                txbuf[4] = '\r';
                txbuf[5] = '\n';
                txbuflen = 6;
                txidx = 0;
                UCSR0B |= 0b00100000;
            }
            if (t == ']') {
                amp_flash++;
                communicate(0x1000 | amp_flash);
                txbuf[0] = 'f';
                printdec(amp_flash, 3, 1);
                txbuf[4] = '\r';
                txbuf[5] = '\n';
                txbuflen = 6;
                txidx = 0;
                UCSR0B |= 0b00100000;
            }
            if (('1' <= t) && (t <= '9')) {
                communicate(0x4000);
                communicate(0x5030 + ((t-'1')<<4));
            }
        }
    }
}
