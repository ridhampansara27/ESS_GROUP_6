/* Includes */
#include "main.h"
#include "bme68x.h"
#include <stdio.h>
#include <math.h>

/* Private variables */
#define BME68X_I2C_ADDR 0x77

I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;

struct bme68x_dev bme;
struct bme68x_conf conf;
struct bme68x_heatr_conf heatr_conf;
struct bme68x_data data;

/* Retarget printf to UART */
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
    return len;
}


// Bosch interface functions



/* System Clock and Peripheral Init Functions (HAL init, clocks, GPIO, I2C, UART) should be here */
/* You can use the auto-generated CubeMX initialization functions */






int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
	I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)intf_ptr;
	if (HAL_I2C_Mem_Read(hi2c, BME68X_I2C_ADDR << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY) == HAL_OK)
	    return 0;
	else
	    return -1;

}

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
	I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)intf_ptr;
	if (HAL_I2C_Mem_Write(hi2c, BME68X_I2C_ADDR << 1, reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data, len, HAL_MAX_DELAY) == HAL_OK)
	    return 0;
	else
	    return -1;

}



//void user_delay_us(uint32_t period, void *intf_ptr) {
//    for(uint32_t i = 0; i < period * 16; i++) {
//        __NOP();
//    }
//}


void user_delay_us(uint32_t period, void *intf_ptr)
{
    // Use microsecond delay if you have it, else approximate with HAL_Delay (ms)
    HAL_Delay((period + 999) / 1000); // Convert us to ms safely
}









void BME688_Init(void)
{
    bme.intf = BME68X_I2C_INTF;
    bme.read = user_i2c_read;
    bme.write = user_i2c_write;
    bme.delay_us = user_delay_us;
//    bme.delay_us(bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + heatr_conf.heatr_dur * 1000, bme.intf_ptr);

//    uint8_t dev_addr = 0x77; // your detected address, if i use this then change in int8_t user _i2c_read AND write.
//    bme.intf_ptr = &dev_addr;
    bme.intf_ptr = &hi2c1;



    int8_t rslt = bme68x_init(&bme);
    if (rslt != BME68X_OK)
    {
        printf("BME688 initialization failed!\r\n");
        while (1);
    }



    struct bme68x_conf conf;
    conf.filter = BME68X_FILTER_OFF;
    conf.odr = BME68X_ODR_NONE;
    conf.os_hum = BME68X_OS_16X;
    conf.os_pres = BME68X_OS_1X;
    conf.os_temp = BME68X_OS_2X;
    bme68x_set_conf(&conf, &bme);

    struct bme68x_heatr_conf heatr_conf;
    heatr_conf.enable = BME68X_ENABLE;
    heatr_conf.heatr_temp = 320; // typical heater temp
    heatr_conf.heatr_dur = 150;  // typical duration in ms
    rslt = bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);





//    bme68x_get_conf(&conf, &bme);
//    conf.filter = BME68X_FILTER_OFF;
//    conf.odr = BME68X_ODR_NONE;
//    conf.os_hum = BME68X_OS_2X;
//    conf.os_pres = BME68X_OS_4X;
//    conf.os_temp = BME68X_OS_8X;
//    bme68x_set_conf(&conf, &bme);
//
//    heatr_conf.enable = BME68X_ENABLE;
//    heatr_conf.heatr_temp = 320; // 320°C
//    heatr_conf.heatr_dur = 150;  // 150 ms
//    bme68x_set_heatr_conf(BME68X_FORCED_MODE, &heatr_conf, &bme);
}






// Declare HAL handlers
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;



// Peripheral init prototypes
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_I2C1_Init(void);
void MX_USART2_UART_Init(void);



