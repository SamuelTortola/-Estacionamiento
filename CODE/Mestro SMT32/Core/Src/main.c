/* USER CODE BEGIN Header */
/**
   ******************************************************************************
 Universidad Del Valle De Guatemala
 IE2023: Electrónica digital 2
 Autor: Samuel Tortola - 22094, Alan Gomez - 22115
 Proyecto: Proyecto servidor WEB
 Hardware: STM32
 Creado: 8/11/2024
Última modificación: 3/11/2024
******************************************************************************
  */
//CODIGO DEL DISPLAY DE 7 SEGMENTOS, SENSORES INFRARROJOS, NEOPIXEL, ESTE SMT32 ES EL MAESTRO DE I2C

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "neo_pixel.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUMBER_OF_LEDS 8

uint8_t espacios_disponibles = 8, activa1 = 0, activa2 = 0, activa3 = 0, activa4 = 0, activa5 = 0, activa6 = 0, activa7 = 0, activa8 = 0, activador = 0, activador1 = 0;
int ESP32_ADDRESS = 0x01, STM32_SLAVE_ADDRESS = 0x02;



volatile uint32_t last_interrupt_time = 0; // Variable global para almacenar el tiempo de la última interrupción
uint32_t debounce_delay = 50; // Tiempo de debounce en ms




/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim3_ch1_trig;

/* USER CODE BEGIN PV */
neopixel_led leds[NUMBER_OF_LEDS + 1];
neopixel_led circular_pattern_led[2 * NUMBER_OF_LEDS + 1];
rgb_color led_pattern[NUMBER_OF_LEDS];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

