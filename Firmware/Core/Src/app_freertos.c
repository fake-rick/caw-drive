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
#include "log.h"
#include "lowpass_filter.h"
#include "motor.h"
#include "motor_load_params.h"
#include "pid.h"
#include "pingpong.h"
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

pid_t pid_current_q;
lowpass_filter_t lpf_q;
pid_t pid_current_d;
lowpass_filter_t lpf_d;
pid_t pid_velocity;
lowpass_filter_t lpf_velocity;
pid_t pid_angle;
lowpass_filter_t lpf_angle;
float target;
motor_control_type_e control_type;

event_t event;
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
uint32_t defaultTaskBuffer[512];
osStaticThreadDef_t defaultTaskControlBlock;
osThreadId stateTaskHandle;
uint32_t stateTaskBuffer[512];
osStaticThreadDef_t stateTaskControlBlock;
osThreadId transmitTaskHandle;
uint32_t transmitTaskBuffer[512];
osStaticThreadDef_t transmitTaskControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const* argument);
void StartStateTask(void const* argument);
void StartTransmitTask(void const* argument);

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
  event_run();

  error("************************");
  error("        CawDrive      ");
  error("   Version: V1.0.0   ");
  error("https://github.com/fake-rick/caw-drive");
  error("************************");

  drv8323_init(&drv8323_dev, &hspi3);
  debug("DRV8323 init ok");
  motor_custom_init(&drv8323_dev);
  pwmx3_driver_init(&driver, &htim1, 12.0, 12.0, PWM_PERIOD);

  as5047p_init(&sensor, &hspi3);
  debug("AS5047P init ok");
  current_init(&current_sensor, &htim1, TIM_CHANNEL_4, &hadc1, &hadc2, 0.001,
               40.0);
  current_calibrate_offsets(&current_sensor);

  pid_init(&pid_current_q, 1.0, 0.0, 0.0, 1000.0, 12.0);
  lowpass_filter_init(&lpf_q, 0.005);
  pid_init(&pid_current_d, 1.0, 0.0, 0.0, 1000.0, 12.0);
  lowpass_filter_init(&lpf_d, 0.005);
  pid_init(&pid_velocity, 0.3, 1.0, 0.0, 1000.0, 12.0);
  lowpass_filter_init(&lpf_velocity, 0.05);
  pid_init(&pid_angle, 2.0, 0.0, 0.0, 0.0, 24.0);
  lowpass_filter_init(&lpf_angle, 0.005);

  control_type = ANGLE;
  target = 0.0;

  motor_init(&motor, &driver, &current_sensor, &sensor, -1, control_type, 7,
             &pid_current_q, &lpf_q, &pid_current_d, &lpf_d, &pid_velocity,
             &lpf_velocity, &pid_angle, &lpf_angle);

  motor_align_sensor(&motor);

  info("initialized successfully");
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

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
  osThreadStaticDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 512,
                    defaultTaskBuffer, &defaultTaskControlBlock);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of stateTask */
  osThreadStaticDef(stateTask, StartStateTask, osPriorityNormal, 0, 512,
                    stateTaskBuffer, &stateTaskControlBlock);
  stateTaskHandle = osThreadCreate(osThread(stateTask), NULL);

  /* definition and creation of transmitTask */
  osThreadStaticDef(transmitTask, StartTransmitTask, osPriorityNormal, 0, 512,
                    transmitTaskBuffer, &transmitTaskControlBlock);
  transmitTaskHandle = osThreadCreate(osThread(transmitTask), NULL);

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

  uint32_t count = 0;
  /* Infinite loop */
  for (;;) {
    motor_step(&motor, target);
    if (count++ == 10) {
      count = 0;
      motor_observation_write_pack(&motor, &usart_dev);
    }
    portYIELD();
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartStateTask */
/**
 * @brief Function implementing the stateTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartStateTask */
void StartStateTask(void const* argument) {
  /* USER CODE BEGIN StartStateTask */
  /* Infinite loop */
  for (;;) {
    event_step();
    osDelay(1000);
  }
  /* USER CODE END StartStateTask */
}

/* USER CODE BEGIN Header_StartTransmitTask */
/**
 * @brief Function implementing the transmitTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTransmitTask */
void StartTransmitTask(void const* argument) {
  /* USER CODE BEGIN StartTransmitTask */
  /* Infinite loop */
  for (;;) {
    device_event_step(&usart_dev);
    portYIELD();
  }
  /* USER CODE END StartTransmitTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
