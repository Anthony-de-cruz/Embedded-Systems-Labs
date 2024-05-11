#include "stm32f7xx_hal.h"
#define LCD_LIGHT  2
#define LCD_NORMAL  1
#define LCD_OFF  0
void EnableBrightnessControl();
void UpdateBrightness(uint16_t setting);