void setSegment(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g);
void display0();
void display1();
void display2();
void display3();
void display4();
void display5();
void display6();
void display7();
void display8();
void display9();

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

    neopixel_led temp_neo_pixel;
	rgb_color red_color = {255, 0, 0};
	rgb_color green_color = {0, 255, 0};
	rgb_color blue_color = {0, 0, 255};
	rgb_color black_color = {0, 0, 0};
	uint16_t led_pos_counter = 0;

	  uint8_t data = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    uint32_t current_time = HAL_GetTick(); // Obtener el tiempo actual en ms
    activador = 1;

    if (GPIO_Pin == sensor1_Pin) {
        if ((current_time - last_interrupt_time) > debounce_delay) {
            if (activa1 == 0) {
                espacios_disponibles--;
                set_specific_led(leds, 0, &black_color);
                set_specific_led(leds, 0, &red_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs

                activa1 = 1;
            } else if (activa1 == 1) {
                espacios_disponibles++;
                set_specific_led(leds, 0, &black_color);
               set_specific_led(leds, 0, &green_color);
               HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa1 = 0;
            }
            last_interrupt_time = current_time;

             data = 0x08; // Declarar una variable de tipo uint8_t con el dato a enviar

           					 // Enviar datos al ESP32
	  if (HAL_I2C_Master_Transmit(&hi2c1, ESP32_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
			  // Manejar el error
			  Error_Handler();
		  }
		//   Enviar datos al STM32 esclavo
		  if (HAL_I2C_Master_Transmit(&hi2c1, STM32_SLAVE_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
			  // Manejar el error
			  Error_Handler();
		  }
        }
    } else if (GPIO_Pin == sensor2_Pin) {
        if ((current_time - last_interrupt_time) > debounce_delay) {
            if (activa2 == 0) {
                espacios_disponibles--;
                set_specific_led(leds, 1, &black_color);
                set_specific_led(leds, 1, &red_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa2 = 1;
            } else if (activa2 == 1) {
                espacios_disponibles++;
                set_specific_led(leds, 1, &black_color);
                set_specific_led(leds, 1, &green_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa2 = 0;
            }
            last_interrupt_time = current_time;

          data = 0x07; // Declarar una variable de tipo uint8_t con el dato a enviar

		 // Enviar datos al ESP32
		  if (HAL_I2C_Master_Transmit(&hi2c1, ESP32_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
			  // Manejar el error
			  Error_Handler();
		  }


		  	//   Enviar datos al STM32 esclavo
		  if (HAL_I2C_Master_Transmit(&hi2c1, STM32_SLAVE_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
			  // Manejar el error
			  Error_Handler();
		  }
        }
    } else if (GPIO_Pin == sensor3_Pin) {
        if ((current_time - last_interrupt_time) > debounce_delay) {
            if (activa3 == 0) {
                espacios_disponibles--;
                set_specific_led(leds, 6, &black_color);
                set_specific_led(leds, 6, &red_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa3 = 1;
            } else if (activa3 == 1) {
                espacios_disponibles++;
                set_specific_led(leds, 6, &black_color);
                set_specific_led(leds, 6, &green_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa3 = 0;
            }
            last_interrupt_time = current_time;

            data = 0x02; // Declarar una variable de tipo uint8_t con el dato a enviar

		 // Enviar datos al ESP32
		  if (HAL_I2C_Master_Transmit(&hi2c1, ESP32_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
			  // Manejar el error
			  Error_Handler();
		  }


		  //   Enviar datos al STM32 esclavo
		  if (HAL_I2C_Master_Transmit(&hi2c1, STM32_SLAVE_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
			  // Manejar el error
			  Error_Handler();
		  }
        }
    } else if (GPIO_Pin == sensor4_Pin) {
        if ((current_time - last_interrupt_time) > debounce_delay) {
            if (activa4 == 0) {
                espacios_disponibles--;
                set_specific_led(leds, 3, &black_color);
                set_specific_led(leds, 3, &red_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa4 = 1;
            } else if (activa4 == 1) {
                espacios_disponibles++;
                set_specific_led(leds, 3, &black_color);
                set_specific_led(leds, 3, &green_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa4 = 0;
            }
            last_interrupt_time = current_time;

            data = 0x05; // Declarar una variable de tipo uint8_t con el dato a enviar

		 // Enviar datos al ESP32
		  if (HAL_I2C_Master_Transmit(&hi2c1, ESP32_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
			  // Manejar el error
			  Error_Handler();
		  }


		  	//   Enviar datos al STM32 esclavo
		  if (HAL_I2C_Master_Transmit(&hi2c1, STM32_SLAVE_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
			  // Manejar el error
			  Error_Handler();
		  }
        }
    } else if (GPIO_Pin == sensor5_Pin) {
        if ((current_time - last_interrupt_time) > debounce_delay) {
            if (activa5 == 0) {
                espacios_disponibles--;
                set_specific_led(leds, 4, &black_color);
                set_specific_led(leds, 4, &red_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa5 = 1;
            } else if (activa5 == 1) {
                espacios_disponibles++;
                set_specific_led(leds, 4, &black_color);
                set_specific_led(leds, 4, &green_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa5 = 0;
            }
            last_interrupt_time = current_time;
             data = 0x04; // Declarar una variable de tipo uint8_t con el dato a enviar

			 // Enviar datos al ESP32
			  if (HAL_I2C_Master_Transmit(&hi2c1, ESP32_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
				  // Manejar el error
				  Error_Handler();
			  }

			  	//   Enviar datos al STM32 esclavo
			  if (HAL_I2C_Master_Transmit(&hi2c1, STM32_SLAVE_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
				  // Manejar el error
				  Error_Handler();
			  }
        }
    } else if (GPIO_Pin == sensor6_Pin) {
        if ((current_time - last_interrupt_time) > debounce_delay) {
            if (activa6 == 0) {
                espacios_disponibles--;
                set_specific_led(leds, 5, &black_color);
                set_specific_led(leds, 5, &red_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa6 = 1;
            } else if (activa6 == 1) {
                espacios_disponibles++;
                set_specific_led(leds, 5, &black_color);
                set_specific_led(leds, 5, &green_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa6 = 0;
            }
            last_interrupt_time = current_time;

             data = 0x03; // Declarar una variable de tipo uint8_t con el dato a enviar

			 // Enviar datos al ESP32
			  if (HAL_I2C_Master_Transmit(&hi2c1, ESP32_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
				  // Manejar el error
				  Error_Handler();
			  }


			  	//   Enviar datos al STM32 esclavo
			  if (HAL_I2C_Master_Transmit(&hi2c1, STM32_SLAVE_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
				  // Manejar el error
				  Error_Handler();
			  }
        }
    } else if (GPIO_Pin == sensor7_Pin) {
        if ((current_time - last_interrupt_time) > debounce_delay) {
            if (activa7 == 0) {
                espacios_disponibles--;
                set_specific_led(leds, 2, &black_color);
                set_specific_led(leds, 2, &red_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa7 = 1;
            } else if (activa7 == 1) {
                espacios_disponibles++;
                set_specific_led(leds, 2, &black_color);
                set_specific_led(leds, 2, &green_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa7 = 0;
            }
            last_interrupt_time = current_time;
            data = 0x06; // Declarar una variable de tipo uint8_t con el dato a enviar

			 // Enviar datos al ESP32
			  if (HAL_I2C_Master_Transmit(&hi2c1, ESP32_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
				  // Manejar el error
				  Error_Handler();
			  }

			  //   Enviar datos al STM32 esclavo
			  if (HAL_I2C_Master_Transmit(&hi2c1, STM32_SLAVE_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
				  // Manejar el error
				  Error_Handler();
			  }
        }
    } else if (GPIO_Pin == sensor8_Pin) {
        if ((current_time - last_interrupt_time) > debounce_delay) {
            if (activa8 == 0) {
                espacios_disponibles--;
                set_specific_led(leds, 7, &black_color);
                set_specific_led(leds, 7, &red_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa8 = 1;
            } else if (activa8 == 1) {
                espacios_disponibles++;
                set_specific_led(leds, 7, &black_color);
                set_specific_led(leds, 7, &green_color);
                HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs
                activa8 = 0;
            }
            last_interrupt_time = current_time;

             data = 0x01; // Declarar una variable de tipo uint8_t con el dato a enviar

           					 // Enviar datos al ESP32
		 if (HAL_I2C_Master_Transmit(&hi2c1, ESP32_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
			  // Manejar el error
			  Error_Handler();
		  }


		 	//   Enviar datos al STM32 esclavo
		  if (HAL_I2C_Master_Transmit(&hi2c1, STM32_SLAVE_ADDRESS << 1, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
			  // Manejar el error
			  Error_Handler();
		  }

        }
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


	// Patterns
	for(int i = 0; i < NUMBER_OF_LEDS; i++){
		led_pattern[i].b = (i%2) * 255;
	}

	// Some pattern
	led_pattern[0].b = 127;
	led_pattern[0].r = (50);
	led_pattern[0].g = 0;
	led_pattern[1].b = 255;
	led_pattern[1].r = (100);
	led_pattern[1].g = 0;
	led_pattern[2].b = 0;
	led_pattern[2].r = (150);
	led_pattern[2].g = 127;
	led_pattern[3].b = 0;
	led_pattern[3].r = (150);
	led_pattern[3].g = 255;

	// Setting the pattern
	set_pattern_led(circular_pattern_led, led_pattern, NUMBER_OF_LEDS);
	set_pattern_led(circular_pattern_led + NUMBER_OF_LEDS, led_pattern, NUMBER_OF_LEDS);

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
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */


    reset_all_leds(leds, NUMBER_OF_LEDS); // Apagar todos los LEDs
    set_specific_led(leds, 0, &green_color); // Establecer LEDs en verde
    set_specific_led(leds, 1, &green_color);
    set_specific_led(leds, 2, &green_color);
    set_specific_led(leds, 3, &green_color);
    set_specific_led(leds, 4, &green_color);
    set_specific_led(leds, 5, &green_color);
    set_specific_led(leds, 6, &green_color);
    set_specific_led(leds, 7, &green_color);
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs


    HAL_Delay(3000);  //Retardo para estaperar a que el sistema se inicie bien

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(activador == 1|| activador1 == 0){
		  activador1 = 1;

		  	 if(espacios_disponibles <= 0 ){
		 		  display0();
		 		 reset_all_leds(leds, NUMBER_OF_LEDS); // Apagar todos los LEDs
		 		    set_specific_led(leds, 0, &red_color); // Establecer LED en rojo
		 		    set_specific_led(leds, 1, &red_color);
		 		    set_specific_led(leds, 2, &red_color);
		 		    set_specific_led(leds, 3, &red_color);
		 		    set_specific_led(leds, 4, &red_color);
		 		    set_specific_led(leds, 5, &red_color);
		 		    set_specific_led(leds, 6, &red_color);
		 		    set_specific_led(leds, 7, &red_color);
		 		    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs


		 		  espacios_disponibles = 0;
		 	  }
		 	  else if(espacios_disponibles == 1){
		 		  display1();

		 	  }
		 	  else if(espacios_disponibles == 2){
		 	 		  display2();


}
		 	  else if(espacios_disponibles == 3){
		 	 		  display3();


		 	  }
		 	  else if(espacios_disponibles == 4){
		 		  display4();

		 	  }

		 	  else if(espacios_disponibles == 5){
		 	 		  display5();


		 	 }
		 	  else if(espacios_disponibles == 6){
		 	 		  display6();


		 	  }
		 	  else if(espacios_disponibles == 7){
		 		  display7();

		 	  }

		 	  else if(espacios_disponibles >= 8){
		 	 		  display8();
		 	 		  reset_all_leds(leds, NUMBER_OF_LEDS); // Apagar todos los LEDs
		 	 		    set_specific_led(leds, 0, &green_color);
		 	 		    set_specific_led(leds, 1, &green_color);
		 	 		    set_specific_led(leds, 2, &green_color);
		 	 		    set_specific_led(leds, 3, &green_color);
		 	 		    set_specific_led(leds, 4, &green_color);
		 	 		    set_specific_led(leds, 5, &green_color);
		 	 		    set_specific_led(leds, 6, &green_color);
		 	 		    set_specific_led(leds, 7, &green_color);
		 	 		    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)leds, NUMBER_OF_LEDS * 24 + 24); // Enviar datos a los LEDs



		 	 		 espacios_disponibles = 8;
		 	 }

		  	 activador = 0;

	  }









  HAL_Delay(10); // Esperar 10ms


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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;  // Velocidad de reloj
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0; // Dirección del maestro (no utilizada en modo maestro)
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 3;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 24;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, D_C_Pin|D_D_Pin|D_E_Pin|D_A_Pin
                          |D_B_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D_F_Pin|D_G_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : sensor1_Pin sensor2_Pin */
  GPIO_InitStruct.Pin = sensor1_Pin|sensor2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : sensor4_Pin sensor6_Pin sensor8_Pin sensor5_Pin
                           sensor3_Pin */
  GPIO_InitStruct.Pin = sensor4_Pin|sensor6_Pin|sensor8_Pin|sensor5_Pin
                          |sensor3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : D_C_Pin D_D_Pin D_E_Pin D_A_Pin
                           D_B_Pin */
  GPIO_InitStruct.Pin = D_C_Pin|D_D_Pin|D_E_Pin|D_A_Pin
                          |D_B_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : D_F_Pin D_G_Pin */
  GPIO_InitStruct.Pin = D_F_Pin|D_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : sensor7_Pin */
  GPIO_InitStruct.Pin = sensor7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(sensor7_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void setSegment(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e, uint8_t f, uint8_t g) {
    HAL_GPIO_WritePin(D_A_GPIO_Port, D_A_Pin, a);
    HAL_GPIO_WritePin(D_B_GPIO_Port, D_B_Pin, b);
    HAL_GPIO_WritePin(D_C_GPIO_Port, D_C_Pin, c);
    HAL_GPIO_WritePin(D_D_GPIO_Port, D_D_Pin, d);
    HAL_GPIO_WritePin(D_E_GPIO_Port, D_E_Pin, e);
    HAL_GPIO_WritePin(D_F_GPIO_Port, D_F_Pin, f);
    HAL_GPIO_WritePin(D_G_GPIO_Port, D_G_Pin, g);
}


void display0() {
    setSegment(1, 1, 1, 1, 1, 1, 0); // 0
}

void display1() {
    setSegment(0, 1, 1, 0, 0, 0, 0); // 1
}

void display2() {
    setSegment(1, 1, 0, 1, 1, 0, 1); // 2
}

void display3() {
    setSegment(1, 1, 1, 1, 0, 0, 1); // 3
}

void display4() {
    setSegment(0, 1, 1, 0, 0, 1, 1); // 4
}

void display5() {
    setSegment(1, 0, 1, 1, 0, 1, 1); // 5
}

void display6() {
    setSegment(1, 0, 1, 1, 1, 1, 1); // 6
}

void display7() {
    setSegment(1, 1, 1, 0, 0, 0, 0); // 7
}

void display8() {
    setSegment(1, 1, 1, 1, 1, 1, 1); // 8
}

void display9() {
    setSegment(1, 1, 1, 1, 0, 1, 1); // 9
}

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
