#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

/*
 *Workqueues have a type of struct workqueue_struct which is defined in <linux/workqueue.h>
 *
 */

MODULE_LICENSE("GPL");


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
DECLARE_DELAYED_WORK(wq1, deferred_fn1);

struct workqueue_struct *workqueue;


static int workqueue_init(void)
{
	pr_info("Hello World\n");
	pr_info("Jiffies:%ld\n",jiffies);

	workqueue = create_workqueue("Jamal");
	/*
	 *	Second argument : Number of ticks (jiffies) to wait
	 */
	
	schedule_delayed_work(&wq, 10);
	queue_delayed_work(workqueue, &wq1, 10);

	return 0;
}


static void workqueue_exit(void)
{
	destroy_workqueue(workqueue);
	pr_info("Good Bye\n");
}

module_init(workqueue_init);
module_exit(workqueue_exit);
