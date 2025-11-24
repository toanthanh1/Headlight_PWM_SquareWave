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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx.h"




int main(void)
{
  // Enable clock for GPIOD and TIM4 (bare metal)
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

  // Configure PD14 as Alternate Function (AF2 - TIM4_CH3)
  GPIOD->MODER &= ~(3 << (14 * 2));
  GPIOD->MODER |=  (2 << (14 * 2)); // AF mode
  GPIOD->AFR[1] &= ~(0xF << ((14-8)*4));
  GPIOD->AFR[1] |=  (2 << ((14-8)*4)); // AF2 for TIM4

  // Configure TIM4 for PWM mode on Channel 3
  TIM4->PSC = 7;         // Prescaler = 7 (divide clock by 8)
  TIM4->ARR = 10000-1;   // Auto-reload = 9999 (Period)
  TIM4->CCR3 = 0;        // Initial duty cycle = 0
  TIM4->CCMR2 &= ~(0xFF << 0);
  TIM4->CCMR2 |= (6 << 4); // PWM mode 1 for CH3
  TIM4->CCMR2 |= (1 << 3); // Enable preload for CCR3
  TIM4->CCER  |= TIM_CCER_CC3E; // Enable output CH3
  TIM4->CR1   |= TIM_CR1_ARPE;  // Enable preload for ARR
  TIM4->EGR   |= TIM_EGR_UG;    // Update registers
  TIM4->CR1   |= TIM_CR1_CEN;   // Enable timer

  // Generate square wave: alternate between 0% and 100% duty cycle
  while (1)
  {
    TIM4->CCR3 = 10000; // 100% duty (LED ON)
    for (volatile uint32_t i = 0; i < 800000; ++i); // ~1s delay

    TIM4->CCR3 = 0;     // 0% duty (LED OFF)
    for (volatile uint32_t i = 0; i < 800000; ++i); // ~1s delay
  }
}


// SystemClock_Config is not needed for bare metal (HSI default)







/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */

// Error_Handler is not needed for bare metal

