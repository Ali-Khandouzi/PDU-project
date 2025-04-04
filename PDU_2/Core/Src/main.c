/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "includes.h"
#include "pdu.h"
#include "tasks.h"
#include "uart_handler.h"
#include "led_handler.h"
#include "adc_handler.h"

/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart2;

/* Definition of Task Stacks ------------------------------------------------ */
OS_STK	Task_Receive_stk[TASK_STACKSIZE];
OS_STK	Task_Transmit_stk[TASK_STACKSIZE];
OS_STK	Task_Button_stk[TASK_STACKSIZE];
OS_STK	Task_LED_stk[TASK_STACKSIZE];
OS_STK	Task_ADC_stk[TASK_STACKSIZE];
OS_STK	Task_Periodic_stk[TASK_STACKSIZE];

PDU *pdu_queue_buffer[QUEUE_SIZE]; // Queue buffer (array of PDU pointers)

OS_EVENT *tx_queue;				// Global queue pointer
OS_FLAG_GRP *event_flags;

uint8_t pdu_buffer[PDU_LENGTH + 1];

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  RetargetInit(&huart2);
  MX_ADC3_Init();
  OS_ERR err;

  /* USART2 interrupt Init */
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);

  // Initialize µC/OS-II
  OSInit();

  // Create Event Object
  event_flags = OSFlagCreate(0x00, &err); // Initial flags = 0

  // Create the queue
  tx_queue = OSQCreate((void **)&pdu_queue_buffer[0], QUEUE_SIZE);
  if (tx_queue == NULL) {
	  printf("Queue initialization failed\r\n");
	  Error_Handler();
  }

  OSTaskCreateExt(TaskReceive,
      		  	  	  NULL,
      				  (void *)&Task_Receive_stk[TASK_STACKSIZE],
  					  TASK_RECEIVE_PRIORITY,
  					  TASK_RECEIVE_PRIORITY,
  					  Task_Receive_stk,
      				  TASK_STACKSIZE,
      				  NULL,
  					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
  					  );

  OSTaskCreateExt(TaskTransmit,
					  NULL,
					  (void *)&Task_Transmit_stk[TASK_STACKSIZE],
					  TASK_TRANSMIT_PRIORITY,
					  TASK_TRANSMIT_PRIORITY,
					  Task_Transmit_stk,
					  TASK_STACKSIZE,
					  NULL,
					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
					  );

  OSTaskCreateExt(TaskButton,
					  NULL,
					  (void *)&Task_Button_stk[TASK_STACKSIZE],
					  TASK_BUTTON_PRIORITY,
					  TASK_BUTTON_PRIORITY,
					  Task_Button_stk,
					  TASK_STACKSIZE,
					  NULL,
					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
					  );

  OSTaskCreateExt(TaskLED,
					  NULL,
					  (void *)&Task_LED_stk[TASK_STACKSIZE],
					  TASK_LED_PRIORITY,
					  TASK_LED_PRIORITY,
					  Task_LED_stk,
					  TASK_STACKSIZE,
					  NULL,
					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
					  );

  OSTaskCreateExt(TaskADC,
					  NULL,
					  (void *)&Task_ADC_stk[TASK_STACKSIZE],
					  TASK_ADC_PRIORITY,
					  TASK_ADC_PRIORITY,
					  Task_ADC_stk,
					  TASK_STACKSIZE,
					  NULL,
					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
					  );

  OSTaskCreateExt(TaskPeriodic,
  					  NULL,
  					  (void *)&Task_Periodic_stk[TASK_STACKSIZE],
					  TASK_PERIODIC_PRIORITY,
					  TASK_PERIODIC_PRIORITY,
					  Task_Periodic_stk,
  					  TASK_STACKSIZE,
  					  NULL,
  					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
  					  );

  // Start UART reception
  HAL_UART_Receive_IT(&huart2, pdu_buffer, PDU_LENGTH);

  OSStart();
  return 0;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    OS_ERR err;
    if(GPIO_Pin == GPIO_PIN_13)
    	OSFlagPost(event_flags, BUTTON_EVENT, OS_FLAG_SET, &err);
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
