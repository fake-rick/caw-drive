/* Copyright 2015-2023 The MathWorks, Inc. */

/****************************************************
*                                                   *   
* Wrapper functions for CMSIS functions             *
*                                                   *  
****************************************************/

#ifndef MW_CMSIS_H
#define MW_CMSIS_H

#include "arm_math.h"
#include "rtwtypes.h"

#define mw_arm_vlog_f32(pSrc, pDst, blockSize) arm_vlog_f32((float32_t *)pSrc, (float32_t *)pDst, blockSize)
#define mw_arm_vexp_f32(pSrc, pDst, blockSize) arm_vexp_f32((float32_t *)pSrc, (float32_t *)pDst, blockSize)

#define mw_arm_abs_f32(pSrc, pDst, blockSize) arm_abs_f32((float32_t *)pSrc, (float32_t *)pDst, blockSize)
#define mw_arm_abs_q7(pSrc, pDst, blockSize) arm_abs_q7((q7_t *)pSrc, (q7_t *)pDst, blockSize) 
#define mw_arm_abs_q15(pSrc, pDst, blockSize) arm_abs_q15((q15_t *)pSrc, (q15_t *)pDst, blockSize) 
#define mw_arm_abs_q31(pSrc, pDst, blockSize) arm_abs_q31((q31_t *)pSrc, (q31_t *)pDst, blockSize) 

#define mw_arm_sqrt_f32(pIn, pOut, blockSize) {\
if ((blockSize) == 1) { \
    arm_sqrt_f32(*(pIn), (pOut)); \
} else { \
    uint32_t i;\
    float32_t *pInput, *pOutput;\
    pInput = (float32_t *)pIn;\
    pOutput = (float32_t *)pOut;\
    for(i=0;i<blockSize;i++){\
        arm_sqrt_f32((float32_t)pInput[i],pOutput);\
        pOutput++;}\
    }\
}

#define mw_arm_float_to_q31(pSrc, pDst, blockSize) arm_float_to_q31((float32_t *)pSrc, (q31_t *)pDst, blockSize)
#define mw_arm_float_to_q15(pSrc, pDst, blockSize) arm_float_to_q15((float32_t *)pSrc, (q15_t *)pDst, blockSize)
#define mw_arm_float_to_q7(pSrc, pDst, blockSize) arm_float_to_q7((float32_t *)pSrc, (q7_t *)pDst, blockSize)

#define mw_arm_q15_to_float(pSrc, pDst, blockSize) arm_q15_to_float((q15_t *)pSrc, (float32_t *)pDst, blockSize)
#define mw_arm_q15_to_q31(pSrc, pDst, blockSize) arm_q15_to_q31((q15_t *)pSrc, (q31_t *)pDst, blockSize)
#define mw_arm_q15_to_q7(pSrc, pDst, blockSize) arm_q15_to_q7((q15_t *)pSrc, (q7_t *)pDst, blockSize)

#define mw_arm_q31_to_float(pSrc, pDst, blockSize) arm_q31_to_float((q31_t *)pSrc, (float32_t *)pDst, blockSize)
#define mw_arm_q31_to_q15(pSrc, pDst, blockSize) arm_q31_to_q15((q31_t *)pSrc, (q15_t *)pDst, blockSize)
#define mw_arm_q31_to_q7(pSrc, pDst, blockSize) arm_q31_to_q7((q31_t *)pSrc, (q7_t *)pDst, blockSize)

#define mw_arm_q7_to_float(pSrc, pDst, blockSize) arm_q7_to_float((q7_t *)pSrc, (float32_t *)pDst, blockSize)
#define mw_arm_q7_to_q31(pSrc, pDst, blockSize) arm_q7_to_q31((q7_t *)pSrc, (q31_t *)pDst, blockSize)
#define mw_arm_q7_to_q15(pSrc, pDst, blockSize) arm_q7_to_q15((q7_t *)pSrc, (q15_t *)pDst, blockSize)

