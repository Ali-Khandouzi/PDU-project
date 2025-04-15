#include "adc_handler.h"
#include "includes.h"
#include "pdu.h"

extern ADC_HandleTypeDef hadc3;
extern OS_MEM *pdu_pool;


uint8_t OffsetAddtoTemp(uint8_t offsett) {
	uint8_t temp = tempfunction();
	return temp + offsett;
}

void SendTemperature(uint8_t temperature) {
    INT8U err;
    PDU *response = (PDU *)OSMemGet(pdu_pool, &err);  // Allocate from pool

    if (err == OS_ERR_NONE) {
        response->header = PDU_HEADER;
        response->sid = SID_ADC_READ;
        memset(response->data, 0xFF, sizeof(response->data));
        response->data[0] = 0x01;
        memcpy(&response->data[5], &temperature, sizeof(temperature));
        printf("Temperature2 = %d\r\n", response->data[5]);

        OSQPost(tx_queue, (void *)response);
        OSFlagPost(event_flags, TRANSMIT_EVENT, OS_FLAG_SET, &err);
    }
}

float tempfunction(void) {
	uint16_t adc_value;
	float tempfloat;
	uint8_t tempint;

	// Read ADC Value
	HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
	HAL_ADC_Start(&hadc3);
	HAL_ADC_PollForConversion(&hadc3, HAL_MAX_DELAY); // MUST USE INTERRUPT, Don't Forget...
	adc_value = HAL_ADC_GetValue(&hadc3);
	HAL_ADC_Stop(&hadc3);

	tempfloat = ((float)adc_value) / 4095 * 3300;
	tempfloat = ((tempfloat - 760.0) / 2.5) + 25;
	tempint = (uint8_t)roundf(tempfloat);

	return tempint;
}
