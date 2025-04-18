#include "./hall.h"

#include "dev_usart.h"
#include "tim.h"

void hall_init(void) { HAL_TIMEx_HallSensor_Start_IT(&htim3); }

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
  dev_usart_write("hello\r\n", 8);
}