/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../caw_embedded/app/motor/observation.h"
#include "as5047p.h"
#include "caw_config.h"
#include "code.h"
#include "current.h"
#include "custom.h"
#include "device.h"
#include "drv8323.h"
#include "event.h"
#include "helper.h"
#include "log.h"
#include "lowpass_filter.h"
#include "motor.h"
#include "motor_flash_params.h"
#include "motor_init_params.h"
#include "motor_load_params.h"
#include "motor_params.h"
#include "motor_set_params.h"
#include "pid.h"
#include "pwmx3.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
device_t usart_dev;
drv8323_t drv8323_dev;
motor_t motor;
pwmx3_driver driver;
as5047p_t sensor;
current_t current_sensor;

extern pid_t global_motor_pid_current_q;
extern lowpass_filter_t global_motor_lpf_q;
extern pid_t global_motor_pid_current_d;
extern lowpass_filter_t global_motor_lpf_d;
extern pid_t global_motor_pid_velocity;
extern lowpass_filter_t global_motor_lpf_velocity;
extern pid_t global_motor_pid_angle;
extern lowpass_filter_t global_motor_lpf_angle;
extern pll_t global_motor_encoder_pll;

event_t event;

extern motor_params_t global_motor_params;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
uint32_t defaultTaskBuffer[256];
osStaticThreadDef_t defaultTaskControlBlock;
osThreadId eventTaskHandle;
uint32_t stateTaskBuffer[512];
osStaticThreadDef_t stateTaskControlBlock;
osSemaphoreId observationBinarySemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const* argument);
void StartEventTask(void const* argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                   StackType_t** ppxIdleTaskStackBuffer,
                                   uint32_t* pulIdleTaskStackSize);

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                   StackType_t** ppxIdleTaskStackBuffer,
                                   uint32_t* pulIdleTaskStackSize) {
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  log_init(&usart_dev);
  device_init(&usart_dev);
  device_set_id(&usart_dev, 0x1001, device_type_motor);
  event_init(&usart_dev);
  event_register(main_code_motor, sub_code_motor_load_params,
                 motor_load_params_cb);
  event_register(main_code_motor, sub_code_motor_init_params,
                 motor_init_params_cb);
  event_register(main_code_motor, sub_code_motor_set_params,
                 motor_set_params_cb);
  event_register(main_code_motor, sub_code_motor_flash_params,
                 motor_flash_params_cb);
  event_run();

  error("CawDrive v1.0.0");
  error("https://github.com/fake-rick/caw-drive");

  drv8323_init(&drv8323_dev, &hspi3);
  debug("DRV8323 init ok");
  motor_custom_init(&drv8323_dev);
  pwmx3_driver_init(&driver, &htim1, 12.0, 12.0, PWM_PERIOD);

  as5047p_init(&sensor, &hspi3);
  debug("AS5047P init ok");
  current_init(&current_sensor, &htim1, TIM_CHANNEL_4, &hadc1, &hadc2, 0.001,
               40.0);
  current_calibrate_offsets(&current_sensor);

  motor_params_load(&global_motor_params);
  motor_params_reset();

  // pll_init(&encoder_pll, as5047p_get_cpr(), 1000.0);

  motor_init(&motor, &driver, &current_sensor, &sensor, -1,
             global_motor_params.mode, 7, &global_motor_pid_current_q,
             &global_motor_lpf_q, &global_motor_pid_current_d,
             &global_motor_lpf_d, &global_motor_pid_velocity,
             &global_motor_lpf_velocity, &global_motor_pid_angle,
             &global_motor_lpf_angle, 0);

  motor_align_sensor(&motor);

  HAL_TIM_Base_Start_IT(&htim8);

  info("initialized successfully");
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of observationBinarySem */
  osSemaphoreDef(observationBinarySem);
  observationBinarySemHandle =
      osSemaphoreCreate(osSemaphore(observationBinarySem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadStaticDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 256,
                    defaultTaskBuffer, &defaultTaskControlBlock);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of eventTask */
  osThreadStaticDef(eventTask, StartEventTask, osPriorityNormal, 0, 512,
                    stateTaskBuffer, &stateTaskControlBlock);
  eventTaskHandle = osThreadCreate(osThread(eventTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const* argument) {
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;) {
    motor_observation_write_pack(&motor, &usart_dev);
    osDelay(2);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartEventTask */
/**
 * @brief Function implementing the eventTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartEventTask */
void StartEventTask(void const* argument) {
  /* USER CODE BEGIN StartEventTask */
  /* Infinite loop */
  for (;;) {
    event_step(&event);
    osDelay(1);
  }
  /* USER CODE END StartEventTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
