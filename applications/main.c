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

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define PA8 8
#define PB0 16
#define WDT_DEVICE_NAME    "wdt"

static rt_device_t wdg_dev;
int count = 0;

static void idle_hook(void)
{
    if (count > 10) {
        return;
    }

    /* 在空闲线程的回调函数里喂狗 */
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, RT_NULL);
    LOG_D("feed the dog!\n");

    count++;
}

int main(void)
{
    int count = 1;

    wdg_dev = rt_device_find(WDT_DEVICE_NAME);
    if (!wdg_dev)
    {

        LOG_D("find %s failed!\n", WDT_DEVICE_NAME);
        return RT_ERROR;
    }

    rt_device_init(wdg_dev);

    rt_uint32_t timeout = 20;
    /* 设置看门狗溢出时间 */
    if (rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, &timeout) != RT_EOK)
    {
        LOG_D("set dog timeout failed!\n");
        return RT_ERROR;
    }

    /* 启动看门狗 */
    if (rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_START, RT_NULL) != RT_EOK)
    {
        LOG_D("start dog failed!\n");
        return -RT_ERROR;
    }

    /* 设置空闲线程回调函数 */
    rt_thread_idle_sethook(idle_hook);

    rt_pin_mode(PB0, PIN_MODE_OUTPUT);
    rt_pin_mode(PA8, PIN_MODE_OUTPUT);

    rt_pin_write(PA8, PIN_HIGH);
    rt_thread_mdelay(500);
    rt_pin_write(PA8, PIN_LOW);

    int status = PIN_HIGH;
    while (count++)
    {
        rt_pin_write(PB0, status);
        status = !status;
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}
