/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define sensor1_Pin GPIO_PIN_0
#define sensor1_GPIO_Port GPIOC
#define sensor1_EXTI_IRQn EXTI0_IRQn
#define sensor2_Pin GPIO_PIN_1
#define sensor2_GPIO_Port GPIOC
#define sensor2_EXTI_IRQn EXTI1_IRQn
#define TX_Pin GPIO_PIN_0
#define TX_GPIO_Port GPIOA
#define RX_Pin GPIO_PIN_1
#define RX_GPIO_Port GPIOA
#define sensor4_Pin GPIO_PIN_4
#define sensor4_GPIO_Port GPIOA
#define sensor4_EXTI_IRQn EXTI4_IRQn
#define sensor6_Pin GPIO_PIN_5
#define sensor6_GPIO_Port GPIOA
#define sensor6_EXTI_IRQn EXTI9_5_IRQn
#define sensor8_Pin GPIO_PIN_7
#define sensor8_GPIO_Port GPIOA
#define sensor8_EXTI_IRQn EXTI9_5_IRQn
#define D_C_Pin GPIO_PIN_1
#define D_C_GPIO_Port GPIOB
#define D_D_Pin GPIO_PIN_2
#define D_D_GPIO_Port GPIOB
#define D_E_Pin GPIO_PIN_12
#define D_E_GPIO_Port GPIOB
#define D_A_Pin GPIO_PIN_14
#define D_A_GPIO_Port GPIOB
#define D_B_Pin GPIO_PIN_15
#define D_B_GPIO_Port GPIOB
#define sensor5_Pin GPIO_PIN_9
#define sensor5_GPIO_Port GPIOA
#define sensor5_EXTI_IRQn EXTI9_5_IRQn
#define sensor3_Pin GPIO_PIN_10
#define sensor3_GPIO_Port GPIOA
#define sensor3_EXTI_IRQn EXTI15_10_IRQn
#define D_F_Pin GPIO_PIN_11
#define D_F_GPIO_Port GPIOA
#define D_G_Pin GPIO_PIN_12
#define D_G_GPIO_Port GPIOA
#define sensor7_Pin GPIO_PIN_3
#define sensor7_GPIO_Port GPIOB
#define sensor7_EXTI_IRQn EXTI3_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
