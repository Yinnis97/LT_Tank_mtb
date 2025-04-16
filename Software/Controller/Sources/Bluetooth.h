#ifndef SOURCES_BLUETOOTH_H_
#define SOURCES_BLUETOOTH_H_

#include "Globals.h"


static wiced_bt_dev_status_t  app_bt_management_callback            (wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);
static wiced_bt_gatt_status_t app_bt_gatt_event_callback            (wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data);
static wiced_bt_gatt_status_t app_bt_connect_event_handler          (wiced_bt_gatt_connection_status_t *p_conn_status);
static wiced_bt_gatt_status_t app_bt_discover_services_by_uuid		(uint16_t connection_id);
void 						  app_bt_scan_callback					(wiced_bt_ble_scan_results_t *Scan_Results, uint8_t *adv_data);


static wiced_result_t app_bt_management_callback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data )
{
    wiced_result_t result = WICED_BT_ERROR;
    wiced_bt_device_address_t bda = {0};

    printf("Bluetooth Management Event: 0x%x %s\n", event, get_bt_event_name(event));

    switch( event )
    {
		case BTM_ENABLED_EVT:
			if( WICED_BT_SUCCESS == p_event_data->enabled.status )
			{
				printf( "*****Bluetooth Enabled*****\n" );

				// Set the local BDA from the value in the configurator and print it
				wiced_bt_set_local_bdaddr((uint8_t *)cy_bt_device_address, BLE_ADDR_PUBLIC);
				wiced_bt_dev_read_local_addr( bda );
				printf( "Local Bluetooth Device Address: ");
				print_bd_address(bda);

				// Disable pairing
				wiced_bt_set_pairable_mode( WICED_FALSE, WICED_FALSE );

				// Start scanning
				wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, WICED_TRUE, app_bt_scan_callback);

	            result = WICED_BT_SUCCESS;
			}
			else
			{
				printf( "Failed to initialize Bluetooth controller and stack\n" );
			}
			break;


		case BTM_BLE_SCAN_STATE_CHANGED_EVT:
			switch( p_event_data->ble_scan_state_changed )
			{
				case BTM_BLE_SCAN_TYPE_NONE:
					printf( "Scanning stopped.\r\n" );
					break;

				case BTM_BLE_SCAN_TYPE_HIGH_DUTY:
					printf( "High duty scanning.\r\n" );
					break;

				case BTM_BLE_SCAN_TYPE_LOW_DUTY:
					printf( "Low duty scanning.\r\n" );
					break;
			}
			result = WICED_BT_SUCCESS;
			break;

		default:
			break;
    }

    return result;
}

void app_bt_scan_callback(wiced_bt_ble_scan_results_t *Scan_Results, uint8_t *adv_data)
{
	if(Scan_Results->remote_bd_addr != NULL && Scan_Results != NULL)
	{
		printf("Device found : ");
		print_bd_address(Scan_Results->remote_bd_addr);
		printf("\n\r");

		uint8_t len;
		uint8_t *name = wiced_bt_ble_check_advertising_data(adv_data, BTM_BLE_ADVERT_TYPE_NAME_COMPLETE, &len);
		if(name && (len == strlen(DEVICE_NAME)) && !memcmp( DEVICE_NAME,name,len))
		{
			printf("PSOC Device found!\n\r");

			wiced_bt_gatt_register(app_bt_gatt_event_callback);

			// Stop scanning before connecting
			wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_NONE, WICED_TRUE, NULL);

			// Initiate connection
			wiced_bt_gatt_le_connect(Scan_Results->remote_bd_addr, Scan_Results->ble_addr_type, BLE_CONN_MODE_HIGH_DUTY, WICED_TRUE);
		}
	}
}


