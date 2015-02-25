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
	pr_info("%s:Jiffies:%ld\n",__func__,jiffies);
	pr_info("%s:context:%s\n",__func__,current->comm);
}


void deferred_fn1(struct work_struct *ptr)
{
	pr_info("%s:Jiffies:%ld\n",__func__,jiffies);
	pr_info("%s:context:%s\n",__func__,current->comm);
}

DECLARE_WORK(wq1, deferred_fn1);
DECLARE_DELAYED_WORK(wq, deferred_fn);
static int workqueue_init(void)
{
	pr_info("%s:Jiffies:%ld\n", __func__, jiffies);
	/*
	 *	Second argument : Number of ticks (jiffies) to wait
	 */
	schedule_delayed_work(&wq, 10);
	flush_scheduled_work();
	schedule_work(&wq1);
	flush_scheduled_work();
	pr_info("%s:Jiffies:%ld\n", __func__,jiffies);
	return 0;
}


static void workqueue_exit(void)
{
	pr_info("Good Bye\n");
}

module_init(workqueue_init);
module_exit(workqueue_exit);
