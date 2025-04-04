#pragma once
#include "pdu.h"

// Task priorities
#define TASK_BUTTON_PRIORITY     5
#define TASK_RECEIVE_PRIORITY    6
#define TASK_TRANSMIT_PRIORITY   7
#define TASK_ADC_PRIORITY        9
#define TASK_LED_PRIORITY        8
#define TASK_PERIODIC_PRIORITY   10

// Task stack sizes
#define TASK_STACKSIZE (256 * 4)

// Task function declarations
void TaskReceive(void* pdata);
void TaskTransmit(void* pdata);
void TaskButton(void* pdata);
void TaskLED(void* pdata);
void TaskADC(void* pdata);
void TaskPeriodic(void* pdata);

/* Event Flags------------------------------------------------------------------ */
#define LED_EVENT   	 0x01  		// LED_Blink_task
#define ADC_EVENT   	 0x02  		// Adc_task
#define BUTTON_EVENT 	 0x04  		// Button_task
#define TRANSMIT_EVENT   0x08		// Transmit_task
#define UART_RX_EVENT    0x10		// UART Reception

/* Queue Definitions ------------------------------------------------------------*/
#define QUEUE_SIZE 16
