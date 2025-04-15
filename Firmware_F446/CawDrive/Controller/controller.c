#include "./controller.h"

#include "./Drivers/drv8323/drv8323.h"

drv8323_t g_driver;

void controller_init(void) {
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
}