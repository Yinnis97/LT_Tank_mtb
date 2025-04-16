#ifndef SOURCES_CONTROLLER_H_
#define SOURCES_CONTROLLER_H_

#include "Globals.h"

void Controller_task(void *arg);
void Initialize_adc(void);
void Initialize_buttons(void);
void Testing(void);



// Controller Task
void Controller_task(void *arg)
{
	while(1)
	{
		while((connection_id != 0) && ServiceDiscoveryComplete && (handle.write_motor != 0))
		{
			b_data.left = cyhal_gpio_read(BUTTON_LEFT_PIN);
			b_data.right = cyhal_gpio_read(BUTTON_RIGHT_PIN);
			b_data.forward = cyhal_gpio_read(BUTTON_FORWARD_PIN);
			b_data.backwards = cyhal_gpio_read(BUTTON_BACKWARDS_PIN);

			uint8_t writeData[10];
			writeData[0] = (uint8_t)b_data.forward;
			writeData[1] = (uint8_t)b_data.backwards;
			writeData[2] = (uint8_t)b_data.right;
			writeData[3] = (uint8_t)b_data.left;

			// Setup Write Parameters
			write_params.len = sizeof(writeData);
			write_params.handle = handle.write_motor;
			write_params.offset = 0;
			write_params.auth_req = GATT_AUTH_REQ_NONE;

	    	wiced_bt_gatt_client_send_write(connection_id, GATT_CMD_WRITE, &write_params,  writeData, NULL);

			vTaskDelay(100);
		}
		vTaskDelay(500);
	}
}

// Initialize all buttons
void Initialize_buttons(void)
{
	cyhal_gpio_init(BUTTON_LEFT_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1u);
	cyhal_gpio_init(BUTTON_RIGHT_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1u);
	cyhal_gpio_init(BUTTON_FORWARD_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1u);
	cyhal_gpio_init(BUTTON_BACKWARDS_PIN, CYHAL_GPIO_DIR_INPUT, CYHAL_GPIO_DRIVE_PULLUP, 1u);
}

// Initialize ADC
void Initialize_adc()
{
	cyhal_adc_init(&Joystick_ADC, JS_X_PIN, NULL);

    const cyhal_adc_channel_config_t channel_config =
        { .enable_averaging = false, .min_acquisition_ns = 1000, .enabled = true };

    cyhal_adc_channel_init_diff(&Joystick_Channel_adc_x, &Joystick_ADC, JS_X_PIN, CYHAL_ADC_VNEG,
                                       &channel_config);
    cyhal_adc_channel_init_diff(&Joystick_Channel_adc_y, &Joystick_ADC, JS_Y_PIN, CYHAL_ADC_VNEG,
                                       &channel_config);

    printf("Initialize ADC Complete.\n\r");
}

// Testing function
void Testing()
{
	//	js_data.x = cyhal_adc_read_uv(&Joystick_Channel_adc_x);
	//	js_data.y = cyhal_adc_read_uv(&Joystick_Channel_adc_y);
	//
	//	uint8_t writeData[10];
	//	writeData[0] = (uint8_t)(js_data.y >> 24) & 0xFF;
	//	writeData[1] = (uint8_t)(js_data.y >> 16) & 0xFF;
	//	writeData[2] = (uint8_t)(js_data.y >> 8) & 0xFF;
	//	writeData[3] = (uint8_t)(js_data.y & 0xFF);
	//	writeData[4] = (uint8_t)(js_data.x >> 24) & 0xFF;
	//	writeData[5] = (uint8_t)(js_data.x >> 16) & 0xFF;
	//	writeData[6] = (uint8_t)(js_data.x >> 8) & 0xFF;
	//	writeData[7] = (uint8_t)(js_data.x & 0xFF);
	//	writeData[8] = (uint8_t)js_data.button;
	//}

#endif /* SOURCES_CONTROLLER_H_ */
