#pragma once
#include "main.h"
#include "includes.h"

/* PDU Definitions -----------------------------------------------------------*/
#define PDU_SIZE           	8
#define PDU_HEADER         	0x5A
#define SID_LED_BLINK  	   	0x18
#define SID_ADC_READ    	0x3B
#define SID_BUTTON_EVENT    0x22
#define SID_NEG_RESPONSE    0x7F
#define SID_POS_RESPONSE    0xAA

// PDU Structure Definition
typedef struct {
    uint8_t header; 				// Always 0x5A
    uint8_t sid;					// Subfunction ID
    uint8_t data[PDU_SIZE - 2];		// Payload data
} PDU;

// Shared queue and flags
extern OS_EVENT *tx_queue;		// Global queue pointer
extern OS_EVENT *led_queue;
extern OS_EVENT *adc_queue;
extern OS_FLAG_GRP *event_flags;

// PDU processing functions
bool validate_pdu_header(const PDU* pdu);
bool validate_led_payload(uint16_t payload);
bool validate_adc_payload(uint16_t payload);
void process_received_pdu(PDU* pdu);
void SendPositiveResponse(uint8_t sid);
void SendNegativeResponse(uint8_t wrong_sid);
