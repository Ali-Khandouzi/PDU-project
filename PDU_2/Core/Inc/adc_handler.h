#pragma once
#include "main.h"
#include "pdu.h"
#include "tasks.h"

void SendTemperature(uint8_t temperature);
uint8_t OffsetAddtoTemp(uint8_t offsett);
float tempfunction(void);