int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART2_UART_Init();

    printf("\r\nSTM32 BME688 Initialization...\r\n");
    BME688_Init();
    printf("BME688 initialized successfully.\r\n");

    while (1)
    {
//        uint8_t n_fields;
//        bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
//        bme.delay_us(bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + heatr_conf.heatr_dur * 1000, bme.intf_ptr);
//
//        bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
//
//
//        if (n_fields)
//        {
//            printf("Temp: %.2f C, Pres: %.2f hPa, Hum: %.2f %%, Gas res: %.2f ohms\r\n",
//                   data.temperature, data.pressure / 100, data.humidity, data.gas_resistance);
//        }
//
//        HAL_Delay(1000);

//        	uint8_t n_fields;
//        	int8_t rslt;
//
//            rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
//            printf("Set mode result: %d\r\n", rslt);
//
//            HAL_Delay(400); // 200 ms wait
//
//            memset(&data, 0, sizeof(data));
//            rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
//            printf("Get data result: %d, n_fields: %d\r\n", rslt, n_fields);
//
//            if (n_fields)
//            {
//            	printf("Raw data: T=%ld, P=%lu, H=%lu, G=%lu\r\n",                                  // show raw values
//            	       data.temperature, data.pressure, data.humidity, data.gas_resistance);
//            	printf("Raw data hex: T=0x%08lx, P=0x%08lx, H=0x%08lx, G=0x%08lx\r\n",
//            	       (unsigned long)data.temperature,
//            	       (unsigned long)data.pressure,
//            	       (unsigned long)data.humidity,
//            	       (unsigned long)data.gas_resistance);
//
//
//
//            	printf("Temp: %d.%02d C\r\n", (int)data.temperature, (int)(fabs(data.temperature * 100)) % 100);
//
//
//            	printf("Temp: %.2f C, Pres: %.2f hPa, Hum: %.2f %%, Gas res: %.2f ohms\r\n",
//            	               data.temperature / 100.0,
//            	               data.pressure / 100.0,
//            	               data.humidity / 1000.0,
//            	               (float)data.gas_resistance);
//            }
//            else
//            {
//                printf("No new data available.\r\n");
//            }
//
//            HAL_Delay(1000);


    	int8_t rslt = bme68x_set_op_mode(BME68X_FORCED_MODE, &bme);
    	    printf("Set mode result: %d\r\n", rslt);

    	    uint32_t del_period = bme68x_get_meas_dur(BME68X_FORCED_MODE, &conf, &bme) + heatr_conf.heatr_dur * 1000;
    	    user_delay_us(del_period, bme.intf_ptr);

    	    uint8_t n_fields;
    	    rslt = bme68x_get_data(BME68X_FORCED_MODE, &data, &n_fields, &bme);
    	    printf("Get data result: %d, n_fields: %d\r\n", rslt, n_fields);

    	    printf("Raw data: T=%ld, P=%ld, H=%ld, G=%ld\r\n",
    	        (long)data.temperature,
    	        (long)data.pressure,
    	        (long)data.humidity,
    	        (long)data.gas_resistance);

    	    printf("Temp: %d.%02d C, Pres: %lu.%02lu hPa, Hum: %d.%02d %%, Gas res: %lu ohms\r\n",
    	        (int)data.temperature, (int)(fabs(data.temperature * 100)) % 100,
    	        (uint32_t)(data.pressure / 100), (uint32_t)(((uint32_t)data.pressure % 100) * 100 / 100),
    	        (int)data.humidity, (int)(fabs(data.humidity * 100)) % 100,
    	        (uint32_t)data.gas_resistance);


    	    HAL_Delay(1000);

    }

}


/* USER CODE BEGIN 4 */
//int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
//{
//    I2C_HandleTypeDef *i2c = (I2C_HandleTypeDef*)intf_ptr;
//    uint16_t dev_addr = 0x77 << 1; // shifted for STM32 HAL
//    if (HAL_I2C_Mem_Read(i2c, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, HAL_MAX_DELAY) == HAL_OK)
//        return 0;
//    else
//        return -1;
//}
//
//int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
//{
//    I2C_HandleTypeDef *i2c = (I2C_HandleTypeDef*)intf_ptr;
//    uint16_t dev_addr = 0x77 << 1; // shifted for STM32 HAL
//    if (HAL_I2C_Mem_Write(i2c, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, (uint8_t*)data, len, HAL_MAX_DELAY) == HAL_OK)
//        return 0;
//    else
//        return -1;
//}






/* USER CODE END 4 */

// SystemClock_Config, MX_I2C1_Init, MX_USART2_UART_Init, MX_GPIO_Init, Error_Handler
// retain these as previously generated by CubeMX, unchanged
// or let me know if you need them rewritten cleanly as well.



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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00503D58;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//int _write(int file, char *ptr, int len)
//{
//    HAL_UART_Transmit(&huart2, (uint8_t*)ptr, len, HAL_MAX_DELAY);
//    return len;
//}

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
