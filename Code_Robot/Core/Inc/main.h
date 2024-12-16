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
#define INT_TOF1_Pin GPIO_PIN_13
#define INT_TOF1_GPIO_Port GPIOC
#define INT_TOF2_Pin GPIO_PIN_14
#define INT_TOF2_GPIO_Port GPIOC
#define LIDAR_M_EN_Pin GPIO_PIN_1
#define LIDAR_M_EN_GPIO_Port GPIOA
#define TX_link_Pin GPIO_PIN_2
#define TX_link_GPIO_Port GPIOA
#define RX_link_Pin GPIO_PIN_3
#define RX_link_GPIO_Port GPIOA
#define LIDAR_DEV_EN_Pin GPIO_PIN_4
#define LIDAR_DEV_EN_GPIO_Port GPIOA
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOC
#define INT1_ADX_Pin GPIO_PIN_1
#define INT1_ADX_GPIO_Port GPIOB
#define INT1_ADX_EXTI_IRQn EXTI1_IRQn
#define INT2_ADX_Pin GPIO_PIN_2
#define INT2_ADX_GPIO_Port GPIOB
#define INT2_ADX_EXTI_IRQn EXTI2_IRQn
#define Status_Blue_Pin GPIO_PIN_13
#define Status_Blue_GPIO_Port GPIOB
#define Status_Red_Pin GPIO_PIN_14
#define Status_Red_GPIO_Port GPIOB
#define Bouton_1_Pin GPIO_PIN_15
#define Bouton_1_GPIO_Port GPIOB
#define PWM_MOT1_CH1_Pin GPIO_PIN_8
#define PWM_MOT1_CH1_GPIO_Port GPIOA
#define PWM_MOT2_CH1_Pin GPIO_PIN_9
#define PWM_MOT2_CH1_GPIO_Port GPIOA
#define LIDAR_M_SCTR_Pin GPIO_PIN_10
#define LIDAR_M_SCTR_GPIO_Port GPIOA
#define PWM_MOT1_CH2_Pin GPIO_PIN_11
#define PWM_MOT1_CH2_GPIO_Port GPIOA
#define PWM_MOT2_CH2_Pin GPIO_PIN_12
#define PWM_MOT2_CH2_GPIO_Port GPIOA
#define Status_Lidar_Pin GPIO_PIN_15
#define Status_Lidar_GPIO_Port GPIOA
#define TX_LIDAR_Pin GPIO_PIN_10
#define TX_LIDAR_GPIO_Port GPIOC
#define RX_LIDAR_Pin GPIO_PIN_11
#define RX_LIDAR_GPIO_Port GPIOC
#define Codeur1_PH1_Pin GPIO_PIN_4
#define Codeur1_PH1_GPIO_Port GPIOB
#define Codeur1_PH2_Pin GPIO_PIN_5
#define Codeur1_PH2_GPIO_Port GPIOB
#define Codeur2_PH1_Pin GPIO_PIN_6
#define Codeur2_PH1_GPIO_Port GPIOB
#define Codeur2_PH2_Pin GPIO_PIN_7
#define Codeur2_PH2_GPIO_Port GPIOB
#define SCL_TOF_Pin GPIO_PIN_8
#define SCL_TOF_GPIO_Port GPIOB
#define SDA_TOF_Pin GPIO_PIN_9
#define SDA_TOF_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