#define mw_arm_add_f32(pSrcA, pSrcB, pDst, blockSize) arm_add_f32((float32_t *)pSrcA, (float32_t *)pSrcB, (float32_t *)pDst, blockSize)
#define mw_arm_add_q31(pSrcA, pSrcB, pDst, blockSize) arm_add_q31((q31_t *)pSrcA, (q31_t *)pSrcB, (q31_t *)pDst, blockSize)
#define mw_arm_add_q15(pSrcA, pSrcB, pDst, blockSize) arm_add_q15((q15_t *)pSrcA, (q15_t *)pSrcB, (q15_t *)pDst, blockSize)
#define mw_arm_add_q7(pSrcA, pSrcB, pDst, blockSize)  arm_add_q7((q7_t *)pSrcA, (q7_t *)pSrcB, (q7_t *)pDst, blockSize)

#define mw_arm_sub_f32(pSrcA, pSrcB, pDst, blockSize) arm_sub_f32((float32_t *)pSrcA, (float32_t *)pSrcB, (float32_t *)pDst, blockSize)
#define mw_arm_sub_q31(pSrcA, pSrcB, pDst, blockSize) arm_sub_q31((q31_t *)pSrcA, (q31_t *)pSrcB, (q31_t *)pDst, blockSize)
#define mw_arm_sub_q15(pSrcA, pSrcB, pDst, blockSize) arm_sub_q15((q15_t *)pSrcA, (q15_t *)pSrcB, (q15_t *)pDst, blockSize)
#define mw_arm_sub_q7(pSrcA, pSrcB, pDst, blockSize)  arm_sub_q7((q7_t *)pSrcA, (q7_t *)pSrcB, (q7_t *)pDst, blockSize)

#define mw_arm_mult_f32(pSrcA, pSrcB, pDst, blockSize) arm_mult_f32((float32_t *)pSrcA, (float32_t *)pSrcB, (float32_t *)pDst, blockSize)
#define mw_arm_mult_q31(pSrcA, pSrcB, pDst, blockSize) arm_mult_q31((q31_t *)pSrcA, (q31_t *)pSrcB, (q31_t *)pDst, blockSize)
#define mw_arm_mult_q15(pSrcA, pSrcB, pDst, blockSize) arm_mult_q15((q15_t *)pSrcA, (q15_t *)pSrcB, (q15_t *)pDst, blockSize)
#define mw_arm_mult_q7(pSrcA, pSrcB, pDst, blockSize)  arm_mult_q7((q7_t *)pSrcA, (q7_t *)pSrcB, (q7_t *)pDst, blockSize)

#define mw_arm_cmplx_conj_f32(pSrc, pDst, numSamples) arm_cmplx_conj_f32((float32_t *)pSrc, (float32_t *)pDst, numSamples)
#define mw_arm_cmplx_conj_q31(pSrc, pDst, numSamples) arm_cmplx_conj_q31((q31_t *)pSrc, (q31_t *)pDst, numSamples)
#define mw_arm_cmplx_conj_q15(pSrc, pDst, numSamples) arm_cmplx_conj_q15((q15_t *)pSrc, (q15_t *)pDst, numSamples)

#define mw_arm_cmplx_mult_cmplx_f32(pSrcA, pSrcB, pDst, blockSize) arm_cmplx_mult_cmplx_f32((float32_t *)pSrcA, (float32_t *)pSrcB, (float32_t *)pDst, blockSize)
#define mw_arm_cmplx_mult_cmplx_q31(pSrcA, pSrcB, pDst, blockSize) arm_cmplx_mult_cmplx_q31((q31_t *)pSrcA, (q31_t *)pSrcB, (q31_t *)pDst, blockSize)
#define mw_arm_cmplx_mult_cmplx_q15(pSrcA, pSrcB, pDst, blockSize) arm_cmplx_mult_cmplx_q15((q15_t *)pSrcA, (q15_t *)pSrcB, (q15_t *)pDst, blockSize)

