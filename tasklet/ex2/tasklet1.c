#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/sched.h>


MODULE_LICENSE("GPL");
char data[] = "Hello Kernel" ;


static void task_handler(unsigned long task_data)
{
	pr_info("%s\n",(char *)task_data);
	pr_info("%s:context:%s\n",__func__,current->comm);
	pr_info("pid:%d\n",current->pid);
	return;
}


static DECLARE_TASKLET_DISABLED(my_tasklet, task_handler, (unsigned long)&data);

static int __init hello_init(void)
{
	pr_info("Hello World\n");
	tasklet_enable(&my_tasklet);
	tasklet_schedule(&my_tasklet);
	return 0;
}


static void hello_exit(void)
{
	tasklet_kill(&my_tasklet);
	pr_info("Good Bye\n");
}

MODULE_DESCRIPTION(KBUILD_MODNAME);
module_init(hello_init);
module_exit(hello_exit);
