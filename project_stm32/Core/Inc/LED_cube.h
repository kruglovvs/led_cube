#ifndef INCLUDE_LED_CUBE_H
#define INCLUDE_LED_CUBE_H

#include "main.h"
#include "stdbool.h"
#include "string.h"
#include "stdlib.h"

#define CUBE_SIZE 8
#define SEND_CUBE_DELAY 0xff00
#define CRC8_INIT 0x00
#define CRC8_POLY 0x31
#define CUBE_MASSIVE_SIZE CUBE_SIZE * CUBE_SIZE * CUBE_SIZE * 3
#define TIMEOUT 50
#define ANSWER_ERROR 0xff
#define ANSWER_GOT 0x00

#define HAL_GPIO_SEND_BIT(PIN, BIT, COLORS)   {		 \
    if ((*COLORS) & (1 << BIT)) {                    \
      HAL_GPIO_WritePin(GPIOA, PIN, GPIO_PIN_SET);   \
      TIM1->CNT = 0;                                 \
      while (TIM1->CNT < 51) {};                     \
      HAL_GPIO_WritePin(GPIOA, PIN, GPIO_PIN_RESET); \
      TIM1->CNT = 0;                                 \
    } else {                                         \
      HAL_GPIO_WritePin(GPIOA, PIN, GPIO_PIN_SET);   \
      TIM1->CNT = 0;                                 \
      while (TIM1->CNT < 1) {};                      \
      HAL_GPIO_WritePin(GPIOA, PIN, GPIO_PIN_RESET); \
      TIM1->CNT = 0;                                 \
      while (TIM1->CNT < 39) {};                     \
    }        										 \
}
#define HAL_GPIO_SEND_LAST_BIT(PIN, BIT, COLORS)   {		 \
    if ((*COLORS) & (1 << BIT)) {                    \
      HAL_GPIO_WritePin(GPIOA, PIN, GPIO_PIN_SET);   \
      TIM1->CNT = 0;                                 \
      while (TIM1->CNT < 52) {};                     \
      HAL_GPIO_WritePin(GPIOA, PIN, GPIO_PIN_RESET); \
    } else {                                         \
      HAL_GPIO_WritePin(GPIOA, PIN, GPIO_PIN_SET);   \
      TIM1->CNT = 0;                                 \
      while (TIM1->CNT < 1) {};                      \
      HAL_GPIO_WritePin(GPIOA, PIN, GPIO_PIN_RESET); \
      TIM1->CNT = 0;                                 \
      while (TIM1->CNT < 39) {};                     \
    }        										 \
}
#define HAL_GPIO_SEND_BYTE(PIN, COLORS) { \
    HAL_GPIO_SEND_BIT(PIN, 7, COLORS);  \
    HAL_GPIO_SEND_BIT(PIN, 6, COLORS);  \
    HAL_GPIO_SEND_BIT(PIN, 5, COLORS);  \
    HAL_GPIO_SEND_BIT(PIN, 4, COLORS);  \
    HAL_GPIO_SEND_BIT(PIN, 3, COLORS);  \
    HAL_GPIO_SEND_BIT(PIN, 2, COLORS);  \
    HAL_GPIO_SEND_BIT(PIN, 1, COLORS);  \
    HAL_GPIO_SEND_LAST_BIT(PIN, 0, COLORS);  \
    ++COLORS;                     		\
    }
void cube_set_zero(uint8_t *cube_colors);
bool get_cube(uint8_t *cube_colors);
void send_cube(const uint8_t *cube_colors);
uint8_t crc8(const uint8_t *data);
#endif //INCLUDE_LED_CUBE_H