#define mw_arm_cmplx_mult_real_f32(pSrcA, pSrcB, pDst, blockSize) arm_cmplx_mult_real_f32((float32_t *)pSrcA, (float32_t *)pSrcB, (float32_t *)pDst, blockSize)
#define mw_arm_cmplx_mult_real_q31(pSrcA, pSrcB, pDst, blockSize) arm_cmplx_mult_real_q31((q31_t *)pSrcA, (q31_t *)pSrcB, (q31_t *)pDst, blockSize)
#define mw_arm_cmplx_mult_real_q15(pSrcA, pSrcB, pDst, blockSize) arm_cmplx_mult_real_q15((q15_t *)pSrcA, (q15_t *)pSrcB, (q15_t *)pDst, blockSize)

#define mw_arm_rshift_q15(pSrc, shiftBits, pDst, blockSize) arm_shift_q15 ((q15_t *)pSrc, -(shiftBits),(q15_t *)pDst, blockSize)
#define mw_arm_rshift_q31(pSrc, shiftBits, pDst, blockSize) arm_shift_q31 ((q31_t *)pSrc, -(shiftBits), (q31_t *)pDst, blockSize)
#define mw_arm_rshift_q7(pSrc, shiftBits, pDst, blockSize) arm_shift_q7 ((q7_t *)pSrc,  -(shiftBits), (q7_t *)pDst, blockSize)

#define mw_arm_shift_q15(pSrc, shiftBits, pDst, blockSize) arm_shift_q15 ((q15_t *)pSrc, shiftBits,(q15_t *)pDst, blockSize)
#define mw_arm_shift_q31(pSrc, shiftBits, pDst, blockSize) arm_shift_q31 ((q31_t *)pSrc, shiftBits, (q31_t *)pDst, blockSize)
#define mw_arm_shift_q7(pSrc, shiftBits, pDst, blockSize) arm_shift_q7 ((q7_t *)pSrc, shiftBits, (q7_t *)pDst, blockSize)

#define mw_arm_scale_1_f32(scaleValue, pSrc, pDst, blockSize) arm_scale_f32 ((float32_t *)pSrc, *scaleValue, (float32_t *)pDst, blockSize)
#define mw_arm_scale_1_q7(scaleValue, pSrc, pDst, blockSize, scaleShift) arm_scale_q7 ((q7_t *)pSrc, *scaleValue, scaleShift, (q7_t *)pDst, blockSize)
#define mw_arm_scale_1_q15(scaleValue, pSrc, pDst, blockSize, scaleShift) arm_scale_q15 ((q15_t *)pSrc, *scaleValue, scaleShift, (q15_t *)pDst, blockSize)
#define mw_arm_scale_1_q31(scaleValue, pSrc, pDst, blockSize, scaleShift) arm_scale_q31 ((q31_t *)pSrc, *scaleValue, scaleShift, (q31_t *)pDst, blockSize)

#define mw_arm_scale_2_f32(pSrc, scaleValue, pDst, blockSize) arm_scale_f32 ((float32_t *)pSrc, *scaleValue, (float32_t *)pDst, blockSize)
#define mw_arm_scale_2_q7(pSrc, scaleValue, pDst, blockSize, scaleShift) arm_scale_q7 ((q7_t *)pSrc, *scaleValue, scaleShift, (q7_t *)pDst, blockSize)
#define mw_arm_scale_2_q15(pSrc, scaleValue, pDst, blockSize, scaleShift) arm_scale_q15 ((q15_t *)pSrc, *scaleValue, scaleShift, (q15_t *)pDst, blockSize)
#define mw_arm_scale_2_q31(pSrc, scaleValue, pDst, blockSize, scaleShift) arm_scale_q31 ((q31_t *)pSrc, *scaleValue, scaleShift, (q31_t *)pDst, blockSize)

