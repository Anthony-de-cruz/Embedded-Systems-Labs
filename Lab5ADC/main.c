#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "GLCD_Brightness.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#define wait_delay HAL_Delay

#include <math.h>

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

#ifdef __RTX
extern uint32_t os_time;
uint32_t HAL_GetTick(void) {
	return os_time;
}
#endif

/**
This code is not runable before you have finished the configuration
**/


/**
* System Clock Configuration
*/

void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();
	/* The voltage scaling allows optimizing the power
	consumption when the device is clocked below the
	maximum system frequency. */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/* Enable HSE Oscillator and activate PLL
	with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
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



// init ADC configuration 
ADC_HandleTypeDef hadc;
static void MX_ADC_Init(void)
{
    ADC_ChannelConfTypeDef sConfig;
 
	/* Enable ADC CLOCK */
	//__HAL_RCC_ADC1_CLK_ENABLE();
	//__HAL_RCC_ADC2_CLK_ENABLE();
	//__HAL_RCC_ADC3_CLK_ENABLE();

	/* Configure the global features of the ADC (Clock, Resolution, Data Alignment and number
	of conversion) */		
	hadc.Instance = ADC3; //# Select the ADC (ADC1, ADC2, ADC3)
	hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc.Init.Resolution = ADC_RESOLUTION_10B;
	hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc.Init.NbrOfConversion = 1;
    hadc.Init.ScanConvMode = ENABLE;
	hadc.Init.ContinuousConvMode = ENABLE;
	hadc.Init.DiscontinuousConvMode = DISABLE;
	HAL_ADC_Init(&hadc);
	//configure channal
	sConfig.Rank = 1;
	sConfig.Channel = 0;//# Select the ADC Channel (ADC_CHANNEL_X)
	sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	HAL_ADC_ConfigChannel(&hadc, &sConfig);
	HAL_ADC_Start(&hadc);
	HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
}

// init GPIO for analog 
GPIO_InitTypeDef GPIO_InitStruct;
static void MX_GPIO_Init(void){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG; // configure to analog input mode
    GPIO_InitStruct.Pin = GPIO_PIN_1;// #select GPIO GPIO_PIN_X
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);// #select GPIO Group
}
	
void init_config(void){
	HAL_Init(); //Init Hardware Abstraction Layer
	SystemClock_Config(); //Config Clocks
	MX_GPIO_Init();
	MX_ADC_Init();
	GLCD_Initialize(); //Init GLCD
	EnableBrightnessControl();// Enable the control of LCD brightness from GLCD_Brightness.c
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
	
}

// ctrl LCD brightness
void ctrl_brightness (float adc_read){
	float V_pwr=5;//configure the V_pwr
    float V_sense=adc_read; //calculate the V_sense
    float brightness=V_sense/V_pwr; // calculate the ratio of brightness
    char buffer[50];		
    sprintf(buffer, "V reading: %.2f", V_sense);
    GLCD_DrawString(0, 48, buffer);
    if(brightness >0.5)	{
		GLCD_DrawString(0, 24, "Brightness: LCD_LIGHT         ");
		UpdateBrightness(LCD_LIGHT);
	}
	else {
		GLCD_DrawString(0, 24, "Brightness: LCD_OFF           ");
		UpdateBrightness(LCD_OFF);
	}
}
// display temperature
void display_temp(float temp){
	char msg[100];
	sprintf(msg, "core temp.: %.2f deg C   ", temp);
	GLCD_DrawString(0, 72, msg);
}

int main(void){
	init_config();
	while(1){
		uint16_t adc_read;
		
		// read ADC value 
		adc_read = HAL_ADC_GetValue(&hadc);
		ctrl_brightness(adc_read);
		wait_delay(1);
	}
}
