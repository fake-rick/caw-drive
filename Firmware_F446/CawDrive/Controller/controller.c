#include "./controller.h"

#include <string.h>

#include "./Devices/dev_usart.h"
#include "./Drivers/drv8323/drv8323.h"
#include "./FOC/VF.h"
#include "./PWM/pwm.h"
#include "./Sensors/current.h"
#include "./Sensors/hall.h"
#include "./State/state.h"
#include "./temp.h"
#include "./vbus.h"

drv8323_t g_driver;

float load_data[5];
static uint8_t g_tmp[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0,
                            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x80, 0x7f};

void controller_init(void) {
  /// 初始化串口
  dev_usart_init();

  /// 配置DRV8323

  // 使用PWM_MODE_3X控制时将互补引脚拉低
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);

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

  // rtU.SpeedRef = 800;

  rtU.Vbus = vbus_get();

  current_init();
  vbus_init();
  hall_init();
  pwm_init();

  // rtU.Motor_OnOff = 1;

  rtU.ud = 0;
  rtU.uq = 12;
  rtU.Freq = 4;

  pwm_start();

  state_set(STATE_RUN);
}

void controller_step(void) {
  // float temp = temp_get();
  rtU.Vbus = vbus_get();
}

void core_callback() {
  // hall_step();
  // rtU.theta = hall_get_theta();
  // rtU.SpeedFd = hall_get_speed();

  float current[3];

  current_get(current);

  // rtU.ia = current[0];
  // rtU.ib = current[1];
  // rtU.ic = current[2];
  VF_step();

  TIM1->CCR1 = rtY.tABC[0];
  TIM1->CCR2 = rtY.tABC[1];
  TIM1->CCR3 = rtY.tABC[2];

  load_data[0] = current[0];
  load_data[1] = current[1];
  load_data[2] = current[2];
  load_data[3] = rtY.tABC[0];
  load_data[4] = rtY.tABC[1];
  memcpy(g_tmp, (uint8_t *)load_data, sizeof(load_data));

  dev_usart_write(g_tmp, sizeof(g_tmp));
}