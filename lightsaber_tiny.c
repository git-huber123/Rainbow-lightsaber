#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#define CS_PORT PORTB
#define CS_DDR DDRB
#define CS_FLASH 0b00001000
#define CS_DAC 0b00010000

// Commands to the audio controller
//  * 0000 0000 dddd dddd               Set buzz volume
//  * 0001 0000 dddd dddd               Set playback volume
//  * 0010 0000 dddd dddd               Set audio timer divider
//  * 0011 dddd dddd dddd               Set buzz period
//  * 0100 dddd dddd dddd               WAV segment low bits
//  * 0101 dddd dddd dddd               Play WAV segment (high bits)
//  * 0110 0000 0000 0000               Stop current WAV segment

#define BUZZ_CENTER 174
const uint8_t unit[833] PROGMEM = {
    128, 128, 129, 130, 132, 134, 137, 141, 144, 148, 152, 156, 160,
    165, 169, 174, 179, 183, 187, 192, 196, 199, 203, 206, 209, 212,
    214, 216, 218, 219, 220, 220, 220, 219, 218, 217, 215, 213, 210,
    207, 203, 199, 195, 191, 187, 183, 179, 176, 173, 170, 167, 165,
    162, 160, 158, 156, 154, 153, 151, 149, 148, 147, 146, 144, 143,
    142, 141, 140, 140, 139, 138, 137, 137, 136, 136, 135, 135, 134,
    134, 133, 133, 133, 132, 132, 132, 131, 131, 131, 131, 131, 130,
    130, 130, 130, 130, 130, 129, 129, 129, 129, 129, 129, 129, 129,
    129, 129, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128,
    128};

volatile uint16_t   idx = 0;
volatile uint8_t    amp = 100;
volatile uint8_t    amp_flash = 100;

volatile uint32_t   addr = 1;
volatile uint32_t   end_addr;
volatile int16_t    next_sample = 0;
volatile int16_t    next_out = 0;

// Variables for buffering data from the flash chip
         uint8_t    data[64];
volatile uint8_t    num_bytes;
volatile uint8_t    data_idx;
volatile uint8_t    flash_mode = 0;

#define FLASH_ENABLE    0b00000001
#define FLASH_AUDIO     0b00000010

// Parsed WAV data
         uint32_t   data_len=0;
         uint32_t   fmt_len=0;
         uint32_t   riff_len=0;
         uint32_t   sample_rate=0;
         uint8_t    bytes_per_sample=0;
         uint32_t   data_offsets[4];
         uint32_t   data_lengths[4];
         uint8_t    data_chunks=0;

// Audio playback
volatile uint32_t   curr_sample = 0;
volatile uint8_t    curr_chunk = 0;
volatile char       recv = 0;
volatile uint8_t    finished = 0;

// Control
volatile uint8_t    start_signal = 0;
volatile uint16_t   control_data = 0;
         uint16_t   addr_lowbits = 0;

void read_bytes_blocking(uint32_t offset, uint8_t num) {
    // Wait for transaction to finish
    while (flash_mode & FLASH_ENABLE);
    // Load address and size
    addr = offset;
    data_idx = 0;
    num_bytes = num;
    flash_mode = FLASH_ENABLE;
    while (flash_mode & FLASH_ENABLE);
}

uint32_t next_long(uint8_t offset) {
    return *((uint32_t*)(data+offset));
}

void prepare_audio(uint32_t offset) { 
    read_bytes_blocking(offset, 36);
    
    sample_rate = next_long(24);
    riff_len = next_long(4);
    fmt_len = next_long(16);
    
    bytes_per_sample = (data[34]>>3)+(data[35]<<5);
    
    
    // Prepare for audio playback
    uint32_t x=36;
    data_chunks = 0;
    while (x < riff_len + 8) {
        read_bytes_blocking(offset+x, 8);
        data_lengths[data_chunks] = next_long(4);
        if (memcmp(data, "data", 4)==0) {
            data_offsets[data_chunks] = offset + x + 8;
            x = x + 8 + data_lengths[data_chunks];
            data_chunks++;
        } else x = x+8+data_lengths[data_chunks];
    }
}

void play_audio() {
    while (flash_mode & FLASH_ENABLE);
    curr_chunk = 0;
    curr_sample = 0;
    finished = 0;
    addr = data_offsets[0];
    //addr = 0x3002C;
    end_addr = addr + data_lengths[0];
    if (bytes_per_sample > 2) addr += (bytes_per_sample - 2);
    flash_mode |= FLASH_ENABLE | FLASH_AUDIO;
}


