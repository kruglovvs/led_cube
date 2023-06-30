#include "LED_cube.h"
#include "main.h"

void cube_set_zero(uint8_t *cube_colors) {
	memset(cube_colors, 0, CUBE_MASSIVE_SIZE);
	for (uint16_t i = 0; i < CUBE_MASSIVE_SIZE; ++i) {
		cube_colors[i] = 0;
	}
	send_cube(cube_colors);
}
bool get_cube(uint8_t *cube_colors, const UART_HandleTypeDef *huart5) {
	HAL_UART_Receive_DMA(huart5, cube_colors, CUBE_MASSIVE_SIZE + 1);
	uint8_t got_crc8 = cube_colors[CUBE_MASSIVE_SIZE];
	uint8_t calc_crc8 = crc8(cube_colors);
	uint8_t answer = 0;
	if (calc_crc8 == got_crc8) {
		answer = ANSWER_GOT;
	} else {
		answer = ANSWER_ERROR;
	}
	HAL_UART_Transmit(&huart5, &answer, 1, TIMEOUT);
	return answer;
  //проверить crc8
  //считать от питонистов данные в режиме ожидания
  //считываем данные для кубика, вбиваем их в структуру

}
void send_cube(const uint8_t *cube_colors) {
  for (uint16_t pin = GPIO_PIN_0; pin < GPIO_PIN_8; pin *= 2) {
	  const uint8_t *end = cube_colors + CUBE_MASSIVE_SIZE / 8;
	  while (cube_colors < end) {
			  HAL_GPIO_SEND_BYTE(pin, cube_colors);
	  }
	  HAL_GPIO_WritePin(GPIOA, pin, GPIO_PIN_RESET);
  }
  TIM1->CNT = 0;
  while (TIM1->CNT < SEND_CUBE_DELAY) {};
}
uint8_t crc8(const uint8_t *data) {
    uint8_t crc = CRC8_INIT;
    uint8_t j = 0;
    for (uint16_t i = 0; i < CUBE_MASSIVE_SIZE; ++i, ++data) {
        crc ^= *data;
        for (j = 0; j < 8; j++) {
        	if (crc & 0x80) { //старший бит
        		crc = (crc << 1) ^ CRC8_POLY;
        	} else {
        		crc = crc << 1;
        	}
        }
    }
    return crc;
}
