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
#include "stm32f3xx_hal.h"

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

/* USER CODE BEGIN Private defines */
#define RST_Pin GPIO_PIN_1     // Reset pin for e-Paper
#define RST_GPIO_Port GPIOA    // Connected to PA1

#define DC_Pin GPIO_PIN_2      // Data/Command pin for e-Paper
#define DC_GPIO_Port GPIOA     // Connected to PA2

#define BUSY_Pin GPIO_PIN_3    // Busy pin for e-Paper
#define BUSY_GPIO_Port GPIOA   // Connected to PA3

#define SPI_CS_Pin GPIO_PIN_4  // Chip Select pin for e-Paper
#define SPI_CS_GPIO_Port GPIOA // Connected to PA4

#define SCK_Pin GPIO_PIN_5     // Clock pin (SPI SCK) for e-Paper
#define SCK_GPIO_Port GPIOA    // Connected to PA5

#define PWR_Pin GPIO_PIN_6     // Optional power control (Not assigned yet)
#define PWR_GPIO_Port GPIOA    // Connected to PA6 (if needed for future)

#define DIN_Pin GPIO_PIN_7     // Data In (MOSI) for e-Paper
#define DIN_GPIO_Port GPIOA    // Connected to PA7
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
