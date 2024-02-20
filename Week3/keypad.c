#include "keypad.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"

void initializeMembranePins (void){
	GPIO_InitTypeDef gpio; 
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();  
	__HAL_RCC_GPIOB_CLK_ENABLE(); 

	gpio.Mode = GPIO_MODE_OUTPUT_PP; 
	gpio.Pull = GPIO_PULLDOWN; 
	gpio.Speed = GPIO_SPEED_HIGH; 
	
	// D8
	gpio.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOI, &gpio);
	
	// D9
	gpio.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOA, &gpio);
	
	// D10
	gpio.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &gpio);
	
	// D11
	gpio.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB, &gpio);
	
	// D12
	gpio.Pin = GPIO_PIN_14;
	HAL_GPIO_Init(GPIOB, &gpio);
	
	// D13
	gpio.Pin = GPIO_PIN_1;
	HAL_GPIO_Init(GPIOI, &gpio);
	
	// D14
	gpio.Pin = GPIO_PIN_9;
	HAL_GPIO_Init(GPIOB, &gpio);
	
	// D15
	gpio.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOB, &gpio);
}

int convertPinsToNum(int k, int r){
	if(k == 11 && r == 15){
			return 1;
	}
	if(k == 10 && r == 15){
			return 2;
	}
	if(k == 9 && r == 15){
			return 3;
	}
	if(k == 8 && r == 15){
			return 11; //A
	}
	if(k == 11 & r == 14){
		return 4;
	}
	if(k == 10 & r == 14){
		return 5;
	}
	if(k == 9 & r == 14){
		return 6;
	}
	if(k == 8 & r == 14){
		return 12; //B
	}
	if(k == 11 & r == 13){
		return 7;
	}
	if(k == 10 & r == 13){
		return 8;
	}
	if(k == 9 & r == 13){
		return 9;
	}
	if(k == 8 & r == 13){
		return 13; //C
	}
	if(k == 11 & r == 12){
		return 15; //*
	}
	if(k == 10 & r == 12){
		return 0;
	}
	if(k == 9 & r == 12){
		return 16; //#
	}
	if(k == 8 & r == 12){
		return 14; //D
	}
	
	return 0;
}

void turnOn(int pinNo){
	switch (pinNo){
		case 8:
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_2, GPIO_PIN_RESET);
			break;
		case 9:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
			break;
		case 10:
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
			break;
		case 11:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
			break;
		case 12:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
			break;
		case 13:
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_RESET);
			break;
		case 14:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);
			break;
		case 15:
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
			break;		
	}
}

void turnOff(int pinNo){
		switch (pinNo){
			case 8:
				HAL_GPIO_WritePin(GPIOI, GPIO_PIN_2, GPIO_PIN_SET);
				break;
			case 9:
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
				break;
			case 10:
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
				break;
			case 11:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
				break;
			case 12:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
				break;
			case 13:
				HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_SET);
				break;
			case 14:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
				break;
			case 15:
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
				break;
	}
}

GPIO_PinState readPin(int number){
	switch(number){
		case 8:
			return HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_2);
		case 9:
			return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
		case 10:
			return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_8);
		case 11:
			return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15);	
		case 12:
			return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
		case 13:
			return HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_1);
		case 14:
			return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);
		case 15:
			return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);	
		default:
			return GPIO_PIN_RESET;
	}
}

void setColsIn(){
	//set columns to write
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_INPUT;
	gpio.Pull = GPIO_PULLDOWN; //
	gpio.Speed = GPIO_SPEED_HIGH; //
	// D8
	gpio.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOI, &gpio);
	
	// D9
	gpio.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOA, &gpio);
	
	// D10
	gpio.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &gpio);
	
	// D11
	gpio.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB, &gpio);
}

void setColsOut(){
	//set columns to write
	GPIO_InitTypeDef gpio;
	gpio.Mode = GPIO_MODE_OUTPUT_PP;
	gpio.Pull = GPIO_PULLDOWN; //
	gpio.Speed = GPIO_SPEED_HIGH; //
	// D8
	gpio.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOI, &gpio);
	
	// D9
	gpio.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOA, &gpio);
	
	// D10
	gpio.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOA, &gpio);
	
	// D11
	gpio.Pin = GPIO_PIN_15;
	HAL_GPIO_Init(GPIOB, &gpio);
}

int getInput (void){
	unsigned int k, r;
	
	//set rows high one at a time
	for (r = 12; r <= 15; r++){
		
			//set columns to low
			setColsOut();
			for (k = 8; k<=11; k++){
				turnOn(k);
			}
			setColsIn();
		
			turnOff(r);

			// check the value of each column
			for (k = 8; k <= 11; k++){
					if(readPin(k) == GPIO_PIN_SET){
						turnOn(r);
						return convertPinsToNum(k, r);
					}
			}
			turnOn(r);
	}
	
	return -1;
}