static wiced_bt_gatt_status_t app_bt_gatt_event_callback( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data )
{

    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;

    switch (event)
    {
		case GATT_CONNECTION_STATUS_EVT:
			{
				status = app_bt_connect_event_handler(&p_event_data->connection_status);
				break;
			}
		case GATT_DISCOVERY_CPLT_EVT:
			{
				wiced_bt_gatt_discovery_complete_t *p_complete = &p_event_data->discovery_complete;
				// Discovery Complete
				if (p_complete->discovery_type == GATT_DISCOVER_SERVICES_BY_UUID)
				{
					printf("Service discovery complete. Starting characteristic discovery...\n\r");
					printf("Char disc Start handle : %d\n\r",handle.service_start);
					printf("Char disc End handle : %d\n\r",handle.service_end);
					wiced_bt_gatt_discovery_param_t char_discovery_param = {0};
					char_discovery_param.s_handle = handle.service_start;
					char_discovery_param.e_handle = handle.service_end;
					wiced_bt_gatt_client_send_discover(connection_id, GATT_DISCOVER_CHARACTERISTICS, &char_discovery_param);
				}
				else if(p_complete->discovery_type == GATT_DISCOVER_CHARACTERISTICS)
				{
					printf("Characteristics discovery complete.\n\r");
					printf("--------------------------------\n\r");
					printf("Declaration Motor Handle : %d\n\r",handle.dclr_motor);
					printf("Writable Motor Handle : %d\n\r",handle.write_motor);
					printf("Declaration Lights Handle : %d\n\r",handle.dclr_lights);
					printf("Writable Lights Handle : %d\n\r",handle.write_lights);
					ServiceDiscoveryComplete = true;
				}
				break;
			}
		case GATT_OPERATION_CPLT_EVT:
			{
				//wiced_bt_gatt_operation_complete_t *p_data = &p_event_data->operation_complete;
				//printf("GATT Operation Complete. Op: 0x%X, Status: 0x%X\n",p_data->op, p_data->status);
				break;
			}

		case GATT_DISCOVERY_RESULT_EVT:
			{
				wiced_bt_gatt_discovery_result_t *p_result = &p_event_data->discovery_result;

				// Check if this is a service discovery result
				if (p_result->discovery_type == GATT_DISCOVER_SERVICES_BY_UUID)
				{
					handle.service_start = p_result->discovery_data.group_value.s_handle;
					handle.service_end = p_result->discovery_data.group_value.e_handle;
				}
				// Check if this is a characteristic discovery result
				else if (p_result->discovery_type == GATT_DISCOVER_CHARACTERISTICS)
				{
					handle.dclr_lights = p_result->discovery_data.characteristic_declaration.handle;
					uuid128 = p_result->discovery_data.characteristic_declaration.char_uuid.uu.uuid128;

					if (memcmp(uuid128, char_motor_uuid.uu.uuid128, LEN_UUID_128) == 0)
					{
						// Declaration Handle + 1 = writable handle.
						handle.write_motor = handle.dclr_lights+1;
						handle.dclr_motor = handle.dclr_lights;
					}
					else if(memcmp(uuid128, char_lights_uuid.uu.uuid128, LEN_UUID_128) == 0)
					{
						handle.write_lights = handle.dclr_lights+1;
					}
				}
				break;
			}
		default:
			// printf( "Unhandled GATT Event: 0x%x (%d)\n", event, event );
			status = WICED_BT_GATT_SUCCESS;
			break;
    }

    return status;
}

static wiced_bt_gatt_status_t app_bt_discover_services_by_uuid(uint16_t connection_id)
{
	wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;

    wiced_bt_gatt_discovery_param_t discovery_param;

    memset(&discovery_param, 0, sizeof(discovery_param));
    discovery_param.s_handle = 1;
    discovery_param.e_handle = 0xFFFF;
    memcpy(discovery_param.uuid.uu.uuid128, service_uuid.uu.uuid128, LEN_UUID_128);
    discovery_param.uuid.len = LEN_UUID_128;

    status = wiced_bt_gatt_client_send_discover(connection_id, GATT_DISCOVER_SERVICES_BY_UUID, &discovery_param);

    return status;
}


static wiced_bt_gatt_status_t app_bt_connect_event_handler(wiced_bt_gatt_connection_status_t *p_conn_status)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_ERROR;

    if (NULL != p_conn_status)
    {
        if (p_conn_status->connected)
        {
           	printf("GATT_CONNECTION_STATUS_EVT: Connect BDA ");
           	print_bd_address(p_conn_status->bd_addr);
			printf("Connection ID %d\n", p_conn_status->conn_id );

			connection_id = p_conn_status->conn_id;
			status = app_bt_discover_services_by_uuid(connection_id);
        }
        else
        {
            printf("Disconnected : BDA " );
            print_bd_address(p_conn_status->bd_addr);
            printf("Connection ID '%d', Reason '%s'\n", p_conn_status->conn_id, get_bt_gatt_disconn_reason_name(p_conn_status->reason) );

			// Handle the disconnection
            connection_id = 0;

            // Restart scanning
            printf("Restart Scanning...\n\r");
            wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, WICED_TRUE, app_bt_scan_callback);
            status = WICED_BT_GATT_SUCCESS;
        }
    }

    return status;
}

#endif /* SOURCES_BLUETOOTH_H_ */
