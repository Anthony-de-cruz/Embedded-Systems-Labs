#include "main.h"

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


void init_GLCD(void){
	GLCD_Initialize(); //Init GLCD
	GLCD_ClearScreen();
	GLCD_SetFont(&GLCD_Font_16x24);
	GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
	GLCD_SetForegroundColor(GLCD_COLOR_WHITE);
	GLCD_DrawString (0 , PROJECT* 24+2, "RSA TEST RX" ) ;
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
}
void inline __attribute__((always_inline)) delayms(uint64_t delay)
{
	delay*=10000;
   while(delay--) __asm("");
}

void init_uart(void){
    //these code is for an STMF4 board, you need to check the datasheet and modify the configuations
    //hints: there should be 8-9 modifications
    /**
        More hints:
        1. check you have enabled correct clocks
        2. check you have configured correct GPIOs
        3. check you have selected correct Alternate Function
        4. check you have selected correct UART/USART Channel        
        5. check you have configured a same baudrate between 2 board        
    **/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__USART2_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Alternate = GPIO_AF7_USART2;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	s_UARTHandle.Instance        = USART2;
	s_UARTHandle.Init.BaudRate   = 9600;
	s_UARTHandle.Init.WordLength = UART_WORDLENGTH_8B;
	s_UARTHandle.Init.StopBits   = UART_STOPBITS_1;
	s_UARTHandle.Init.Parity     = UART_PARITY_NONE;
	s_UARTHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	s_UARTHandle.Init.Mode       = UART_MODE_TX_RX;
	HAL_UART_Init(&s_UARTHandle);
}

int main(void){
	HAL_Init(); //Init Hardware Abstraction Layer
	SystemClock_Config(); //Config Clocks
	init_GLCD();
	init_uart();// Init UART Port
	int index=0;
	
	for (;;)
	{	char rec;
		HAL_UART_Receive(&s_UARTHandle,&rec,1, HAL_MAX_DELAY);// receive 1 char
		rx_buffer[index++]=rec;
       
		if(rec=='\0'){ //receive end char
			index=0;
            GLCD_DrawString (0 , (ENCRY*2)* 24+2, "raw message:");
			strncpy(displayBuff,rx_buffer,sizeof(displayBuff));
            GLCD_DrawString (0 , (ENCRY*2+1)* 24+2, displayBuff);
			
            /* Decryption code here*/
					
		}

	}
}