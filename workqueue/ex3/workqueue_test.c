#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

/*
 *Workqueues have a type of struct workqueue_struct which is defined in <linux/workqueue.h>
 *
 */

MODULE_LICENSE("GPL");

char data[]  = "Deferred Work";

void deferred_fn(struct work_struct *ptr)
{
	pr_info("Jiffies:%ld\n",jiffies);
	pr_info("%s:context:%s\n",__func__,current->comm);
}


struct work_struct wq;

static int workqueue_init(void)
{
	pr_info("Hello World\n");
	pr_info("Jiffies:%ld\n",jiffies);
	INIT_WORK(&wq, deferred_fn);
	/*
	 *	Second argument : Number of ticks (jiffies) to wait
	 */
	schedule_work(&wq);
	return 0;
}


static void workqueue_exit(void)
{
	pr_info("Good Bye\n");
}

module_init(workqueue_init);
module_exit(workqueue_exit);
