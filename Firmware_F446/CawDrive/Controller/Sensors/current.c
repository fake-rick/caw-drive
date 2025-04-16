#include "./current.h"

#include <string.h>

#include "adc.h"
#include "dev_usart.h"
#include "stm32f4xx_hal_adc_ex.h"

#define ADC_RESOLUTION 4096.0f
#define ADC_VOLTAGE_REF 3.3f
#define SHUNT_RESISTANCE 0.005f
#define AMP_GAIN 40.0f

static float g_ia_offset = 0.0f;
static float g_ib_offset = 0.0f;
static float g_ic_offset = 0.0f;
static float g_gain = 0.0f;
static float g_ia = 0.0f;
static float g_ib = 0.0f;
static float g_ic = 0.0f;

static uint8_t g_adc1_state = 0;
static uint8_t g_adc2_state = 0;
static uint8_t g_adc3_state = 0;

static float g_current[3] = {0.0f, 0.0f, 0.0f};

static uint8_t g_tmp[16] = {0, 0, 0, 0, 0, 0, 0,    0,
                            0, 0, 0, 0, 0, 0, 0x80, 0x7f};

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc) {
  static int16_t adc1_cnt = 0;
  static int16_t adc2_cnt = 0;
  static int16_t adc3_cnd = 0;

  if (ADC1 == hadc->Instance) {
    float adc1_in0 = hadc1.Instance->JDR1;
    if (0 == g_adc1_state) {
      g_ia_offset += adc1_in0;
      if (100 == ++adc1_cnt) {
        g_ia_offset /= 100.0f;
        g_adc1_state = 1;
      }
    } else {
      g_ia = (adc1_in0 - g_ia_offset) * g_gain;
    }
  } else if (ADC2 == hadc->Instance) {
    float adc2_in1 = hadc2.Instance->JDR1;
    if (0 == g_adc2_state) {
      g_ib_offset += adc2_in1;
      if (100 == ++adc2_cnt) {
        g_ib_offset /= 100.0f;
        g_adc2_state = 1;
      }
    } else {
      g_ib = (adc2_in1 - g_ib_offset) * g_gain;
    }
  } else if (ADC3 == hadc->Instance) {
    float adc3_in2 = hadc3.Instance->JDR1;
    if (0 == g_adc3_state) {
      g_ic_offset += adc3_in2;
      if (100 == ++adc3_cnd) {
        g_ic_offset /= 100.0f;
        g_adc3_state = 1;
      }
    } else {
      g_ic = (adc3_in2 - g_ic_offset) * g_gain;
    }
  }

  if (!g_adc1_state || !g_adc2_state || !g_adc3_state) {
    return;
  }

  if (ADC1 == hadc->Instance) {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    // 在ADC1中触发FOC控制
    g_current[0] = g_ia;
    g_current[1] = g_ib;
    g_current[2] = g_ic;

    // memcpy(g_tmp, g_current, sizeof(g_current));
    // dev_usart_write(g_tmp, sizeof(g_tmp));
  }
}

void current_init(void) {
  // 计算增益(计算出电压值 除以 采样电阻值 除以 放大倍数)
  g_gain = (ADC_VOLTAGE_REF / ADC_RESOLUTION) / (SHUNT_RESISTANCE * AMP_GAIN);

  HAL_ADCEx_InjectedStart_IT(&hadc1);
  HAL_ADCEx_InjectedStart_IT(&hadc2);
  HAL_ADCEx_InjectedStart_IT(&hadc3);
}

int current_get(float* arr) {
  if (!g_adc1_state || !g_adc2_state || !g_adc3_state) {
    return -1;
  }
  memcpy(arr, g_current, sizeof(g_current));
  return 0;
}