#include "./controller.h"

#include <string.h>

#include "./Devices/dev_usart.h"
#include "./Drivers/drv8323/drv8323.h"
#include "./PWM/pwm.h"
#include "./Sensors/current.h"
#include "./Sensors/hall.h"
#include "./State/state.h"
#include "./temp.h"
#include "./vbus.h"

drv8323_t g_driver;
static uint8_t g_tmp[8] = {0, 0, 0, 0, 0, 0, 0x80, 0x7f};

void controller_init(void) {
  /// 初始化串口
  dev_usart_init();

  /// 配置DRV8323

  // 使用PWM_MODE_3X控制时将互补引脚拉低
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);

  drv8323_init(&g_driver, &hspi3);
  drv8323_calibrate(&g_driver);
  HAL_Delay(10);
  drv8323_write_dcr(&g_driver, 0x0, DIS_GDF_DIS, 0x0, PWM_MODE_3X, 0x0, 0x0,
                    0x0, 0x0, 0x1);
  HAL_Delay(10);
  drv8323_write_csacr(&g_driver, 0x0, 0x1, 0x0, CSA_GAIN_40, 0x0, 0x1, 0x1, 0x1,
                      SEN_LVL_1_0);
  HAL_Delay(10);
  drv8323_write_csacr(&g_driver, 0x0, 0x1, 0x0, CSA_GAIN_40, 0x1, 0x0, 0x0, 0x0,
                      SEN_LVL_1_0);
  HAL_Delay(10);
  drv8323_write_ocpcr(&g_driver, TRETRY_50US, DEADTIME_50NS, OCP_NONE,
                      OCP_DEG_8US, VDS_LVL_1_88);
  drv8323_enable_gd(&g_driver);
  HAL_Delay(10);
  current_init();
  vbus_init();
  pwm_init();
  hall_init();

  state_set(STATE_IDLE);
}

void controller_step(void) {
  float temp = temp_get();
  // memcpy(g_tmp, &temp, sizeof(temp));
  // dev_usart_write(g_tmp, sizeof(g_tmp));
}