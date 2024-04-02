/*
 * Copyright (c) 2006-2024, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-21     RT-Thread    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define KEY1_PIN    GET_PIN(A, 0)
#define BEEP_PIN    GET_PIN(A, 8)
#define LED_RED_PIN GET_PIN(B, 5)

int main(void)
{
    rt_pin_mode(KEY1_PIN, PIN_MODE_INPUT);
    rt_pin_mode(LED_RED_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        if (rt_pin_read(KEY1_PIN) == PIN_HIGH)
        {
            rt_pin_write(LED_RED_PIN, PIN_LOW);
        }
        else
        {
            rt_pin_write(LED_RED_PIN, PIN_HIGH);
        }

        rt_thread_mdelay(10);
    }

    return RT_EOK;
}
