/* USER CODE BEGIN Header */
/**
   ******************************************************************************
 Universidad Del Valle De Guatemala
 IE2023: Electrónica digital 2
 Autor: Samuel Tortola - 22094, Alan Gomez - 22115
 Proyecto: Proyecto servidor WEB
 Hardware: STM32
 Creado: 31/10/2024
Última modificación: 11/11/2024
******************************************************************************
  */
//CODIGO DEL SMT32 ESCLAVO

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "tft.h"// Libreria TFT
#include "functions.h"
#include "user_setting.h"
#include "bitmaps.h"
#include "stm32f4xx_hal.h"
#include "stdlib.h"
#include <stdbool.h>

#include "bitmaps.h" // Arreglos para imprimir en tft
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint16_t ID=0; // Variable de libreria de la TFT

uint8_t Number_Display = 8; // Contador de display
uint8_t Recived_Car = 0;

const uint16_t* carArrays[] = { CAR_M, CAR_A, CAR_N, CAR_R};// Seleccion de carro
const uint16_t* carArrays_Dis[] = { DIS_0, DIS_1, DIS_2, DIS_3, DIS_4, DIS_5, DIS_6, DIS_7, DIS_8};// Numeros para display
bool C1 = false, C2 = false, C3 = false, C4 = false, C5 = false, C6 = false, C7 = false, C8 = false;// Banderas de Carros
const uint16_t* selectedArray; // Carro seleccionado
const uint16_t* selectedArray_DIS;// Numero para display
volatile int KLE; // Posicion de Array para carros
volatile int KLA;// Posiion de Array para display

uint8_t rx_data[1]; // Solo para recibir byte por byte
uint8_t rx_buffer[100]; // Buffer para almacenar lo recibido por UART5
uint8_t RX_Buffer [8] ;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_UART5_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void delay (uint32_t time)
{
	/* change your code here for the delay in microseconds */
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while ((__HAL_TIM_GET_COUNTER(&htim1))<time);
}

int State_Dis(int KLA);// Funcion para Display en pantalla TFT
int State_Cars(int S); // Funcion para estado de carros en pantalla TFT
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM1_Init();
  MX_UART5_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim1);
	ID=readID();
	HAL_Delay(100);
	tft_init(ID);
	setRotation(1);
	fillScreen(BLACK);

	  HAL_UART_Receive_IT(&huart5,rx_data, 1);  // Empezar la recepción por UART1 en modo interrupción
	drawRGBBitmapFast(0,0,Terminal,320,240);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

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
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
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
  if (HAL_UART_Init(&huart2) != HAL_OK)
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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_RST_Pin|D1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|D7_Pin
                          |D0_Pin|D2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_CS_Pin|D6_Pin|D3_Pin|D5_Pin
                          |D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_RST_Pin D1_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RD_Pin LCD_WR_Pin LCD_RS_Pin D7_Pin
                           D0_Pin D2_Pin */
  GPIO_InitStruct.Pin = LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|D7_Pin
                          |D0_Pin|D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_CS_Pin D6_Pin D3_Pin D5_Pin
                           D4_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin|D6_Pin|D3_Pin|D5_Pin
                          |D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int State_Dis(int KLA) {
    selectedArray_DIS = carArrays_Dis[KLA];
    drawRGBBitmapFast(254, 81, selectedArray_DIS, 48, 80);

    return 0; // Retorna un valor (por ejemplo, 0 para indicar éxito)
}


