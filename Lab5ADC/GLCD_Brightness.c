#include "GLCD_Brightness.h"
uint16_t Gap=1;
uint16_t HZ=100;
uint16_t counter=0;
uint16_t flag=0;

void PK3_init(){
	GPIO_InitTypeDef gpio;
	__HAL_RCC_GPIOK_CLK_ENABLE();
	gpio.Pin = GPIO_PIN_3;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_NOPULL;
	gpio.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOK, &gpio);
}

void EnableBrightnessControl(){
	__HAL_RCC_GPIOF_CLK_ENABLE();
	//MX_TIM2_Init();
	PK3_init();
};


void SetLCDBrightness_light(){
 HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_SET);
}


void SetLCDBrightness_off(){
			HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3, GPIO_PIN_RESET);
}


void UpdateBrightness(uint16_t setting){
 if(setting==0) SetLCDBrightness_off();
 else SetLCDBrightness_light();
}
