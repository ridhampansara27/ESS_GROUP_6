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
#include "main.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bme68x.h"
#include "bme68x_defs.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BME68X_I2C_ADDR  0x77  // Your BME680 I2C address

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

struct bme68x_dev bme;
struct bme68x_conf conf;
struct bme68x_heatr_conf heatr_conf;
struct bme68x_data data;

uint8_t rx_byte;
char command_buffer[32];
uint8_t cmd_index = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}




int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr) {
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef*)intf_ptr;
    if (HAL_I2C_Mem_Read(hi2c, BME68X_I2C_ADDR << 1, reg_addr,
                         I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY) == HAL_OK)
        return 0;
    else
        return -1;
}


int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr) {
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef*)intf_ptr;
    if (HAL_I2C_Mem_Write(hi2c, BME68X_I2C_ADDR << 1, reg_addr,
                          I2C_MEMADD_SIZE_8BIT, (uint8_t*)data, len, HAL_MAX_DELAY) == HAL_OK)
        return 0;
    else
        return -1;
}


void user_delay_us(uint32_t period, void *intf_ptr) {
    // crude software delay loop (not precise)
    for (volatile uint32_t i = 0; i < period * 10; i++) {
        __NOP();
    }
}





void BME680_Init(void) {
    bme.intf = BME68X_I2C_INTF;
    bme.read = user_i2c_read;
    bme.write = user_i2c_write;
    bme.delay_us = user_delay_us;
    bme.intf_ptr = &hi2c1;

    if (bme68x_init(&bme) != BME68X_OK) {
        printf("BME680 init failed!\r\n");
        while (1);
    }

    // Sensor configuration (optimized for BME680)
    conf.os_temp = BME68X_OS_8X;
    conf.os_pres = BME68X_OS_4X;
    conf.os_hum = BME68X_OS_2X;
    conf.filter = BME68X_FILTER_SIZE_3;
    conf.odr = BME68X_ODR_NONE;
    bme68x_set_conf(&conf, &bme);

    // Heater configuration
    heatr_conf.enable = BME68X_ENABLE;
    heatr_conf.heatr_temp = 320;     // °C
    heatr_conf.heatr_dur = 150;      // ms
    bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);

    printf("BME680 Initialized.\r\n");

    // Warm-up dummy read
    bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
    uint32_t delay_us = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + heatr_conf.heatr_dur * 1000;
    user_delay_us(delay_us, bme.intf_ptr);

    uint8_t n_fields;
    bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
}





/*void BME680_Init(void) {
    int8_t rslt;

    bme.intf = BME68X_I2C_INTF;
    bme.read = user_i2c_read;
    bme.write = user_i2c_write;
    bme.delay_us = user_delay_us;
    bme.intf_ptr = &hi2c1;

    rslt = bme68x_init(&bme);
    if (rslt != BME68X_OK) {
        printf("BME680 init failed: %d\r\n", rslt);
        while (1);
    }

    // Set oversampling, filter
    conf.os_temp = BME68X_OS_8X;
    conf.os_pres = BME68X_OS_4X;
    conf.os_hum = BME68X_OS_2X;
    conf.filter = BME68X_FILTER_SIZE_3;
    conf.odr = BME68X_ODR_NONE;
    bme68x_set_conf(&conf, &bme);

    // Set heater for 320°C for 150 ms (standard)
    heatr_conf.enable = BME68X_ENABLE;
    heatr_conf.heatr_temp = 320;     // °C
    heatr_conf.heatr_dur = 150;      // ms
    bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);

    printf("BME680 Initialized.\r\n");

    // Optional: test 1st measurement right after init
    bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
    uint32_t delay_us = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + heatr_conf.heatr_dur * 1000;
    user_delay_us(delay_us, bme.intf_ptr);

    uint8_t n_fields = 0;
    rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
    if (rslt == BME68X_OK && n_fields) {
        printf("Warm-up data: T=%.2f°C, H=%.2f%%\r\n",
               data.temperature / 100.0f,
               data.humidity / 1000.0f);
    } else {
        printf("Warm-up read failed: rslt=%d, n=%d\r\n", rslt, n_fields);
    }
}*/