int State_Cars(int S){
switch (S) {
		case 1:
			 C1 = !C1;

			 if(C1 == 1){
				 KLE = (KLE < 3) ? (KLE + 1) : 0;
				selectedArray = carArrays[KLE]; // Selecciona el arreglo
				// Llama a la función con el arreglo seleccionado
				drawRGBBitmapFast(14, 15, selectedArray,30, 50);
				drawRGBBitmapFast(12,95,mySem_RED,33,13);
				Number_Display = Number_Display - 1;
				State_Dis(Number_Display);// imprimir en display

			 }

			 else if(C1 == 0){
				 drawRGBBitmapFast(14, 15, CAR_NAC, 30, 50);
				 drawRGBBitmapFast(12,95,mySem_RED,33,13);
				 Number_Display = Number_Display + 1;
				 State_Dis(Number_Display);// imprimir en display

			 }
			break;
		case 2:
			 C2 = !C2;

			 if(C2 == 1){
				 KLE = (KLE < 3) ? (KLE + 1) : 0;
				selectedArray = carArrays[KLE]; // Selecciona el arreglo
				// Llama a la función con el arreglo seleccionado
				drawRGBBitmapFast(73, 15, selectedArray, 30, 50);
				drawRGBBitmapFast(71,95,mySem_GREEN,33,13);
				Number_Display = Number_Display - 1;
				State_Dis(Number_Display);// imprimir en display
			 }

			 else if(C2 == 0){
				 drawRGBBitmapFast(73, 15, CAR_NAC, 30, 50);
				 drawRGBBitmapFast(71,95,mySem_RED,33,13);
				 Number_Display = Number_Display + 1;
				 State_Dis(Number_Display);// imprimir en display

			 }
			break;
		case 3:
			 C3 = !C3;

			 if(C3 == 1){
				 KLE = (KLE == 0) ? (KLE - 1) : 3;
				selectedArray = carArrays[KLE]; // Selecciona el arreglo
				// Llama a la función con el arreglo seleccionado
				drawRGBBitmapFast(132, 15, selectedArray, 30, 50);
				drawRGBBitmapFast(130,95,mySem_GREEN,33,13);
				Number_Display = Number_Display - 1;
				State_Dis(Number_Display);// imprimir en display
			 }

			 else if(C3 == 0){
				 drawRGBBitmapFast(132, 15, CAR_NAC, 30, 50);
				 drawRGBBitmapFast(130,95,mySem_RED,33,13);
				 Number_Display = Number_Display + 1;
				 State_Dis(Number_Display);// imprimir en display
			 }
			break;
		case 4:
			 C4 = !C4;

			 if(C4 == 1){
				 KLE = (KLE < 3) ? (KLE + 1) : 0;
				selectedArray = carArrays[KLE]; // Selecciona el arreglo
				// Llama a la función con el arreglo seleccionado
				drawRGBBitmapFast(191, 15, selectedArray, 30, 50);
				drawRGBBitmapFast(189, 95,mySem_GREEN,33,13);
				Number_Display = Number_Display - 1;
				State_Dis(Number_Display);// imprimir en display
			 }

			 else if(C4 == 0){
				 drawRGBBitmapFast(191, 15, CAR_NAC, 30, 50);
				 drawRGBBitmapFast(189,95,mySem_RED,33,13);
				 Number_Display = Number_Display + 1;
				 State_Dis(Number_Display);// imprimir en display
			 }
			break;


		case 5:
			 C5 = !C5;

			 if(C5 == 1){
				 KLE = (KLE < 3) ? (KLE + 1) : 0;
				selectedArray = carArrays[KLE]; // Selecciona el arreglo
				// Llama a la función con el arreglo seleccionado
				drawRGBBitmapFast(14, 176, selectedArray, 30, 50);
				drawRGBBitmapFast(12,134,mySem_GREEN,33,13);
				Number_Display = Number_Display - 1;
				State_Dis(Number_Display);// imprimir en display
			 }

			 else if(C5 == 0){
				 drawRGBBitmapFast(14, 176, CAR_NAC, 30, 50);
				 drawRGBBitmapFast(12,134,mySem_RED,33,13);
				 Number_Display = Number_Display + 1;
				 State_Dis(Number_Display);// imprimir en display
			 }
			 break;
		case 6:
			 C6 = !C6;

			 if(C6 == 1){
				 KLE = (KLE < 3) ? (KLE + 1) : 0;
				selectedArray = carArrays[KLE]; // Selecciona el arreglo
				// Llama a la función con el arreglo seleccionado
				drawRGBBitmapFast(73, 176, selectedArray, 30, 50);
				drawRGBBitmapFast(71,134,mySem_GREEN,33,13);
				Number_Display = Number_Display - 1;
				State_Dis(Number_Display);// imprimir en display
			 }

			 else if(C6 == 0){
				 drawRGBBitmapFast(73, 176, CAR_NAC, 30, 50);
				 drawRGBBitmapFast(71,134,mySem_RED,33,13);
				 Number_Display = Number_Display + 1;
				 State_Dis(Number_Display);// imprimir en display
			 }
			 break;
		case 7:
			 C7 = !C7;

			 if(C7 == 1){
				 KLE = (KLE < 3) ? (KLE + 1) : 0;
				selectedArray = carArrays[KLE]; // Selecciona el arreglo
				// Llama a la función con el arreglo seleccionado
				drawRGBBitmapFast(132, 176, selectedArray, 30, 50);
				drawRGBBitmapFast(130,134,mySem_GREEN,33,13);
				Number_Display = Number_Display - 1;
				State_Dis(Number_Display);// imprimir en display
			 }

			 else if(C7 == 0){
				 drawRGBBitmapFast(132, 176, CAR_NAC, 30, 50);
				 drawRGBBitmapFast(130,134,mySem_RED,33,13);
				 Number_Display = Number_Display + 1;
				 State_Dis(Number_Display);// imprimir en display
			 }
			 break;
		case 8:
			 C8 = !C8;

			 if(C8 == 1){
				// Llama a la función con el arreglo seleccionado
				drawRGBBitmapFast(191, 176, CAR_N, 30, 50);
				drawRGBBitmapFast(189, 134,mySem_GREEN,33,13);
				Number_Display = Number_Display - 1;
				State_Dis(Number_Display);// imprimir en display
			 }

			 else if(C8 == 0){
				 drawRGBBitmapFast(191, 176, CAR_NAC, 30, 50);
				 drawRGBBitmapFast(189,134,mySem_RED,33,13);
				 Number_Display = Number_Display + 1;
				 State_Dis(Number_Display);// imprimir en display
			 }
			 break;

		default:

			break;
	}

return 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == UART5) // Verificar si la interrupción es de UART1
  {
	  Recived_Car = rx_data[0] - '0';
	  State_Cars(Recived_Car);// Recivir numero de carro

	  // Volver a habilitar la recepción por UART5
	      HAL_UART_Receive_IT(&huart5, rx_data, 1);

  }
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
