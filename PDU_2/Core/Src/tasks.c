#include "tasks.h"
#include "pdu.h"
#include "uart_handler.h"
#include "adc_handler.h"

// Task implementations
void TaskReceive(void* pdata) {
	static PDU received_pdu;
    OS_ERR err;
    OS_FLAGS flags;

    while (1) {
    	// Wait for UART data (auto-clear flag)
		flags = OSFlagPend(event_flags, UART_RX_EVENT, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
		if (flags & UART_RX_EVENT) {

			// Clear all old data
			memset(&received_pdu, 0, sizeof(PDU));
			// Receive Header (Hex), Receive SID (Hex), Receive Payload (Decimal)
			received_pdu.header = AsciiHexToByte(pdu_buffer[0], pdu_buffer[1]);
			received_pdu.sid = AsciiHexToByte(pdu_buffer[2], pdu_buffer[3]);
			uint16_t payload = (uint16_t)atoi((char*)pdu_buffer + 4);

			// Prepare PDU, Sending to other tasks.
			memcpy(&received_pdu.data[2], &payload, sizeof(payload));

			// Validate and process PDU
			if (!validate_pdu_header(&received_pdu)) {
				SendNegativeResponse(0x7F);
				continue;
			}

			process_received_pdu(&received_pdu);
		}
    }
}

void TaskTransmit(void *pdata) {
	PDU *pdu_rx;
	INT8U err;
	OS_FLAGS flags;

	while(1) {
		flags = OSFlagPend(event_flags, TRANSMIT_EVENT, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
		if (flags & TRANSMIT_EVENT) {
			// Wait for a PDU from the queue
			pdu_rx = (PDU *)OSQPend(tx_queue, 0, &err);
			PrintTransmited(pdu_rx);
		}
	}
}

void TaskPeriodic(void *pdata) {
	PDU periodic_temp_pdu = {PDU_HEADER, SID_ADC_READ, {0x01}};
	OS_ERR err;
	while(1) {
		// Periodic Default PDU sending to ADC task
		OSQPost(tx_queue, (void*)&periodic_temp_pdu);  // Forward to ADC task
		OSFlagPost(event_flags, ADC_EVENT, OS_FLAG_SET, &err);
		OSTimeDlyHMSM(0, 0, 10, 0);
	}
}

void TaskButton(void *pdata) {
	PDU pdu_tx = {PDU_HEADER, SID_BUTTON_EVENT, {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x01}};
	INT8U err;
	OS_FLAGS flags;
	while(1) {

		// Wait for BUTTON_EVENT (button press)
		flags = OSFlagPend(event_flags, BUTTON_EVENT, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

		if (flags & BUTTON_EVENT) {
			// Send Button Response
			err = OSQPost(tx_queue, (void*)&pdu_tx); // Post to transmit queue
			OSFlagPost(event_flags, TRANSMIT_EVENT, OS_FLAG_SET, &err);
		}
	}
}

void TaskLED(void *pdata) {
	PDU *pdu_rx;
	INT8U err;
	OS_FLAGS flags;
	uint16_t blink_frequency;

    while(1) {

    	flags = OSFlagPend(event_flags, LED_EVENT, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

    	if (flags & LED_EVENT) {
    		// Receiving PDU from Queue
    		pdu_rx = (PDU *)OSQPend(tx_queue, 0, &err);
    		// Extract frequency from PDU payload
    		memcpy(&blink_frequency, &pdu_rx->data[2], sizeof(uint16_t));
			// Blink LED
			LEDBlink(blink_frequency);
			// Clear previous data
			memset(pdu_rx->data, 0, sizeof(pdu_rx->data));
			// Send Positive Response
			SendPositiveResponse(SID_LED_BLINK);
    	}
    }
}

void TaskADC(void *pdata) {
	OS_FLAGS flags;
	//PDU pdu_temp = {PDU_HEADER, SID_ADC_READ, {0x01}};
	PDU *pdu_rx;
	uint8_t temp;
	uint8_t offset = 0;
	INT8U err;

	while(1) {
		// Wait for ADC_EVENT
		flags = OSFlagPend(event_flags, ADC_EVENT, OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 0, &err);

		if (flags & ADC_EVENT) {
			// Receiving PDU from Queue
			pdu_rx = (PDU *)OSQPend(tx_queue, 0, &err);
			// Check if a new offset inserted
			if (pdu_rx->data[0] == 2) {
				// Extract offset from PDU payload
				memcpy(&offset, &pdu_rx->data[2], sizeof(offset));
				// Read temperature and apply offset
				temp = OffsetAddtoTemp(offset);
				// Clear pdu_rx
				memset(pdu_rx->data, 0, sizeof(pdu_rx->data));
				// Send Positive Response 0x5A 0x3B 0xAA ...
				SendPositiveResponse(SID_ADC_READ);

			} else {
				// Read temperature and apply offset
				temp = OffsetAddtoTemp(offset);
				// Clear pdu_rx
				memset(pdu_rx->data, 0, sizeof(pdu_rx->data));
				// Sending Periodic Temperature = 0x5A 0x3B 0x01 temperature
				SendTemperature(temp);
			}
		}
	}
}
