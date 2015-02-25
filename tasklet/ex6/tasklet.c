#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/sched.h>


MODULE_LICENSE("GPL");
char data[] = "Hello Kernel" ;

static int irq = 17;
int dev_id = 3;
module_param(irq, int, S_IRUGO);

static void task_handler(unsigned long task_data)
{
	pr_info("%s\n",(char *)task_data);
	pr_info("%s:context:%s\n",__func__,current->comm);
	pr_info("pid:%d\n",current->pid);
	return;
}


struct tasklet_struct my_tasklet;

static irqreturn_t my_intr(int irq, void *id)
{

	pr_info("%s:context:%s\n",__func__,current->comm);
	tasklet_schedule(&my_tasklet);
	return IRQ_NONE;
}


static int __init hello_init(void)
{
	pr_info("Hello World\n");
	tasklet_init(&my_tasklet, task_handler, (unsigned long)&data);
	return request_irq(irq, my_intr, IRQF_SHARED, KBUILD_MODNAME, &dev_id);
}


static void hello_exit(void)
{
	tasklet_kill(&my_tasklet);
	free_irq(irq, &dev_id);
	pr_info("Good Bye\n");
}

MODULE_DESCRIPTION(KBUILD_MODNAME);
module_init(hello_init);
module_exit(hello_exit);
