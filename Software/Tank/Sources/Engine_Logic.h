#ifndef SOURCES_ENGINE_LOGIC_H_
#define SOURCES_ENGINE_LOGIC_H_

#include "Globals.h"


void SetMotorSpeed(uint8_t speed)
{
    cyhal_pwm_set_duty_cycle(&pwm_MotorR, speed, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorL, speed, Frequentie);
}

void LeftMotor(uint8_t speed)
{
    cyhal_pwm_set_duty_cycle(&pwm_MotorR, speed, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorL, 0, Frequentie);
}

void RightMotor(uint8_t speed)
{
    cyhal_pwm_set_duty_cycle(&pwm_MotorL, speed, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorR, 0, Frequentie);
}

void InitMotorPins(void)
{
    cyhal_pwm_init(&pwm_MotorR, MotorR_Pin, NULL);
    cyhal_pwm_init(&pwm_MotorL, MotorL_Pin, NULL);
    cyhal_gpio_init(Forward_Pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, Pin_status);
    cyhal_gpio_init(Backward_Pin, CYHAL_GPIO_DIR_OUTPUT, CYHAL_GPIO_DRIVE_STRONG, !Pin_status);
}

void StartMotors()
{
	cyhal_pwm_start(&pwm_MotorR);
	cyhal_pwm_start(&pwm_MotorL);
}

void StopMotors()
{
    cyhal_pwm_set_duty_cycle(&pwm_MotorR, 0, Frequentie);
    cyhal_pwm_set_duty_cycle(&pwm_MotorL, 0, Frequentie);
    cyhal_gpio_write(MotorR_Pin, 0);
    cyhal_gpio_write(MotorL_Pin, 0);
}

void Motor_task(void *arg)
{
	while(1)
	{
		 if(b_data.forward == BUTTON_ON)
		 {
			 cyhal_gpio_write(Forward_Pin, 0);
			 cyhal_gpio_write(Backward_Pin, 1);
			 SetMotorSpeed(100);
		 }
		 else if(b_data.backwards == BUTTON_ON)
		 {
			 cyhal_gpio_write(Forward_Pin, 1);
			 cyhal_gpio_write(Backward_Pin,0);
			 SetMotorSpeed(100);
		 }
		 else if(b_data.right == BUTTON_ON)
		 {
			 RightMotor(100);
		 }
		 else if(b_data.left == BUTTON_ON)
		 {
			 LeftMotor(100);
		 }
		 else
		 {
			 StopMotors();
		 }

		vTaskDelay(10);
	}
}

#endif /* SOURCES_ENGINE_LOGIC_H_ */
