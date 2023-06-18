#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024
#define THREAD1_PRIORITY 5
#define THREAD2_PRIORITY 6

struct k_msgq msgq; 

int interval=1000; // Initial interval of 1s

void thread1(void)
{
    int count = 0;
    while (1)
    {
        k_msleep(1000); //thread1 sleeps for 1 sec
        printk("thread 1\n");
        if (++count >= 10) //condition to check the overflow
        {
            count = 1;
            interval = 1000; // Reset interval to 1 second
        }
        else
        {
            interval += 1000; // Increment interval by 1 second
        }
        k_msgq_put(&msgq, &interval, K_FOREVER); //send message to thread2 
    }
}

void thread2(void)
{
    char buffer[10];
    k_msgq_init(&msgq, buffer, sizeof(buffer), 1);
    while (1) {
        k_msgq_get(&msgq, &interval, K_FOREVER); //read incoming message drom thread1
	k_msleep(1000); // to wait 1s after thread1
        printk("thread 2\n");
    }
}

//spawning the threads
K_THREAD_DEFINE(thread1_id,THREAD_STACK_SIZE, thread1, NULL, NULL, NULL,
		THREAD1_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread2_id,THREAD_STACK_SIZE, thread2, NULL, NULL, NULL,
		THREAD2_PRIORITY, 0, 0);

