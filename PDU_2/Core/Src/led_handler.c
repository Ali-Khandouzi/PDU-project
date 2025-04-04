#include "led_handler.h"
#include "main.h"

// LED Blink Function
void LEDBlink(INT32U blink_frequency_ms) {
	INT32U start_time = OSTimeGet();
	INT32U current_time;
	INT32U total_duration_ms = 5000; // Blink for 5 seconds

	while (1) {
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		OSTimeDlyHMSM(0, 0, 0, blink_frequency_ms);

		current_time = OSTimeGet();
		if ((current_time - start_time) >= total_duration_ms) {
			break; // Exit loop if total duration has elapsed
		}
	}
	// Ensure the LED is off after blinking completes
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // Turn LED off
}
