#pragma once
#include "main.h"
#include "pdu.h"
#include "tasks.h"

// Buffer -------------------------------------------------------------------------
#define PDU_LENGTH 7 // It's for receiving buffer: "5A3B100" = 7 bytes
extern uint8_t pdu_buffer[PDU_LENGTH + 1];	// +1 for null terminator

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
uint8_t AsciiHexToByte(uint8_t high_nibble, uint8_t low_nibble);
void PrintTransmitted(PDU *pdu);
