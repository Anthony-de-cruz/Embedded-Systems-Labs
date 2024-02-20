#include <stdio.h>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"

typedef struct {
	GPIO_InitTypeDef init;
	GPIO_TypeDef *base;
} GPIO_PIN;

void initializePins(void);

void resetPins(void);

void dispNum(int number);

void wait(int delay);

int main(void){
	
	unsigned int i;
	
	initializePins();

	while (1) {
		for(i = 0; i <= 9; i++) {
			resetPins();
			dispNum(i);
			wait(400000); // <- Do not trust
		}
	}
}

void initializePins(void) {
	
	GPIO_PIN pins[8];
	int x;
	
		// enable clock for C, B, H, I, G base
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	// PC7, D0
	pins[0].init.Pin = GPIO_PIN_7;
	pins[0].base = GPIOC;
	
	// PC6, D1
	pins[1].init.Pin = GPIO_PIN_6;
	pins[1].base = GPIOC;
	
	// PG6, D2
	pins[2].init.Pin = GPIO_PIN_6;
	pins[2].base = GPIOG;
	
	// PB4, D3
	pins[3].init.Pin = GPIO_PIN_4;
	pins[3].base = GPIOB;
	
	// PG7, D4
	pins[4].init.Pin = GPIO_PIN_7;
	pins[4].base = GPIOG;
	
	// PI0, D5
	pins[5].init.Pin = GPIO_PIN_0;
	pins[5].base = GPIOI;
	
	// PH6, D6
	pins[6].init.Pin = GPIO_PIN_6;
	pins[6].base = GPIOH;
	
	// PI3, D7
	pins[7].init.Pin = GPIO_PIN_3;
	pins[7].base = GPIOI;

	for (x = 0; x < 8; x++) {
		// set mode as output, nopull
		pins[x].init.Mode = GPIO_MODE_OUTPUT_PP;
		pins[x].init.Pull = GPIO_NOPULL;
		pins[x].init.Speed = GPIO_SPEED_HIGH;
		
		// set mode as output, nopull
		HAL_GPIO_Init(pins[x].base, &pins[x].init);
		// enable the segment
		//HAL_GPIO_WritePin(pins[x].base, pins[x].init.Pin, GPIO_PIN_SET);
	}
}

// D0 - A
// D1 - B
// D2 - C
// D3 - D
// D4 - E
// D5 - F
// D6 - G
void dispNum(int number) {
	switch (number) {
		case 0:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // B
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET); // C
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // B
			HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET); // G
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET); // E
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // D
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // B
			HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET); // G
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET); // C
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // D
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET); // F
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // B
			HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET); // G
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET); // C
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // A
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET); // F
			HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET); // G
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET); // C
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // D
			break;
		case 6:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // A
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET); // F
			HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET); // G
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET); // C
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // D
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET); // E
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // B
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET); // C
			break;
		case 8:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // B
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET); // C
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // D
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET); // E
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET); // F
			HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET); // G
			break;
		case 9:
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // A
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // B
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET); // C
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET); // D
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET); // F
			HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET); // G
			break;
	}
}

void resetPins(void) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET);
}

void wait(int delay) {
	int x;
	for(x = 5; x < delay; x++) {}
}
