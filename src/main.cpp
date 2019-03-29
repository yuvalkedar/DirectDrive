#include <Arduino.h>
#include "font_5x7.h"

#define MATRIX_WIDTH (5)
#define MATRIX_HEIGHT (8)
#define MAX_MATRIX_WIDTH (MATRIX_WIDTH - 1)

#define BITS_IN_BYTE (8)
#define SERIAL_BAUDRATE     (115200)

uint8_t matrix_buffer[MATRIX_WIDTH * MATRIX_HEIGHT / BITS_IN_BYTE] = {0};

void set_vertical_bitmask(uint8_t bitmask) {
    bitmask = ~bitmask;
    PORTB = (PORTB & 0xC3) | (bitmask << 2) & 0x3C;
    PORTC = (PORTC & 0xF0) | (bitmask >> 4) & 0x3F;
}

void set_horizental_index(uint8_t index) {
    uint8_t bitmask = _BV(MAX_MATRIX_WIDTH - index);
    PORTD = (PORTD & 0x1F) | (bitmask << 5) & 0xE0;
    PORTB = (PORTB & 0xFC) | (bitmask >> 3) & 0x03;
}

void scan_matrix() {
    for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
        set_horizental_index(x);
        set_vertical_bitmask(matrix_buffer[x]);
        delay(1);
        set_vertical_bitmask(0);
    }
}

void delay_with_scan(uint32_t ms) {
    ms += millis();
    while(ms > millis())
        scan_matrix();    
}

void clear_matrix() {
    memset(matrix_buffer, 0, sizeof(matrix_buffer));
}

void draw_vertical_bits(uint8_t x, uint8_t bitmask) {
    if (x > MAX_MATRIX_WIDTH)
        return;
    matrix_buffer[x] = bitmask;
}

void shift_matrix_left(uint8_t new_bitmask) {
    for (uint8_t x = 1; x < MATRIX_WIDTH; x++)
        matrix_buffer[x - 1] = matrix_buffer[x];
    matrix_buffer[MAX_MATRIX_WIDTH] = new_bitmask;
}

void print_char(uint8_t c, uint8_t x) {
	if ((c < FONT_FIRST_CHAR) || (c > (FONT_FIRST_CHAR + FONT_CHARS_COUNT)))
        return;

	uint16_t font_index = (c - FONT_FIRST_CHAR) * FONT_WIDTH;

	for (uint8_t i = 0; i < FONT_WIDTH; i++)
		draw_vertical_bits(x + i, FONT_READ_DATA(font_index + i));
}

uint8_t get_str_width(char *str) {
    return strlen(str) * FONT_WIDTH;
}

void scroll_str(char *str, uint32_t speed_ms) {
	// uint8_t str_w = get_str_width(str);
    char c;

    while(c = *str++){
        for (uint8_t x = 0; x < MATRIX_WIDTH; x++) {
            shift_matrix_left(0);
            print_char(c, MAX_MATRIX_WIDTH - x);
            delay_with_scan(speed_ms);
        }
        shift_matrix_left(0);
        delay_with_scan(speed_ms);
        shift_matrix_left(0);
        delay_with_scan(speed_ms);
    }
    for (uint8_t x = 0; x < 3; x++) {
        shift_matrix_left(0);
        delay_with_scan(speed_ms);
    }
}

void setup() {
	Serial.begin(SERIAL_BAUDRATE);
	Serial.println(F("\nstart!"));

    /* set pins 5-7 as OUTPUTs */
    DDRD |= 0xE0;

    /* set pins 8-13 as OUTPUTs */
    DDRB |= 0x3F;

    /* set pins A0-A3 as OUTPUTs */
    DDRC |= 0x0F;

    scan_matrix();
    // print_char('a', 0);
}

void loop() {
    delay_with_scan(100);
    scroll_str("banana", 200);
}
