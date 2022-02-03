/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_bitreversal2.c
 * Description:  Bitreversal functions
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2019 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dsp/transform_functions.h"
#include "arm_common_tables.h"
#include "arm_math_types.h"


/**
  @brief         In-place 64 bit reversal function.
  @param[in,out] pSrc        points to in-place buffer of unknown 64-bit data type
  @param[in]     bitRevLen   bit reversal table length
  @param[in]     pBitRevTab  points to bit reversal table
  @return        none
*/

void arm_bitreversal_64(
        uint64_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab)
{
  uint64_t a, b, i, tmp;

  for (i = 0; i < bitRevLen; )
  {
     a = pBitRevTab[i    ] >> 2;
     b = pBitRevTab[i + 1] >> 2;

     //real
     tmp = pSrc[a];
     pSrc[a] = pSrc[b];
     pSrc[b] = tmp;

     //complex
     tmp = pSrc[a+1];
     pSrc[a+1] = pSrc[b+1];
     pSrc[b+1] = tmp;

    i += 2;
  }
}

/**
  @brief         In-place 32 bit reversal function.
  @param[in,out] pSrc        points to in-place buffer of unknown 32-bit data type
  @param[in]     bitRevLen   bit reversal table length
  @param[in]     pBitRevTab  points to bit reversal table
  @return        none
*/

void arm_bitreversal_32(
        uint32_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab)
{
#if defined (__ARM_ARCH_8M_BASE__) || defined (__ARM_ARCH_6M__)
  __ASM volatile (
 #if defined(__GNUC__)
        ".syntax unified                \n"
 #endif
                    
    " 	ADDS     r3,%1,#1              \n"
    " 	ADDS     %1,%2,#0              \n"
    " 	LSRS     r3,r3,#1              \n"
    "arm_bitreversal_32_0:             \n"
    " 	LDRH     %2,[%1,#2]            \n"
    " 	LDRH     r6,[%1,#0]            \n"
    " 	ADD      %2,%0,%2              \n"
    " 	ADD      r6,%0,r6              \n"
    " 	LDR      r5,[%2,#0]            \n"
    " 	LDR      r4,[r6,#0]            \n"
    " 	STR      r5,[r6,#0]            \n"
    " 	STR      r4,[%2,#0]            \n"
    " 	LDR      r5,[%2,#4]            \n"
    " 	LDR      r4,[r6,#4]            \n"
    " 	STR      r5,[r6,#4]            \n"
    " 	STR      r4,[%2,#4]            \n"
    " 	ADDS     %1,%1,#4              \n"
    " 	SUBS     r3,r3,#1              \n"
    " 	BNE      arm_bitreversal_32_0  \n"
  : /* no outputs */ 
  : "r" (pSrc), "r" (bitRevLen), "r" (pBitRevTab)
  : "r4", "r5", "r6", "memory"
  );
#else
    if (0U == bitRevLen)
    {
      return;
    }

  __ASM volatile (
 #if defined(__GNUC__)
        ".syntax unified                \n"
 #endif

    " 	ADDS     r3,%1,#1              \n"
    " 	ADDS     %1,%2,#2              \n"
    " 	LSRS     r3,r3,#2              \n"
    "arm_bitreversal_32_0:             \n" /* loop unrolled by 2 */
    " 	LDRH     r8,[%1,#4]            \n"
    " 	LDRH     r9,[%1,#2]            \n"
    " 	LDRH     %2,[%1,#0]            \n"
    " 	LDRH     r12,[%1,#-2]          \n"
    " 	ADD      r8,%0,r8              \n"
    " 	ADD      r9,%0,r9              \n"
    " 	ADD      %2,%0,%2              \n"
    " 	ADD      r12,%0,r12            \n"
    " 	LDR      r7,[r9,#0]            \n"
    " 	LDR      r6,[r8,#0]            \n"
    " 	LDR      r5,[%2,#0]            \n"
    " 	LDR      r4,[r12,#0]           \n"
    " 	STR      r6,[r9,#0]            \n"
    " 	STR      r7,[r8,#0]            \n"
    " 	STR      r5,[r12,#0]           \n"
    " 	STR      r4,[%2,#0]            \n"
    " 	LDR      r7,[r9,#4]            \n"
    " 	LDR      r6,[r8,#4]            \n"
    " 	LDR      r5,[%2,#4]            \n"
    " 	LDR      r4,[r12,#4]           \n"
    " 	STR      r6,[r9,#4]            \n"
    " 	STR      r7,[r8,#4]            \n"
    " 	STR      r5,[r12,#4]           \n"
    " 	STR      r4,[%2,#4]            \n"
    " 	ADDS     %1,%1,#8              \n"
    " 	SUBS     r3,r3,#1              \n"
    " 	BNE      arm_bitreversal_32_0  \n"
  : /* no outputs */ 
  : "r" (pSrc), "r" (bitRevLen), "r" (pBitRevTab)
  : "r4", "r5", "r6", "r7", "r8", "r9", "memory"
  );
#endif
}


