
#include "cybsp.h"
#include "cyhal.h"

#include "Globals.h"
#include "Controller.h"
#include "Bluetooth.h"

int main(void)
{
    cybsp_init();
    __enable_irq();

    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    Initialize_buttons();
    Initialize_adc();

    printf("**********Tank Controller*****************\n");

    cybt_platform_config_init(&cybsp_bt_platform_cfg);
    wiced_bt_stack_init (app_bt_management_callback, &wiced_bt_cfg_settings);

    CheckTaskCreation(xTaskCreate(Controller_task, "Controller Task", 1024u, NULL, TASK_PRIORITY, NULL),"Controller Task");
    vTaskStartScheduler() ;

    CY_ASSERT(0) ;
}
