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
#include "stm32g4xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LIDAR_M_EN_Pin GPIO_PIN_13
#define LIDAR_M_EN_GPIO_Port GPIOC
#define Bouton_1_Pin GPIO_PIN_0
#define Bouton_1_GPIO_Port GPIOA
#define TX_link_Pin GPIO_PIN_2
#define TX_link_GPIO_Port GPIOA
#define RX_link_Pin GPIO_PIN_3
#define RX_link_GPIO_Port GPIOA
#define Codeur1_PH2_Pin GPIO_PIN_4
#define Codeur1_PH2_GPIO_Port GPIOA
#define LIDAR_DEV_EN_Pin GPIO_PIN_4
#define LIDAR_DEV_EN_GPIO_Port GPIOC
#define PWM_MOT1_CH2_Pin GPIO_PIN_0
#define PWM_MOT1_CH2_GPIO_Port GPIOB
#define PWM_MOT2_CH2_Pin GPIO_PIN_1
#define PWM_MOT2_CH2_GPIO_Port GPIOB
#define nCS_Pin GPIO_PIN_13
#define nCS_GPIO_Port GPIOB
#define Status_purple_Pin GPIO_PIN_14
#define Status_purple_GPIO_Port GPIOB
#define Status_orange_Pin GPIO_PIN_15
#define Status_orange_GPIO_Port GPIOB
#define INT2_ADX_Pin GPIO_PIN_6
#define INT2_ADX_GPIO_Port GPIOC
#define PWM_MOT1_CH1_Pin GPIO_PIN_9
#define PWM_MOT1_CH1_GPIO_Port GPIOA
#define PWM_MOT2_CH1_Pin GPIO_PIN_10
#define PWM_MOT2_CH1_GPIO_Port GPIOA
#define LIDAR_M_SCTR_Pin GPIO_PIN_11
#define LIDAR_M_SCTR_GPIO_Port GPIOA
#define INT_TOF2_Pin GPIO_PIN_12
#define INT_TOF2_GPIO_Port GPIOA
#define Codeur1_PH1_Pin GPIO_PIN_4
#define Codeur1_PH1_GPIO_Port GPIOB
#define INT1_ADX_Pin GPIO_PIN_5
#define INT1_ADX_GPIO_Port GPIOB
#define Codeur2_PH1_Pin GPIO_PIN_6
#define Codeur2_PH1_GPIO_Port GPIOB
#define Codeur2_PH2_Pin GPIO_PIN_7
#define Codeur2_PH2_GPIO_Port GPIOB
#define INT_TOF1_Pin GPIO_PIN_8
#define INT_TOF1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