ISR(TIM1_COMPA_vect, ISR_NAKED) {
    asm (
        "push r22\n\t"
        "in r22, 0x3f\n\t"
        "push r22\n\t"
        "push r23\n\t"
        "push r24\n\t"
        "push r25\n\t"
        "push r28\n\t"
        "push r29\n\t"
        "push r30\n\t"
        "push r31\n\t"
        "cbi 0x18, 4\n\t"
        
        "ldi r30, lo8(unit)\n\t"
        "ldi r31, hi8(unit)\n\t"
        "lds r24, idx\n\t"
        "lds r25, idx+1\n\t"
        "add r30, r24\n\t"
        "adc r31, r25\n\t"
        "lpm r23, Z\n\t"
        "adiw r24, 1\n\t"
        "ldi r30, hi8(833)\n\t"
        "cpi r24, lo8(833)\n\t"
        "cpc r25, r30\n\t"
        "brne store_next_addr\n\t"
                "clr r24\n\t"
                "clr r25\n\t"
        "store_next_addr:\n\t"
        "sts idx, r24\n\t"
        "sts idx+1, r25\n\t"
        
        "in r22, 0x16\n\t"
        "sts start_signal, r22\n\t"
        
        "lds r24, next_out+1\n\t"
        "out 0x0F, r24\n\t"
        "ldi r24, 0b01000000\n\t"
        "out 0x0E, r24\n\t"
        "ldi r24, 0b00010011\n\t"
        "ldi r25, 0b00010001\n\t"
        "out 0x0D, r25\n\t"
        
        "subi r23, lo8(174)\n\t"    // BUZZ_CENTER
        "lds r22, amp\n\t"
        "clr r28\n\t"
        "clr r29\n\t"
        // At this point, r23 contains the buzz value and r22 contains the buzz amplitude
        // r28 and r29 will contain the product and have been cleared
        "mov __tmp_reg__, r23\n\t"
        "sbrc r23, 7\n\t"           // absolute value of r23
        "neg r23\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "asr r23\n\t"
        "sbrc r22, 0\n\t"
        "add r29, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r29\n\t"
        "ror r28\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 1\n\t"
        "add r29, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r29\n\t"
        "ror r28\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 2\n\t"
        "add r29, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r29\n\t"
        "ror r28\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 3\n\t"
        "add r29, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r29\n\t"
        "ror r28\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 4\n\t"
        "add r29, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r29\n\t"
        "ror r28\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 5\n\t"
        "add r29, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r29\n\t"
        "ror r28\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 6\n\t"
        "add r29, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r29\n\t"
        "ror r28\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 7\n\t"
        "add r29, r23\n\t"
        "out 0x0D, r24\n\t"
        "sbrc __tmp_reg__, 0\n\t"
        "add r28, r22\n\t"
        "sbrc __tmp_reg__, 0\n\t"
        "adc r29, __zero_reg__\n\t"
        
        "sbrc __tmp_reg__, 7\n\t"
        "com r28\n\t"
        "sbrc __tmp_reg__, 7\n\t"
        "com r29\n\t"
        "sbrc __tmp_reg__, 7\n\t"
        "adiw r28, 1\n\t"
        
        "in r24, 0x0F\n\t"
        "sts control_data+1, r24\n\t"
        
        // Second multiplication, with flash value in r23 and flash amplitude in r22
        // Result is stored in r30 and r31
        "lds r23, next_sample+1\n\t"
        "lds r22, amp_flash\n\t"
        "mov __tmp_reg__, r23\n\t"
        "sbrc r23, 7\n\t"           // absolute value of r23
        "neg r23\n\t"
        "clr r31\n\t"
        
        "lds r24, next_out\n\t"
        "out 0x0F, r24\n\t"
        "ldi r24, 0b01000000\n\t"
        "out 0x0E, r24\n\t"
        "ldi r24, 0b00010011\n\t"
        "ldi r25, 0b00010001\n\t"
        "asr r23\n\t"
        "out 0x0D, r25\n\t"
        "sbrc r22, 0\n\t"
        "add r31, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r31\n\t"
        "ror r30\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 1\n\t"
        "add r31, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r31\n\t"
        "ror r30\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 2\n\t"
        "add r31, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r31\n\t"
        "ror r30\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 3\n\t"
        "add r31, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r31\n\t"
        "ror r30\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 4\n\t"
        "add r31, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r31\n\t"
        "ror r30\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 5\n\t"
        "add r31, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r31\n\t"
        "ror r30\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 6\n\t"
        "add r31, r23\n\t"
        "out 0x0D, r24\n\t"
        "asr r31\n\t"
        "ror r30\n\t"
        
        "out 0x0D, r25\n\t"
        "sbrc r22, 7\n\t"
        "add r31, r23\n\t"
        "out 0x0D, r24\n\t"
        "sbrc __tmp_reg__, 0\n\t"
        "add r30, r22\n\t"
        "sbrc __tmp_reg__, 0\n\t"
        "adc r31, __zero_reg__\n\t"
        
        "tst __tmp_reg__\n\t"       // restore sign to result
        "brpl result_positive_2\n\t"
        "com r30\n\t"
        "com r31\n\t"
        "adiw r30, 1\n\t"
        "result_positive_2:\n\t"
        
        "in r22, 0x0F\n\t"
        "sts control_data, r22\n\t"
        
        // Combine buzz and flash data
        // Flash data must be shifted by 4, buzz data by 3
        //"lds r30, next_sample\n\t"
        //"lds r31, next_sample+1\n\t"
        "asr r31\n\t"
        "ror r30\n\t"
        "add r30, r28\n\t"
        "adc r31, r29\n\t"
        
        
        "asr r31\n\t"
        "ror r30\n\t"
        "asr r31\n\t"
        "ror r30\n\t"
        "asr r31\n\t"
        "ror r30\n\t"
        "subi r31, hi8(-2048)\n\t"
        //"ldi r30, 0x00\n\t"
        //"ldi r31, 0x08\n\t"
        "andi r31, 0x0f\n\t"
        "ori r31, 0x30\n\t"
        "sts next_out, r30\n\t"
        "sts next_out+1, r31\n\t"
        
        
        "sbi 0x18, 4\n\t"
        
        "lds r22, flash_mode\n\t"
        "sbrs r22, 0\n\t"
        "rjmp no_flash\n\t"
        
        "cbi 0x18, 3\n\t"
        
        "ldi r22, 0x03\n\t"         // Transmit 0x03 (read)
        "out 0x0F, r22\n\t"
        "ldi r22, 0b01000000\n\t"
        "out 0x0E, r22\n\t"
        "ldi r24, 0b00010011\n\t"
        "ldi r25, 0b00010001\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        
        "lds r30, addr+2\n\t"                   // Transmit address byte 2
        "out 0x0F, r30\n\t"
        "ldi r22, 0b01000000\n\t"
        "out 0x0E, r22\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        
        "lds r29, addr+1\n\t"                   // Transmit address byte 1
        "out 0x0F, r29\n\t"
        "ldi r22, 0b01000000\n\t"
        "out 0x0E, r22\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        
        "lds r28, addr\n\t"                     // Transmit address byte 0
        "out 0x0F, r28\n\t"
        "ldi r22, 0b01000000\n\t"
        "out 0x0E, r22\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        "out 0x0D, r25\n\t"
        "out 0x0D, r24\n\t"
        // r28:r30 now contains the full address
        
        "lds r22, flash_mode\n\t"               // if (flash_mode & 0b10) {
        "sbrs r22, 1\n\t"
        "rjmp no_audio\n\t"
            "lds r22, bytes_per_sample\n\t"
            "add r28, r22\n\t"
            "clr r22\n\t"
            "adc r29, r22\n\t"
            "adc r30, r22\n\t"
            "sts addr, r28\n\t"
            "sts addr+1, r29\n\t"
            "sts addr+2, r30\n\t"
            
            "lds r22, end_addr\n\t"             // if (addr >= end_addr) {
            "lds r23, end_addr+1\n\t"
            "lds r24, end_addr+2\n\t"
            "cp r28, r22\n\t"
            "cpc r29, r23\n\t"
            "cpc r30, r24\n\t"
            "brlo continue_audio\n\t"
                "clr r22\n\t"
                "sts flash_mode, r22\n\t"
                "sts next_sample, r22\n\t"
                "sts next_sample+1, r22\n\t"
            "continue_audio:\n\t"               // }
            
            //"clr r22\n\t"                       // Read in data
            "lds r22, addr+1\n\t"
            "out 0x0F, r22\n\t"
            "ldi r22, 0b01000000\n\t"
            "out 0x0E, r22\n\t"
            "ldi r25, 0b00010001\n\t"
            "out 0x0D, r25\n\t"
            "ldi r24, 0b00010011\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "nop\n\t"
            "in r22, 0x0F\n\t"
            "sts next_sample, r22\n\t"
            
            //"clr r22\n\t"                       // Read in data
            "lds r22, addr+2\n\t"
            "out 0x0F, r22\n\t"
            "ldi r22, 0b01000000\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "nop\n\t"
            "in r22, 0x0F\n\t"
            "sts next_sample+1, r22\n\t"
            
        "rjmp no_flash\n\t"                     // } else {
        "no_audio:\n\t"
            "adiw r28, 1\n\t"                       // addr++;
            "clr r22\n\t"
            "adc r30, r22\n\t"
            "sts addr, r28\n\t"
            "sts addr+1, r29\n\t"
            "sts addr+2, r30\n\t"
            "out 0x0F, r22\n\t"
            "ldi r22, 0b01000000\n\t"
            "out 0x0E, r22\n\t"
            "ldi r25, 0b00010001\n\t"
            "out 0x0D, r25\n\t"
            "ldi r24, 0b00010011\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "out 0x0D, r25\n\t"
            "nop\n\t"
            "out 0x0D, r24\n\t"
            "nop\n\t"
            "lds r22, data_idx\n\t"
            "ldi r30, lo8(data)\n\t"
            "ldi r31, hi8(data)\n\t"
            "add r30, r22\n\t"
            "clr r23\n\t"
            "adc r31, r23\n\t"
            "in r23, 0x0F\n\t"
            "st Z, r23\n\t"
            "inc r22\n\t"
            "lds r23, num_bytes\n\t"
            "cp r22, r23\n\t"
            "brlo continue_noaudio\n\t"
                "clr r22\n\t"
                "sts flash_mode, r22\n\t"
            "continue_noaudio:\n\t"
            "sts data_idx, r22\n\t"
            
            "sbi 0x18, 3\n\t"
            "ldi r22, 0b01000000\n\t"
            "out 0x0D, r25\n\t"
            "out 0x0D, r24\n\t"
            "out 0x0D, r25\n\t"
            "out 0x0D, r24\n\t"
            "out 0x0D, r25\n\t"
            "out 0x0D, r24\n\t"
            "out 0x0D, r25\n\t"
            "out 0x0D, r24\n\t"
            "out 0x0D, r25\n\t"
            "out 0x0D, r24\n\t"
            "out 0x0D, r25\n\t"
            "out 0x0D, r24\n\t"
            "out 0x0D, r25\n\t"
            "out 0x0D, r24\n\t"
            "out 0x0D, r25\n\t"
            "out 0x0D, r24\n\t"
        "no_flash:\n\t"                         // }
        
        "sbi 0x18, 3\n\t"
        "pop r31\n\t"
        "pop r30\n\t"
        "pop r29\n\t"
        "pop r28\n\t"
        "pop r25\n\t"
        "pop r24\n\t"
        "pop r23\n\t"
        "pop r22\n\t"
        "out 0x3f, r22\n\t"
        "pop r22\n\t"
        "reti\n\t"
    );
}

int main() {
    PLLCSR &= 0b11111011;   // Disable PCK
    TCCR1  = 0b10000100;    // divide CK by 8
    OCR1A  = 59;
    OCR1C  = 59;            // Frequency of 16000000/8/60
    
    DDRB   = 0b00011110;
    PORTB  = 0b00011000;

    USICR  = 0b00010000;
    TCCR0A = 0b00010010;
    TCCR0B = 0b00000001;
    OCR0A  = 1;
    OCR0B  = 0;
    
    TIMSK |= 0b01000000;
    
    sei();
    
    
    prepare_audio(0x000000);
    play_audio();
    
    while (1) {
        if (start_signal & 0b01) {
            switch (control_data >> 12) {
                case 0b0000:
                    amp = control_data & 0xff;
                    break;
                case 0b0001:
                    amp_flash = control_data & 0xff;
                    break;
                case 0b0100:
                    addr_lowbits = control_data & 0x0fff;
                    break;
                case 0b0101:
                    prepare_audio((((uint32_t)(control_data & 0x0fff))<<12) | addr_lowbits);
                    play_audio();
                    break;
                default:
                    break;
            }
            start_signal = 0;
        }
    }
}