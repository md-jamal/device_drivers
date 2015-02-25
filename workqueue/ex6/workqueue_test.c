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
	pr_info("%s:Jiffies:%ld\n",__func__, jiffies);
	pr_info("%s:context:%s\n",__func__, current->comm);
}


void deferred_fn1(struct work_struct *ptr)
{
	pr_info("%s:Jiffies:%ld\n",__func__, jiffies);
	pr_info("%s:context:%s\n",__func__, current->comm);
}

DECLARE_WORK(wq, deferred_fn);
DECLARE_DELAYED_WORK(wq1, deferred_fn1);


static int workqueue_init(void)
{
	pr_info("%s:Jiffies:%ld\n",__func__, jiffies);
	/*
	 *	Second argument : Number of ticks (jiffies) to wait
	 */
	schedule_work(&wq);
	schedule_delayed_work(&wq1, 10);
	cancel_delayed_work(&wq1);

	pr_info("%s:Jiffies:%ld\n",__func__, jiffies);
	return 0;
}


static void workqueue_exit(void)
{
	pr_info("%s:Good Bye\n",__func__);
}

module_init(workqueue_init);
module_exit(workqueue_exit);