#define mw_arm_bias_1_f32(biasValue, pSrc, pDst, blockSize) arm_offset_f32 ((float32_t *)pSrc, *biasValue, (float32_t *)pDst, blockSize)
#define mw_arm_bias_1_q7(biasValue, pSrc, pDst, blockSize) arm_offset_q7 ((q7_t *)pSrc, *biasValue, (q7_t *)pDst, blockSize)
#define mw_arm_bias_1_q15(biasValue, pSrc, pDst, blockSize) arm_offset_q15 ((q15_t *)pSrc, *biasValue, (q15_t *)pDst, blockSize)
#define mw_arm_bias_1_q31(biasValue, pSrc, pDst, blockSize) arm_offset_q31 ((q31_t *)pSrc, *biasValue, (q31_t *)pDst, blockSize)

#define mw_arm_bias_2_f32(pSrc, biasValue, pDst, blockSize) arm_offset_f32 ((float32_t *)pSrc, *biasValue, (float32_t *)pDst, blockSize)
#define mw_arm_bias_2_q7(pSrc, biasValue, pDst, blockSize) arm_offset_q7 ((q7_t *)pSrc, *biasValue, (q7_t *)pDst, blockSize)
#define mw_arm_bias_2_q15(pSrc, biasValue, pDst, blockSize) arm_offset_q15 ((q15_t *)pSrc, *biasValue, (q15_t *)pDst, blockSize)
#define mw_arm_bias_2_q31(pSrc, biasValue, pDst, blockSize) arm_offset_q31 ((q31_t *)pSrc, *biasValue, (q31_t *)pDst, blockSize)

#define mw_arm_dot_prod_f32(pSrcA, pSrcB, pDst, blockSize) arm_dot_prod_f32 ((float32_t *)pSrcA,  (float32_t *)pSrcB, blockSize, (float32_t *)pDst)
#define mw_arm_dot_prod_q7(pSrcA, pSrcB, pDst, blockSize) arm_dot_prod_q7 ((q7_t *)pSrcA,  (q7_t *)pSrcB, blockSize, (q31_t *)(pDst))
#define mw_arm_dot_prod_q15(pSrcA, pSrcB, pDst, blockSize) arm_dot_prod_q15 ((q15_t *)pSrcA, (q15_t *) pSrcB, blockSize, (q63_t *) (pDst))
#define mw_arm_dot_prod_q31(pSrcA, pSrcB, pDst, blockSize) arm_dot_prod_q31 ((q31_t *)pSrcA,  (q31_t *)pSrcB, blockSize, (q63_t *)(pDst))

#define mw_arm_saturate_q15(pSrcIn, pSrcMin, pSrcMax, pDst, blockSize) arm_clip_q15 ((q15_t *)pSrcIn, (q15_t *)pDst, (q15_t)(*pSrcMin), (q15_t)(*pSrcMax), blockSize)
#define mw_arm_saturate_q31(pSrcIn, pSrcMin, pSrcMax, pDst, blockSize) arm_clip_q31 ((q31_t *)pSrcIn, (q31_t *)pDst, (q31_t)(*pSrcMin), (q31_t)(*pSrcMax), blockSize)
#define mw_arm_saturate_q7(pSrcIn, pSrcMin, pSrcMax, pDst, blockSize) arm_clip_q7 ((q7_t *)pSrcIn, (q7_t *)pDst, (q7_t)(*pSrcMin), (q7_t)(*pSrcMax), blockSize)
#define mw_arm_saturate_f32(pSrcIn, pSrcMin, pSrcMax, pDst, blockSize) arm_clip_f32 ((float32_t *)pSrcIn, (float32_t *)pDst, *pSrcMin, *pSrcMax, blockSize)

#define mw_arm_uminus_f32(pSrc, pDst, blockSize) arm_negate_f32((float32_t *)pSrc, (float32_t *)pDst, blockSize)
#define mw_arm_uminus_q31(pSrc, pDst, blockSize) arm_negate_q31((q31_t *)pSrc, (q31_t *)pDst, blockSize)
#define mw_arm_uminus_q15(pSrc, pDst, blockSize) arm_negate_q15((q15_t *)pSrc, (q15_t *)pDst, blockSize)
#define mw_arm_uminus_q7(pSrc, pDst, blockSize)  arm_negate_q7((q7_t *)pSrc, (q7_t *)pDst, blockSize)

