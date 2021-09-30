/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-24     yangjie      the first version
 • 理解多线程时间片轮转的基本原理；
• 理解同优先级线程间的时间片轮转机制；
• 在 RT-Thread 中熟练使用时间片轮转来完成需求。
为了体现时间片轮转，本实验设计了 thread1、thread2 两个相同优先级的线程，thread1 时间片为 10，
thread2 时间片为 5，如果就绪列表中该优先级最高，则这两个线程会按照时间片长短被轮番调度。两个线
程采用同一个入口函数，分别打印一条带有累加计数的信息（每个线程进入一次入口函数会将计数 count+
+，count>200 时线程退出）遵循时间片轮转调度机制。
 */

/*
* 程序清单：相同优先级线程按照时间片轮转调度
*
* 这个例子中将创建两个线程，每一个线程都在打印信息
*
*/

#include <rtthread.h>

#define THREAD_STACK_SIZE   1024
#define THREAD_PRIORITY     20
#define THREAD_TIMESLICE    10

/* 线程入口 */
static void thread_entry(void *parameter)
{
    rt_uint32_t value;
    rt_uint32_t count = 0;

    value = (rt_uint32_t)parameter;
    while (1)
    {
        if (0 == (count % 5))
        {
            rt_kprintf("thread %d is running ,thread %d count = %d\n", value, value, count);

            if (count > 200)
                return;
        }
        count++;
    }
}

int timeslice_sample(void)
{
    rt_thread_t tid = RT_NULL;
    /* 创建线程1 */
    tid = rt_thread_create("thread1",
                           thread_entry, (void *)1,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY, THREAD_TIMESLICE);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    /* 创建线程2 */
    tid = rt_thread_create("thread2",
                           thread_entry, (void *)2,
                           THREAD_STACK_SIZE,
                           THREAD_PRIORITY, THREAD_TIMESLICE - 5);
    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(timeslice_sample, timeslice sample);

