/*
 * base_Time_Count.c
 *
 *  Created on: 2019年5月20日
 *      Author: Administrator
 */


#include "main.h"

uint32_t msAPI_Timer_DiffTime_2(uint32_t u32OldTime, uint32_t u32NewTime)
{
    if (u32OldTime > u32NewTime)
    {
        return (HAL_MAX_DELAY - u32OldTime + u32NewTime + 1);
    }
    else
    {
        return (u32NewTime - u32OldTime);
    }
}



// __weak void HAL_Delay(uint32_t Delay)
//{
//  uint32_t tickstart = HAL_GetTick();
//  uint32_t wait = Delay;
//
//  /* Add a freq to guarantee minimum wait */
//  if (wait < HAL_MAX_DELAY)
//  {
//    wait += (uint32_t)(uwTickFreq);
//  }
//
//  while ((HAL_GetTick() - tickstart) < wait)
//  {
//  }
//}


