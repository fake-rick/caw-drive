/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: VF.h
 *
 * Code generated for Simulink model 'VF'.
 *
 * Model version                  : 1.1
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Wed Mar 26 01:49:43 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef VF_h_
#define VF_h_
#ifndef VF_COMMON_INCLUDES_
#define VF_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* VF_COMMON_INCLUDES_ */

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real_T UnitDelay_DSTATE;             /* '<S2>/Unit Delay' */
} DW;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T ud;                         /* '<Root>/ud' */
  real32_T uq;                         /* '<Root>/uq' */
  real32_T Freq;                       /* '<Root>/Freq' */
  real32_T Vbus;                       /* '<Root>/Vbus' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real32_T tABC[3];                    /* '<Root>/tABC' */
} ExtY;

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;

/* Model entry point functions */
extern void VF_initialize(void);
extern void VF_step(void);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('SVPWM/VF mode')    - opens subsystem SVPWM/VF mode
 * hilite_system('SVPWM/VF mode/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SVPWM'
 * '<S1>'   : 'SVPWM/VF mode'
 * '<S2>'   : 'SVPWM/VF mode/FreqGenerator'
 * '<S3>'   : 'SVPWM/VF mode/SVPWM'
 * '<S4>'   : 'SVPWM/VF mode/iPark'
 * '<S5>'   : 'SVPWM/VF mode/FreqGenerator/RangeLimit'
 * '<S6>'   : 'SVPWM/VF mode/FreqGenerator/RangeLimit/If Action Subsystem'
 * '<S7>'   : 'SVPWM/VF mode/FreqGenerator/RangeLimit/If Action Subsystem1'
 * '<S8>'   : 'SVPWM/VF mode/FreqGenerator/RangeLimit/If Action Subsystem2'
 * '<S9>'   : 'SVPWM/VF mode/SVPWM/ei_t'
 * '<S10>'  : 'SVPWM/VF mode/SVPWM/iClark'
 */
#endif                                 /* VF_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
