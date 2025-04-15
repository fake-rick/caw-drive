#include "./current.h"

#include "adc.h"
#include "dev_usart.h"
#include "stm32f4xx_hal_adc_ex.h"

static float g_ia_offset = 0.0f;
static float g_ib_offset = 0.0f;
static float g_ic_offset = 0.0f;

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (ADC1 == hadc->Instance) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  }
}

void current_init(void) {
  HAL_ADCEx_InjectedStart_IT(&hadc1);
  HAL_ADCEx_InjectedStart_IT(&hadc2);
  HAL_ADCEx_InjectedStart_IT(&hadc3);
}