#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f7xx_hal.h"
const uint8_t APBPrescTable[8] = { 0, 0, 0, 0, 1, 2, 3, 4 };
#include "stm32746g_discovery.h"        // Keil.STM32F746G-Discovery::Board Support:Drivers:Basic I/O
#include "GLCD_Config.h"
#include "Board_GLCD.h"
#include "Board_Touch.h"
#include "rsa.h"
#define wait_delay HAL_Delay
#define PROJECT 0
#define MSG 1
#define ENCRY 2
#define DECRY 3
#define UART 4
#define MAX_CHAR 100
#ifdef __RTX
extern uint32_t os_time;
uint32_t HAL_GetTick(void) {
	return os_time;
}
#endif

extern GLCD_FONT GLCD_Font_6x8;
extern GLCD_FONT GLCD_Font_16x24;
static UART_HandleTypeDef s_UARTHandle;


void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
void init_GLCD(void);
void inline __attribute__((always_inline)) delayms(uint64_t delay);
void init_uart(void);

//modify the private key
static uint64_t pri_exp=;
static uint64_t pri_mod=;
//modify the public key
static uint64_t pub_exp=;
static uint64_t pub_mod=;

struct public_key_class pub[1];
struct private_key_class priv[1];
void set_keys(void);
static uint64_t encrypted_buffer[MAX_CHAR];
static char decrypted_buffer[MAX_CHAR];

static char displayBuff[sizeof(uint64_t)*3];
static char tx_buffer[sizeof(uint64_t)*MAX_CHAR];
static char rx_buffer[sizeof(uint64_t)*MAX_CHAR];
