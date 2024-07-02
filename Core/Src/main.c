#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


UART_HandleTypeDef huart2;
char readBuf[1];
__IO ITStatus UartReady = SET;


void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
uint8_t processUserInput(uint8_t opt);
int8_t readUserInput(void);


int main(void)
{
	uint8_t opt = 0;


  HAL_Init();


  SystemClock_Config();


  MX_GPIO_Init();
  MX_USART2_UART_Init();

  HAL_NVIC_SetPriority(USART2_IRQn, 0,0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);


  while (1)
  {
	  opt = readUserInput();
	  if(opt > 0){
		  processUserInput(opt);
	  }

	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
	  HAL_Delay(250);

	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	  HAL_Delay(250);

	 HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
	 HAL_Delay(250);



  }

}

void USART2_IRQHandler(void){
	HAL_UART_IRQHandler(&huart2);

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle){

	UartReady = SET;


}

int8_t readUserInput(void){
	int8_t retVal = -1;

	if(UartReady == SET){
		UartReady = RESET;
		HAL_UART_Receive_IT(&huart2, (uint8_t*) readBuf, 1);
		retVal = atoi(readBuf);
	}

	return retVal;

}

uint8_t processUserInput(uint8_t opt){
	switch(opt){

	case 1:
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
		break;
	}

	return 1;
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};


  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);


  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }


  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }


}


static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);


   GPIO_InitStruct.Pin = GPIO_PIN_13;
   GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


   GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6;
   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


}

void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
