/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: VF.c
 *
 * Code generated for Simulink model 'VF'.
 *
 * Model version                  : 1.2
 * Simulink Coder version         : 24.2 (R2024b) 21-Jun-2024
 * C/C++ source code generated on : Mon Apr 21 16:53:27 2025
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: ARM Compatible->ARM Cortex-M
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "VF.h"
#include "mw_cmsis.h"
#include <math.h>
#include "rtwtypes.h"

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Model step function */
void VF_step(void)
{
  real_T rtb_Sum;
  real32_T rtb_Add;
  real32_T rtb_Min;
  real32_T rtb_Sum1_g;
  real32_T rtb_Sum_m;

  /* Sum: '<S2>/Sum' incorporates:
   *  Gain: '<S2>/Gain'
   *  Inport: '<Root>/Freq'
   *  UnitDelay: '<S2>/Unit Delay'
   */
  rtb_Sum = 0.000628318521F * rtU.Freq + rtDW.UnitDelay_DSTATE;

  /* If: '<S5>/If' incorporates:
   *  Constant: '<S5>/Constant'
   *  Constant: '<S5>/Constant1'
   *  RelationalOperator: '<S5>/Relational Operator'
   *  RelationalOperator: '<S5>/Relational Operator1'
   */
  if (rtb_Sum > 6.2831853071795862) {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem2' incorporates:
     *  ActionPort: '<S8>/Action Port'
     */
    /* Sum: '<S8>/Sum' incorporates:
     *  Constant: '<S8>/Constant'
     */
    rtb_Sum -= 6.2831853071795862;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem2' */
  } else if (rtb_Sum < 0.0) {
    /* Outputs for IfAction SubSystem: '<S5>/If Action Subsystem1' incorporates:
     *  ActionPort: '<S7>/Action Port'
     */
    /* Sum: '<S7>/Sum' incorporates:
     *  Constant: '<S7>/Constant'
     */
    rtb_Sum += 6.2831853071795862;

    /* End of Outputs for SubSystem: '<S5>/If Action Subsystem1' */
  }

  /* End of If: '<S5>/If' */

  /* Trigonometry: '<S1>/Cos' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion1'
   */
  rtb_Sum_m = arm_cos_f32((real32_T)rtb_Sum);

  /* Trigonometry: '<S1>/Sin' incorporates:
   *  DataTypeConversion: '<S1>/Data Type Conversion'
   */
  rtb_Sum1_g = arm_sin_f32((real32_T)rtb_Sum);

  /* Sum: '<S4>/Add' incorporates:
   *  Inport: '<Root>/ud'
   *  Inport: '<Root>/uq'
   *  Product: '<S4>/Product'
   *  Product: '<S4>/Product1'
   */
  rtb_Add = rtU.ud * rtb_Sum_m - rtU.uq * rtb_Sum1_g;

  /* Gain: '<S10>/Gain' */
  rtb_Min = -0.5F * rtb_Add;

  /* Gain: '<S10>/Gain1' incorporates:
   *  Inport: '<Root>/ud'
   *  Inport: '<Root>/uq'
   *  Product: '<S4>/Product2'
   *  Product: '<S4>/Product3'
   *  Sum: '<S4>/Add1'
   */
  rtb_Sum1_g = (rtU.ud * rtb_Sum1_g + rtU.uq * rtb_Sum_m) * 0.866025388F;

  /* Sum: '<S10>/Sum' */
  rtb_Sum_m = rtb_Min + rtb_Sum1_g;

  /* Sum: '<S10>/Sum1' */
  rtb_Sum1_g = rtb_Min - rtb_Sum1_g;

  /* Gain: '<S9>/Gain' incorporates:
   *  MinMax: '<S9>/Max'
   *  MinMax: '<S9>/Min'
   *  Sum: '<S9>/Sum'
   */
  rtb_Min = (fminf(fminf(rtb_Add, rtb_Sum_m), rtb_Sum1_g) + fmaxf(fmaxf(rtb_Add,
    rtb_Sum_m), rtb_Sum1_g)) * -0.5F;

  /* Outport: '<Root>/tABC' incorporates:
   *  Constant: '<S3>/Constant'
   *  Gain: '<S3>/Gain'
   *  Gain: '<S3>/PWM_HalfPeriod'
   *  Inport: '<Root>/Vbus'
   *  Product: '<S3>/Divide'
   *  Sum: '<S3>/Sum'
   *  Sum: '<S3>/Sum1'
   */
  rtY.tABC[0] = (-(rtb_Min + rtb_Add) / rtU.Vbus + 0.5F) * 8000.0F;
  rtY.tABC[1] = (-(rtb_Min + rtb_Sum_m) / rtU.Vbus + 0.5F) * 8000.0F;
  rtY.tABC[2] = (-(rtb_Min + rtb_Sum1_g) / rtU.Vbus + 0.5F) * 8000.0F;

  /* Update for UnitDelay: '<S2>/Unit Delay' */
  rtDW.UnitDelay_DSTATE = rtb_Sum;
}

/* Model initialize function */
void VF_initialize(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
