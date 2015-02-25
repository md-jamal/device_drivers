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


void deferred_fn1(struct work_struct *ptr)
{
	pr_info("Jiffies:%ld\n",jiffies);
	pr_info("%s:context:%s\n",__func__,current->comm);
}

DECLARE_DELAYED_WORK(wq, deferred_fn);
struct delayed_work wq1;

static int workqueue_init(void)
{
	pr_info("Hello World\n");
	pr_info("Jiffies:%ld\n",jiffies);
	INIT_DELAYED_WORK(&wq1, deferred_fn1);

	/*
	 *	Second argument : Number of ticks (jiffies) to wait
	 */
	schedule_delayed_work(&wq, 10);
	schedule_delayed_work(&wq1, 10);
	return 0;
}


static void workqueue_exit(void)
{
	pr_info("Good Bye\n");
}

module_init(workqueue_init);
module_exit(workqueue_exit);
