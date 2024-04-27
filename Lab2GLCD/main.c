#include <stdio.h>
#include <stdlib.h>
#include "stm32f7xx_hal.h"
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#define wait_delay HAL_Delay
extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;

/**
* The code from snippet.c goes here
**/

#ifdef __RTX
extern uint32_t os_time;
uint32_t HAL_GetTick(void) {
	return os_time;
}
#endif

void drawDigitBox(
    int x, int y,
    int width, int height, 
    int font_width, int font_height,
    int digit
);
    
typedef struct TouchBox {
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
} TouchBox;

bool checkTouchBox(TouchBox *box, TOUCH_STATE *tsc_state);
    
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


int main(void){
    unsigned int digit;
    char debug_buffer[128];
    
    TouchBox touch_box = {
        50,
        50,
        50,
        50,
    };
    
    int touched;
    
    TOUCH_STATE tsc_state;
    HAL_Init(); //Init Hardware Abstraction Layer
    SystemClock_Config(); //Config Clocks
    Touch_Initialize(); // Init Touchscreen
    GLCD_Initialize(); //Init GLCD
    GLCD_ClearScreen();
    GLCD_SetFont(&GLCD_Font_16x24);
    GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
    GLCD_SetForegroundColor(GLCD_COLOR_BLACK);

    while(1){
        
        if (Touch_GetState(&tsc_state)) {
            GLCD_DrawString(0, 1 *
                GLCD_Font_16x24.width, "ERR: Touch_GetState");
            exit(1);
        }

        
        if (tsc_state.pressed && checkTouchBox(&touch_box, &tsc_state)) {
            touched = 1;
        } else {
            touched = 0;
        };
        
        if (tsc_state.pressed) {
            sprintf(debug_buffer, "X:%i,Y:%i,touched:%i       ", tsc_state.x, tsc_state.y, touched);
        }
        
        GLCD_SetFont(&GLCD_Font_6x8);
        GLCD_DrawString(GLCD_Font_16x24.width, GLCD_Font_16x24.height, debug_buffer);
        GLCD_SetFont(&GLCD_Font_16x24);
        

        GLCD_DrawRectangle(touch_box.x, touch_box.y, touch_box.width, touch_box.height);

        
        for(digit = 0; digit < 4; digit++) {
            
            drawDigitBox(
                (int)GLCD_SIZE_X / 5 + digit * GLCD_Font_16x24.width * 5,
                GLCD_Font_16x24.height * 5,
                GLCD_Font_16x24.width * 3,
                GLCD_Font_16x24.width * 3,
                16,
                24,
                48 + digit
            );
        }
        wait_delay(100);
        //GLCD_ClearScreen();
    }
}


void drawDigitBox(
    int x, int y,
    int width, int height, 
    int font_width, int font_height,
    int digit
) {
    GLCD_DrawRectangle(x, y, width, height);
    GLCD_DrawChar(
        (int)x + (width / 2) - (font_width / 2),
        (int)y + (height / 2) - (font_height / 2),
        digit);
}

bool checkTouchBox(TouchBox *box, TOUCH_STATE *tsc_state) {
    if (tsc_state->x > box->x && tsc_state->x < box->x + box->width) {
        if (tsc_state->y > box->y && tsc_state->y < box->y + box->height) {
            return 1;
        }
    }
    return 0;
};
