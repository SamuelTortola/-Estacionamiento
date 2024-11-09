
#ifndef INC_NEO_PIXEL_H_
#define INC_NEO_PIXEL_H_
#include <stdint.h>
#include <stdio.h>

// Structure to store an RGB color
typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
}rgb_color;

// Structure for one NeoPixel LED
typedef struct
{
	uint16_t g[8];
	uint16_t r[8];
	uint16_t b[8];
}neopixel_led;

/**************************************************************************
 * 1.25 us - Period
 * Logical 1 - 0.8 us HIGH
 * Logical 0 - 0.4 us HIGH
 * System Frequency = 80 MHz, Prescaler = 3 -> Timer Frequency = 20 MHz
 * Auto-Reload Value = 24 -> PWM Frequency = 800 kHz
 * CCR = 15 -> 0.8 us HIGH
 * CCR = 7 -> 0.4 us HIGH
 * Have to be modified based on the Auto Reload Value
 *************************************************************************/

#define LED_LOGICAL_ONE  15
#define LED_LOGICAL_ZERO 7

void reset_all_leds(neopixel_led* leds, uint16_t number_leds);
void set_all_leds(neopixel_led* leds, uint16_t number_leds);
void set_specific_led(neopixel_led* leds, uint16_t led_position, rgb_color* color);
void set_pattern_led(neopixel_led* leds, rgb_color *pattern, uint16_t number_leds);


#endif /* INC_NEO_PIXEL_H_ */
