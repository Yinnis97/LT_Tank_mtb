
#include "cybsp.h"
#include "cyhal.h"

#include "Globals.h"
#include "Engine_Logic.h"
#include "Bluetooth.h"

int main(void)
{
	cybsp_init() ;
    __enable_irq();

    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    InitMotorPins();
    StartMotors();

    printf("**********Tank*****************\n");

    cybt_platform_config_init(&cybsp_bt_platform_cfg);
    wiced_bt_stack_init (app_bt_management_callback, &wiced_bt_cfg_settings);

    CheckTaskCreation(xTaskCreate(Motor_task, "Motor Task", 1024u, NULL, TASK_PRIORITY, NULL),"Motor Task");
    vTaskStartScheduler() ;

    CY_ASSERT(0) ;
}


