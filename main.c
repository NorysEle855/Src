#include "main.h"
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);

// Declaracion de variables
volatile int count = 0;
volatile uint32_t valor_adcA0 = 0;
volatile int porcentaje_adc = 0;
volatile uint32_t valor_adcA1 = 0;
volatile int porcentaje_control = 0;


// Declaracion de funciones
void casoA(void);
void casoB(void);
void casoC(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();

  while (1)
  {
    ADC_ChannelConfTypeDef sConfig = {0};
    float temp_porcentaje;

    // --- LEER CANAL 0 (NIVEL en PA0) ---
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
      valor_adcA0 = HAL_ADC_GetValue(&hadc1);
      temp_porcentaje = (0.0259f * valor_adcA0) - 5.8717f;
      if (temp_porcentaje < 0.0f) temp_porcentaje = 0.0f;
      if (temp_porcentaje > 100.0f) temp_porcentaje = 100.0f;
      porcentaje_adc = (int)temp_porcentaje;
    }
    HAL_ADC_Stop(&hadc1);

    // --- LEER CANAL 1 (CONTROL en PA1) ---
    sConfig.Channel = ADC_CHANNEL_1; 
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK) {
      valor_adcA1 = HAL_ADC_GetValue(&hadc1);
      temp_porcentaje = (0.0259f * valor_adcA1) - 4.8717f;
      if (temp_porcentaje < 0.0f) temp_porcentaje = 0.0f;
      if (temp_porcentaje > 100.0f) temp_porcentaje = 100.0f;
      porcentaje_control = (int)temp_porcentaje;
    }
    HAL_ADC_Stop(&hadc1);

    // --- LÓGICA DE ESTADOS ---
    if (count == 1) casoA();
    else if (count == 2) casoB();
    else if (count == 3) casoC();
    else count = 1;

    HAL_Delay(10);
  }
  {
    HAL_ADC_Start(&hadc1);
    
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
      valor_adcA0 = HAL_ADC_GetValue(&hadc1);
      float temp_porcentaje = (0.0259f * valor_adcA0) - 5.8717f;

      if (temp_porcentaje < 0.0f) temp_porcentaje = 0.0f;
      else if (temp_porcentaje > 100.0f) temp_porcentaje = 100.0f;

      porcentaje_adc = (int)temp_porcentaje;
    }
    HAL_ADC_Stop(&hadc1);
    // La evaluación de casos va AQUÍ ADENTRO, así solo se ejecuta una vez por clic
      if (count == 1)
      {
        casoA();
      }
      else if (count == 2)
      {
        casoB();
      }
      else if (count == 3)
      {
        casoC();
      }
      else
      {
        count = 1;
        casoA();
      }
  }
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

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_DMA_Init(void)
{
  __HAL_RCC_DMA2_CLK_ENABLE();

  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_15, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_SET);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0); // Prioridad 0 (la más alta)
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t ultimo_tiempo_presionado = 0;
  uint32_t tiempo_actual = HAL_GetTick();

  if (GPIO_Pin == GPIO_PIN_2)
  {
    // Solo entramos si han pasado más de 200 ms
    if ((tiempo_actual - ultimo_tiempo_presionado) > 200)
    {
      count++;

      

      ultimo_tiempo_presionado = tiempo_actual;
    }
  }
}

void casoA(void)
{
  // 1. APAGAR SISTEMAS INACTIVOS (Variables de control y Alarma)
  // Apagar LED de Alarma (PA3)
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  
  // Apagar LEDs Blancos (Puerto A: PA10, PA15)
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10 | GPIO_PIN_15, GPIO_PIN_RESET);
  
  // Apagar LEDs Blancos (Puerto B: PB3, PB4, PB5, PB6, PB7, PB8, PB9)
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | 
                           GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);
                           
  // Apagar LED Blanco (Puerto C: PC13)
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);


  // 2. APAGAR SEMÁFORO PRINCIPAL PARA REFRESCAR EL ESTADO
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | 
                           GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);


  // 3. ENCENDER SECUENCIALMENTE SEGÚN EL PORCENTAJE (Modo Barra)
  // Leds Rojos (0% - 40%)
  if (porcentaje_adc >= 10) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,  GPIO_PIN_SET);
  if (porcentaje_adc >= 20) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,  GPIO_PIN_SET);
  if (porcentaje_adc >= 30) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,  GPIO_PIN_SET);
  if (porcentaje_adc >= 40) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

  // Leds Amarillos (41% - 70%)
  if (porcentaje_adc >= 50) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  if (porcentaje_adc >= 60) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
  if (porcentaje_adc >= 70) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);

  // Leds Verdes (71% - 100%)
  if (porcentaje_adc >= 80) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
  if (porcentaje_adc >= 90) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  GPIO_PIN_SET);
  if (porcentaje_adc >= 100) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}

void casoB(void)
{
  // 1. SEMÁFORO DE NIVEL (Igual que en Modo A)
  // Apagamos primero
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | 
                           GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);

  // Encendemos según el Nivel
  if (porcentaje_adc >= 10) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,  GPIO_PIN_SET);
  if (porcentaje_adc >= 20) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,  GPIO_PIN_SET);
  if (porcentaje_adc >= 30) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,  GPIO_PIN_SET);
  if (porcentaje_adc >= 40) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
  if (porcentaje_adc >= 50) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
  if (porcentaje_adc >= 60) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
  if (porcentaje_adc >= 70) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
  if (porcentaje_adc >= 80) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
  if (porcentaje_adc >= 90) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8,  GPIO_PIN_SET);
  if (porcentaje_adc >= 100) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);


  // 2. SEMÁFORO DE CONTROL (10 LEDs Blancos)
  // Apagamos primero
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | 
                           GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  // Encendemos según el Control
  if (porcentaje_control >= 10) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET);
  if (porcentaje_control >= 20) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
  if (porcentaje_control >= 30) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,  GPIO_PIN_SET);
  if (porcentaje_control >= 40) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,  GPIO_PIN_SET);
  if (porcentaje_control >= 50) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5,  GPIO_PIN_SET);
  if (porcentaje_control >= 60) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,  GPIO_PIN_SET);
  if (porcentaje_control >= 70) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,  GPIO_PIN_SET);
  if (porcentaje_control >= 80) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,  GPIO_PIN_SET);
  if (porcentaje_control >= 90) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,  GPIO_PIN_SET);
  if (porcentaje_control >= 100) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);


  // 3. LED DE ALARMA (PA3) CON CICLO PROPORCIONAL
  if (porcentaje_adc > porcentaje_control) 
  {
    int diferencia = porcentaje_adc - porcentaje_control;
    
    // El ciclo total es 2000 ms (2 segundos).
    // Si la diferencia es 100%, el tiempo encendido es 2000ms. Si es 50%, es 1000ms.
    // Fórmula: tiempo_encendido = (diferencia * 2000) / 100 => diferencia * 20
    uint32_t tiempo_encendido_ms = diferencia * 20; 
    
    // Obtenemos el tiempo actual de la placa y lo dividimos en bloques de 2000ms
    uint32_t tiempo_dentro_del_ciclo = HAL_GetTick() % 2000; 

    // Si estamos dentro del tiempo de encendido, prendemos. Si nos pasamos, apagamos.
    if (tiempo_dentro_del_ciclo < tiempo_encendido_ms) {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
    } else {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    }
  } 
  else 
  {
    // Si Nivel NO supera a Control, la alarma permanece apagada
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
  }
}

void casoC(void)
{
  // Aquí irá tu código para el Caso C
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */