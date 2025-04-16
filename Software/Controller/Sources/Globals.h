#ifndef SOURCES_GLOBALS_H_
#define SOURCES_GLOBALS_H_
/*
 * Main global header.
 * Has to be included in every header file.
 * We're not using source files to make things easier for us.
 * Globals and Pins will be defined in here.
*/

// Includes
#include "cybsp_bt_config.h"
#include "wiced_bt_stack.h"
#include "cybsp.h"
#include "cyhal.h"
#include "cy_retarget_io.h"
#include "cycfg_bt_settings.h"
#include "cycfg_gap.h"
#include "app_bt_utils.h"
#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

// Task defines
#define TASK_STACK_SIZE 		(1024u)
#define	TASK_PRIORITY 			(5u)

// Pins
#define I2C_SCL_PIN 			P6_0
#define I2C_SDA_PIN				P6_1
#define JS_Button_PIN			P9_1
#define JS_Y_PIN				P10_0
#define JS_X_PIN				P10_2
#define BUTTON_RIGHT_PIN		P10_3
#define BUTTON_LEFT_PIN			P10_4
#define BUTTON_BACKWARDS_PIN	P10_5
#define BUTTON_FORWARD_PIN		P10_6

// UUID's of the service and characteristics we'll need.
#define __UUID_SERVICE_TANK_LT                     		{0x88, 0x29, 0x6D, 0xB5, 0xB4, 0xA6, 0x0A, 0xB2, 0x5B, 0x41, 0x16, 0xA2, 0xD8, 0xC2, 0x15, 0x95}
#define __UUID_CHARACTERISTIC_TANK_LT_MOTOR_CONTROL    	{0x50, 0xB1, 0x6B, 0x48, 0x56, 0xC0, 0x5A, 0x8F, 0xF5, 0x41, 0x0D, 0x12, 0x0E, 0xE4, 0xD5, 0xA5}
#define __UUID_CHARACTERISTIC_TANK_LT_LIGHTS        	{0x32, 0x58, 0x39, 0xB9, 0x04, 0xF2, 0x9A, 0xB2, 0xAB, 0x41, 0xA8, 0x95, 0x1E, 0x88, 0x5B, 0x44}

// Device Name
#define DEVICE_NAME "Tank_LT"

// A struct for all the button data.
typedef struct
{
	bool left;
	bool right;
	bool forward;
	bool backwards;
}Button_data;
Button_data b_data;

// A struct defined for all the joystick data.
typedef struct
{
    int32_t y;
    int32_t x;
    bool button;
} Joystick_data;
Joystick_data js_data;

// A struct defined for all the handles.
typedef struct
{
	uint16_t dclr_motor;
	uint16_t dclr_lights;
	uint16_t write_motor;
	uint16_t write_lights;
	uint16_t service_start;
	uint16_t service_end;
} Handles;
Handles handle;

// ADC
cyhal_adc_channel_t Joystick_Channel_adc_x;
cyhal_adc_channel_t Joystick_Channel_adc_y;
cyhal_adc_t Joystick_ADC;

// UUID
wiced_bt_uuid_t service_uuid = {
    .len = LEN_UUID_128,
    .uu.uuid128 = __UUID_SERVICE_TANK_LT
};

wiced_bt_uuid_t char_motor_uuid = {
    .len = LEN_UUID_128,
    .uu.uuid128 = __UUID_CHARACTERISTIC_TANK_LT_MOTOR_CONTROL
};

wiced_bt_uuid_t char_lights_uuid = {
    .len = LEN_UUID_128,
    .uu.uuid128 = __UUID_CHARACTERISTIC_TANK_LT_LIGHTS
};

// Connection ID
uint16_t connection_id = 0;

// Discovery globals
bool ServiceDiscoveryComplete = false;
uint8_t *uuid128;

// Write Parameters
wiced_bt_gatt_write_hdr_t write_params;

// Check task creation succes
void CheckTaskCreation(BaseType_t Task,char* Name)
{
	if (Task != pdPASS)
	    {
	        printf("%s creation failed\n",Name);
	    }
	    else
	    {
	    	printf("%s creation Succes\n",Name);
	    }
}

// Print the UUID
void print_uuid(uint8_t *uuid)
{
    printf("UUID: %02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X\n",
           uuid[15], uuid[14], uuid[13], uuid[12],
           uuid[11], uuid[10],
           uuid[9], uuid[8],
           uuid[7], uuid[6],
           uuid[5], uuid[4], uuid[3], uuid[2], uuid[1], uuid[0]);
}

#endif /* SOURCES_GLOBALS_H_ */
