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
	GLCD_DrawString (0 , PROJECT* 24+2, "RSA TEST TX" ) ;
	GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
	GLCD_SetForegroundColor(GLCD_COLOR_BLUE);
}
void inline __attribute__((always_inline)) delayms(uint64_t delay)
{
	delay*=10000;
   while(delay--) __asm("");
}

void set_keys(){
  pub->modulus = pub_mod;
  pub->exponent = pub_exp;
  priv->modulus = pri_mod;
  priv->exponent = pri_exp;
}  
uint64_t decodeBuff[MAX_CHAR];
int main(void){
	HAL_Init(); //Init Hardware Abstraction Layer
	SystemClock_Config(); //Config Clocks
	init_GLCD();
	char message[] = "Hello World from TX";
	GLCD_DrawString (0 , (MSG*2)* 24+2, "Message:" );
	GLCD_DrawString (0 , (MSG*2+1)* 24+2, message);
	//set RSA KEY
	set_keys();
    
    //encrypt
    rsa_encrypt(message, sizeof(message), pub,&encrypted_buffer);
	GLCD_DrawString (0 , (ENCRY*2)* 24+2, "Encryption:" );
		
		
	//convert the uint64_t encryption  to string
	encode(encrypted_buffer,tx_buffer,strlen(message));
	
    //show parts of the encryption
	strncpy(displayBuff,tx_buffer,sizeof(displayBuff));
	GLCD_DrawString (0 , (ENCRY*2+1)* 24+2, displayBuff);
    
	
    //convert the string to uint64_t encryption
    if(dencode(tx_buffer,decodeBuff)){
        GLCD_DrawString (0 , (DECRY*2)* 24+2, "Decryption:" );
        //decrypt
        rsa_decrypt(decodeBuff, sizeof(uint64_t)*strlen(message), priv, &decrypted_buffer);
        GLCD_DrawString (0 , (DECRY*2+1)* 24+2, decrypted_buffer);
    }	
}