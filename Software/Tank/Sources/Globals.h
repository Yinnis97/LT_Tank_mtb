#ifndef SOURCES_GLOBALS_H_
#define SOURCES_GLOBALS_H_

#include <stdio.h>
#include <stdint.h>
#include "cybsp.h"
#include "cyhal.h"
#include "cycfg_bt_settings.h"
#include "cycfg_gap.h"
#include "cycfg_gatt_db.h"
#include "cycfg_bt.timestamp"
#include "cybsp_bt_config.h"
#include "cy_retarget_io.h"
#include "stdlib.h"
#include <FreeRTOS.h>
#include <task.h>
#include "wiced_bt_stack.h"
#include "app_bt_utils.h"

// Pins
#define MotorR_Pin 					(P9_1)
#define MotorL_Pin 					(P9_7)
#define Forward_Pin					(P9_4)
#define Backward_Pin				(P9_6)

// Globals Defines
#define Frequentie					(10u)
#define TASK_STACK_SIZE				(4096u)
#define	TASK_PRIORITY 				(5u)

// Motors
cyhal_pwm_t		pwm_MotorR;
cyhal_pwm_t 	pwm_MotorL;
bool			Pin_status = 0;

// Enum for button status
enum
{
	BUTTON_ON = 0,
	BUTTON_OFF
};

// A struct for all the button data.
typedef struct
{
	bool left;
	bool right;
	bool forward;
	bool backwards;
}Button_data;
Button_data b_data;

// A struct for all the joystick data.
typedef struct
{
    uint32_t y;
    uint32_t x;
    bool button;
} Joystick_data;
Joystick_data js_data;

// A struct for all the light data.
typedef struct
{
	uint8_t top;
	uint8_t front;
	uint8_t back;
}Lights_data;
Lights_data ls_data;

// Bluetooth
uint16_t connection_id = 0;
uint8_t* remote_bda;

// Check task creation status
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
#endif /* SOURCES_GLOBALS_H_ */