#define mw_arm_and_uint32(pSrcA, pSrcB, pDst, blockSize) arm_and_u32((uint32_t *)(uint32_t *)pSrcA, (uint32_t *)pSrcB, (uint32_t *)pDst, blockSize)
#define mw_arm_and_uint16(pSrcA, pSrcB, pDst, blockSize) arm_and_u16((uint16_t *)pSrcA, (uint16_t *)pSrcB, (uint16_t *)pDst, blockSize)
#define mw_arm_and_uint8(pSrcA, pSrcB, pDst, blockSize) arm_and_u8((uint8_t *)pSrcA, (uint8_t *)pSrcB, (uint8_t *)pDst, blockSize)

#define mw_arm_or_uint32(pSrcA, pSrcB, pDst, blockSize) arm_or_u32((uint32_t *)pSrcA, (uint32_t *)pSrcB, (uint32_t *)pDst, blockSize)
#define mw_arm_or_uint16(pSrcA, pSrcB, pDst, blockSize) arm_or_u16((uint16_t *)pSrcA, (uint16_t *)pSrcB, (uint16_t *)pDst, blockSize)
#define mw_arm_or_uint8(pSrcA, pSrcB, pDst, blockSize) arm_or_u8((uint8_t *)pSrcA, (uint8_t *)pSrcB, (uint8_t *)pDst, blockSize)

#define mw_arm_xor_uint32(pSrcA, pSrcB, pDst, blockSize) arm_xor_u32((uint32_t *)pSrcA, (uint32_t *)pSrcB, (uint32_t *)pDst, blockSize)
#define mw_arm_xor_uint16(pSrcA, pSrcB, pDst, blockSize) arm_xor_u16((uint16_t *)pSrcA, (uint16_t *)pSrcB, (uint16_t *)pDst, blockSize)
#define mw_arm_xor_uint8(pSrcA, pSrcB, pDst, blockSize) arm_xor_u8((uint8_t *)pSrcA, (uint8_t *)pSrcB, (uint8_t *)pDst, blockSize)

#define mw_arm_not_uint32(pSrc, pDst, blockSize) arm_not_u32((uint32_t *)pSrc, (uint32_t *)pDst, blockSize)
#define mw_arm_not_uint16(pSrc, pDst, blockSize) arm_not_u16((uint16_t *)pSrc, (uint16_t *)pDst, blockSize)
#define mw_arm_not_uint8(pSrc, pDst, blockSize) arm_not_u8((uint8_t *)pSrc, (uint8_t *)pDst, blockSize)

#define mw_arm_saturate_q15(pSrcIn, pSrcMin, pSrcMax, pDst, blockSize) arm_clip_q15 ((q15_t *)pSrcIn, (q15_t *)pDst, (q15_t)(*pSrcMin), (q15_t)(*pSrcMax), blockSize)
#define mw_arm_saturate_q31(pSrcIn, pSrcMin, pSrcMax, pDst, blockSize) arm_clip_q31 ((q31_t *)pSrcIn, (q31_t *)pDst, (q31_t)(*pSrcMin), (q31_t)(*pSrcMax), blockSize)
#define mw_arm_saturate_q7(pSrcIn, pSrcMin, pSrcMax, pDst, blockSize) arm_clip_q7 ((q7_t *)pSrcIn, (q7_t *)pDst, (q7_t)(*pSrcMin), (q7_t)(*pSrcMax), blockSize)
#define mw_arm_saturate_f32(pSrcIn, pSrcMin, pSrcMax, pDst, blockSize) arm_clip_f32 ((float32_t *)pSrcIn, (float32_t *)pDst, *pSrcMin, *pSrcMax, blockSize)

#endif
