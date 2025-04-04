#include "uart_handler.h"
#include "includes.h"

extern UART_HandleTypeDef huart2;
//uint8_t pdu_buffer[PDU_LENGTH + 1];	// +1 for null terminator

void PrintTransmited(PDU *pdu) {
	// Print the PDU contents
	printf("Transmit_Task received a PDU:\r\n");
	printf("Header: 0x%02X\r\n", pdu->header);
	printf("SID: 0x%02X\r\n", pdu->sid);
	printf("Payload: ");
	for (int i = 0; i < 6; i++) {
		printf("0x%02X ", pdu->data[i]);
	}
	printf("\r\n");
}

// Function to convert ASCII hex pair (e.g., "5A") into a uint8_t (0x5A)
uint8_t AsciiHexToByte(uint8_t high_nibble, uint8_t low_nibble) {
    uint8_t result = 0;

    // Convert high nibble (e.g., '5' → 0x05)
    if (high_nibble >= '0' && high_nibble <= '9') {
        result = (high_nibble - '0') << 4;
    } else if (high_nibble >= 'A' && high_nibble <= 'F') {
        result = (high_nibble - 'A' + 10) << 4;
    } else if (high_nibble >= 'a' && high_nibble <= 'f') {
        result = (high_nibble - 'a' + 10) << 4;
    }

    // Convert low nibble (e.g., 'A' → 0x0A)
    if (low_nibble >= '0' && low_nibble <= '9') {
        result |= (low_nibble - '0');
    } else if (low_nibble >= 'A' && low_nibble <= 'F') {
        result |= (low_nibble - 'A' + 10);
    } else if (low_nibble >= 'a' && low_nibble <= 'f') {
        result |= (low_nibble - 'a' + 10);
    }

    return result;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART2) {
		OS_ERR err;
		OSFlagPost(event_flags, UART_RX_EVENT, OS_FLAG_SET, &err);
		HAL_UART_Receive_IT(&huart2, pdu_buffer, PDU_LENGTH);
	}
}
