#include <zephyr/kernel.h>

#define THREAD_STACK_SIZE 1024
#define THREAD1_PRIORITY 5
#define THREAD2_PRIORITY 6

struct k_msgq msgq;  
K_MSGQ_DEFINE(msgq, sizeof(bool), 1, 1); //defining a message queue

#define min_delay 1000 

void thread1(void)
{   int interval = min_delay; // Initial interval of 1s
    bool msg=false; 
    k_msleep(1000);
    while (1)
    {
        printk("thread 1\n");
        msg=1; //set msg to true after printing
        k_msgq_put(&msgq, &msg, K_NO_WAIT); //send message to thread2
        if (interval >= 10000) //condition to check the overflow
        {
            interval=min_delay; //reset the interval to 1s
        }
        else
        {
            interval += 1000; // Increment interval by 1 second
        }
        k_msleep(interval); //delay for next execution
    }
}

void thread2(void)
{
    bool recv_msg;
    while (1) {
        k_msgq_get(&msgq, &recv_msg, K_FOREVER); //read incoming message drom thread1
	    if(recv_msg)
        {
        k_msleep(1000); // to wait 1s after thread1
        printk("thread 2\n");
        printk("\n"); //to separate the print statements
        }
    }
}

//spawning the threads
K_THREAD_DEFINE(thread1_id,THREAD_STACK_SIZE, thread1, NULL, NULL, NULL,
		THREAD1_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread2_id,THREAD_STACK_SIZE, thread2, NULL, NULL, NULL,
		THREAD2_PRIORITY, 0, 0);