/*
void BME680_Init(void) {
    bme.intf = BME68X_I2C_INTF;
    bme.read = user_i2c_read;
    bme.write = user_i2c_write;
    bme.delay_us = user_delay_us;
    bme.intf_ptr = &hi2c1;

    if (bme68x_init(&bme) != BME68X_OK) {
        printf("BME680 initialization failed!\r\n");
        while (1);
    }

    conf.filter = BME68X_FILTER_OFF;
    conf.odr = BME68X_ODR_NONE;
    conf.os_hum = BME68X_OS_16X;
    conf.os_pres = BME68X_OS_1X;
    conf.os_temp = BME68X_OS_2X;
    bme68x_set_conf(&conf, &bme);

    heatr_conf.enable = BME68X_ENABLE;
    heatr_conf.heatr_temp = 320;
    heatr_conf.heatr_dur = 150;
    bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);

    printf("BME680 Initialized.\r\n");
}
*/





void handle_command(char *cmd) {
    // Trigger measurement
    if (bme68x_set_op_mode(BME68X_FORCED_MODE, &bme) != BME68X_OK)
        return;

    uint32_t delay_us = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + heatr_conf.heatr_dur * 1000;
    user_delay_us(delay_us, bme.intf_ptr);

    uint8_t n_fields = 0;
    if (bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme) != BME68X_OK || n_fields == 0) {
        printf("Sensor data not available.\r\n");
        return;
    }

    float temp = data.temperature / 1.0f;
    float press = data.pressure / 100.0f;
    float hum = data.humidity / 1.0f;
    float gas = (float)data.gas_resistance;

    // Apply value sanity filters
    if (temp < -40 || temp > 85) temp = 0;        // Temperature typical range
    if (hum < 0 || hum > 100) hum = 0;            // Humidity realistic range
    if (press < 300 || press > 1100) press = 0;   // Pressure in hPa
    if (gas < 100 || gas > 1000000) gas = 0;      // Gas in Ohms


    if (strcmp(cmd, "TEMP") == 0) {
        printf("Temperature: %.2f °C\r\n", temp);
   // if (strcmp(cmd, "TEMP") == 0) {
      //  printf("Temperature: %.2f °C\r\n", 25.00);
    } else if (strcmp(cmd, "HUM") == 0) {
        printf("Humidity: %.2f %%RH\r\n", hum);
    } else if (strcmp(cmd, "PRESS") == 0) {
        printf("Pressure: %.2f hPa\r\n", press);
    } else if (strcmp(cmd, "GAS") == 0) {
        printf("Gas Resistance: %.2f Ω\r\n", gas);
    } else if (strcmp(cmd, "ALL") == 0) {
        printf("Temperature: %.2f °C\r\n", temp);
        printf("Pressure:    %.2f hPa\r\n", press);
        printf("Humidity:    %.2f %%RH\r\n", hum);
        printf("Gas:         %.2f Ω\r\n", gas);
    } else if (strcmp(cmd, "HELP") == 0) {
        printf("Available commands:\r\n");
        printf("  TEMP   - Show temperature\r\n");
        printf("  HUM    - Show humidity\r\n");
        printf("  PRESS  - Show pressure\r\n");
        printf("  GAS    - Show gas resistance\r\n");
        printf("  ALL    - Show all values\r\n");
    } else {
        printf("Unknown command: %s\r\n", cmd);
    }
}









void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART2) {
        if (rx_byte == '\r' || rx_byte == '\n') {
            if (cmd_index > 0) {
                command_buffer[cmd_index] = '\0';
                printf("Command received: %s\r\n", command_buffer);
                handle_command(command_buffer);
                cmd_index = 0;
            }
        } else {
            command_buffer[cmd_index++] = rx_byte;
            if (cmd_index >= sizeof(command_buffer)) {
                cmd_index = 0;  // reset buffer to avoid overflow
            }
        }
        HAL_UART_Receive_IT(&huart2, &rx_byte, 1);  // restart interrupt
    }
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  printf("STM32 BME680 CLI Project Start\r\n");

  BME680_Init();
  HAL_Delay(1000);  // Allow sensor to stabilize

  printf("Type TEMP, HUM, PRESS, GAS or ALL and hit Enter\r\n\r\n");

  HAL_UART_Receive_IT(&huart2, &rx_byte, 1); // Start receiving UART
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_Delay(100);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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
#ifdef USE_FULL_ASSERT
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
