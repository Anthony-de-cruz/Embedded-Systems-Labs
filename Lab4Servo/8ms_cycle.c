#include "stm32f7xx_hal.h"

TIM_HandleTypeDef htim2;

#ifdef __RTX
extern uint32_t os_time;
uint32_t HAL_GetTick(void){
	return os_time;
}
#endif

void SysTick_Handler(void){
  HAL_IncTick();
}

/**
Pin PA15 configuration (TIM2_CH1) and initialization.
The only difference between the traditional GPIO initialization is the use of alternate function:
1) We need to specify the mode (gpio.Mode) as the alternate;
2) We need to map the right timer (or other peripheral) to the pin (gpio.Alternate).
**/
void PA15_Init(){
	GPIO_InitTypeDef gpio;

	// Enable the clock for the base A
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio.Pin = GPIO_PIN_15;
	gpio.Mode = GPIO_MODE_AF_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	gpio.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &gpio);
}

/**
System clock configuration.
We will use this one for now, however, we will replace it 
with the old one (the one we used for GLCD) soon.
**/
void SystemClock_Config(void){
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	/* The voltage scaling allows optimizing the power
	consumption when the device is clocked below the
	maximum system frequency. */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* Enable HSI Oscillator and activate PLL
	with HSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSEState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	/* Select PLL as system clock source and configure
	the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | 
	RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
Timer (TIM2) and Channel (CH1) initialization. 
**/
static void TIM2_Init(void){
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_OC_InitTypeDef sConfigOC;

	//Timer configuration
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 64000;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	//htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; // Requires library V2.2
	HAL_TIM_Base_Init(&htim2);

	//Set the timer in PWM mode
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim2);

	//Channel configuration
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

	PA15_Init();
}

int main(void){
	//Reset of all peripherals, initializes the Flash interface and the Systick
	HAL_Init();

	//Enable peripheral clock for TIM2
	__HAL_RCC_TIM2_CLK_ENABLE();

	//Configure the system clock
	SystemClock_Config();

	//Initialize TIM2, CH1 and PA15
	TIM2_Init();

	//Start PWM on TIM2_CH1 (PA15 a.k.a. D9)
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	//Set the compare register to 1
	htim2.Instance->CCR1 = 1;
}
