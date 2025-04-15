#include "./current.h"

#include "adc.h"

static float g_ia_offset = 0.0f;
static float g_ib_offset = 0.0f;
static float g_ic_offset = 0.0f;

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef *hadc) {}

void current_init(void) {
  HAL_ADCEx_InjectedStart_IT(&hadc1);
  HAL_ADCEx_InjectedStart(&hadc2);
  HAL_ADCEx_InjectedStart(&hadc3);
}