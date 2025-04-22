#include "pdu.h"
#include "tasks.h"
#include "includes.h"
extern OS_MEM *pdu_pool;


/* PDU Processing Functions */
bool validate_pdu_header(const PDU* pdu) {
    return (pdu->header == PDU_HEADER);
}

bool validate_led_payload(uint16_t payload) {
    return (payload >= 100 && payload <= 999);
}

bool validate_adc_payload(uint16_t payload) {
    return (payload >= 0 && payload <= 49);
}

void process_received_pdu(PDU* pdu) {
    uint16_t payload;
    memcpy(&payload, &pdu->data[2], sizeof(payload));

    OS_ERR err;
    switch (pdu->sid) {
        case SID_LED_BLINK:
            if (validate_led_payload(payload)) {
                OSQPost(led_queue, pdu);
                //OSFlagPost(event_flags, LED_EVENT, OS_FLAG_SET, &err);
            } else {
                SendNegativeResponse(pdu->sid);
            }
            break;

        case SID_ADC_READ:
            pdu->data[0] = 2;
            if (validate_adc_payload(payload)) {
                OSQPost(adc_queue, pdu);
                //OSFlagPost(event_flags, ADC_EVENT, OS_FLAG_SET, &err);
            } else {
                SendNegativeResponse(pdu->sid);
            }
            break;

        default:
            SendNegativeResponse(pdu->sid);
    }
}

void SendPositiveResponse(uint8_t sid) {
    INT8U err;
    PDU *response = (PDU *)OSMemGet(pdu_pool, &err);  // Allocate from pool

    if (err == OS_ERR_NONE) {
        response->header = PDU_HEADER;
        response->sid = sid;
        memset(response->data, 0xFF, sizeof(response->data));
        response->data[0] = 0xAA;  // Success code

        OSQPost(tx_queue, (void *)response);  // µC/OS-II uses 2 arguments
        //OSFlagPost(event_flags, TRANSMIT_EVENT, OS_FLAG_SET, &err);
    }
}

void SendNegativeResponse(uint8_t wrongsid) {
    OS_ERR err;
    PDU *response = (PDU *)OSMemGet(pdu_pool, &err);  // Allocate from pool

    if (err == OS_ERR_NONE) {
        response->header = PDU_HEADER;
        response->sid = wrongsid;
        memset(response->data, 0xFF, sizeof(response->data));
        response->data[0] = 0x7F;  // Error code

        OSQPost(tx_queue, (void *)response);
        //OSFlagPost(event_flags, TRANSMIT_EVENT, OS_FLAG_SET, &err);
    }
}