/**
  @brief         In-place 16 bit reversal function.
  @param[in,out] pSrc        points to in-place buffer of unknown 16-bit data type
  @param[in]     bitRevLen   bit reversal table length
  @param[in]     pBitRevTab  points to bit reversal table
  @return        none
*/

void arm_bitreversal_16(
        uint16_t *pSrc,
  const uint16_t bitRevLen,
  const uint16_t *pBitRevTab)
{
  
#if defined (__ARM_ARCH_8M_BASE__)
  __ASM volatile (
 #if defined(__GNUC__)
        ".syntax unified                \n"
 #endif
    " 	ADDS     r3,%1,#1               \n"
    " 	ADDS     %1,%2,#0               \n"
    " 	LSRS     r3,r3,#1               \n"
    "arm_bitreversal_16_0:              \n"
    " 	LDRH     %2,[%1,#2]             \n"
    " 	LDRH     r6,[%1,#0]             \n"
    "   LSRS     %2,%2,#1               \n"
    "   LSRS     r6,r6,#1               \n"
    " 	ADD      %2,%0,%2               \n"
    " 	ADD      r6,%0,r6               \n"
    " 	LDR      r5,[%2,#0]             \n"
    " 	LDR      r4,[r6,#0]             \n"
    " 	STR      r5,[r6,#0]             \n"
    " 	STR      r4,[%2,#0]             \n"
    " 	ADDS     %1,%1,#4               \n"
    " 	SUBS     r3,r3,#1               \n"
    " 	BNE      arm_bitreversal_16_0   \n"
  : /* no outputs */ 
  : "r" (pSrc), "r" (bitRevLen), "r" (pBitRevTab)
  : "r4", "r5", "r6", "memory"
  );
#else
    if (0U == bitRevLen)
    {
      return;
    }

  __ASM volatile (
 #if defined(__GNUC__)
        ".syntax unified                \n"
 #endif
    " 	ADDS     r3,%1,#1               \n"
    " 	ADDS     %1,%2,#2               \n"
    " 	LSRS     r3,r3,#2               \n"
    "arm_bitreversal_16_0:              \n" /* loop unrolled by 2 */
    " 	LDRH     r8,[%1,#4]             \n"
    " 	LDRH     r9,[%1,#2]             \n"
    " 	LDRH     %2,[%1,#0]             \n"
    " 	LDRH     r12,[%1,#-2]           \n"
    " 	ADD      r8,%0,r8,LSR #1        \n"
    " 	ADD      r9,%0,r9,LSR #1        \n"
    " 	ADD      %2,%0,%2,LSR #1        \n"
    " 	ADD      r12,%0,r12,LSR #1      \n"
    " 	LDR      r7,[r9,#0]             \n"
    " 	LDR      r6,[r8,#0]             \n"
    " 	LDR      r5,[%2,#0]             \n"
    " 	LDR      r4,[r12,#0]            \n"
    " 	STR      r6,[r9,#0]             \n"
    " 	STR      r7,[r8,#0]             \n"
    " 	STR      r5,[r12,#0]            \n"
    " 	STR      r4,[%2,#0]             \n"
    " 	ADDS     %1,%1,#8               \n"
    " 	SUBS     r3,r3,#1               \n"
    " 	BNE      arm_bitreversal_16_0   \n"
  : /* no outputs */ 
  : "r" (pSrc), "r" (bitRevLen), "r" (pBitRevTab)
  : "r4", "r5", "r6", "r7", "r8", "r9", "memory"
  );
#endif
}
