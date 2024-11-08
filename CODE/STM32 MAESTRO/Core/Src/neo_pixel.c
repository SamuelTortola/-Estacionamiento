

#include "neo_pixel.h"

// Function to reset all LEDs
void reset_all_leds(neopixel_led* leds, uint16_t number_leds){

	for(int i = 0; i < number_leds; i++){

		for(int j = 0; j < 8; j++){

			(leds + i) ->r[j] = LED_LOGICAL_ZERO;
			(leds + i) ->g[j] = LED_LOGICAL_ZERO;
			(leds + i) ->b[j] = LED_LOGICAL_ZERO;
		}
	}
}


// Function to set white color to all LEDs
void set_all_leds(neopixel_led* leds, uint16_t number_leds){

	for(int i = 0; i < number_leds; i++){

		for(int j = 0; j < 8; j++){

			(leds + i) ->r[j] = LED_LOGICAL_ONE;
			(leds + i) ->g[j] = LED_LOGICAL_ONE;
			(leds + i) ->b[j] = LED_LOGICAL_ONE;
		}
	}
}


// Function to set specific color to a particular LED
void set_specific_led(neopixel_led* leds, uint16_t led_position, rgb_color* color){

	// 7 - j is neccesary to reverse the order of the bits
	for(int j = 0; j < 8; j++){

		if(color->r & (0x1 <<j)){
			(leds + led_position) ->r[7 - j] = LED_LOGICAL_ONE;
		} else{
			(leds + led_position) ->r[7 - j] = LED_LOGICAL_ZERO;
		}

		if(color->g & (0x01 <<j)){
			(leds + led_position) ->g[7 - j] = LED_LOGICAL_ONE;
		} else{
			(leds + led_position) ->g[7 - j] = LED_LOGICAL_ZERO;
		}

		if(color->b & (0x01 <<j)){
			(leds + led_position) ->b[7 - j] = LED_LOGICAL_ONE;
		} else{
			(leds + led_position) ->b[7 - j] = LED_LOGICAL_ZERO;
		}
	}
}

// Function to set a particular pattern to LEDs
void set_pattern_led(neopixel_led* leds, rgb_color *pattern, uint16_t number_leds){

	for(int i = 0; i < number_leds; i++){
		set_specific_led(leds, i, &pattern[i]);
	}
}

