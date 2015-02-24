#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/sched.h>


static int irq = 17;
int dev_id = 3;
module_param(irq, int, S_IRUGO);
MODULE_LICENSE("GPL");

/*
 * Readme File: Before understanding this
 *
 */

/*
 *
 *	When a process is in execution , the value of all the registers of the CPU, the status of the 
 *	process and the contents of the stack is called the context of the process.When the kernel
 *	needs to switch to another process, it needs to save all the state of the current process,
 *	that is , save the context of the current process,so that when the process is executed again
 *	will get the switching state down.In linux , the current process context is stored in 
 *	task_struct data structure.
 */




/*
 *	Linux maintains a structure of the kind task_struct for every process that runs in the system.
 *	This structure is defined in "linux/sched.h".
 *	
 *
 *	Kernel modules don't execute sequentially as applications do.most operations performed by the 
 *	kernel are related to a  specific process. Kernel code can know the current process driving it
 *	by accessing the global item current, a pointer to struct task_struct .It is included by
 *	linux/sched.h file
 *
 */


/*
 *
 *	in_interrupt(): Returns nonzero if in interrupt context and zero if in process context
 *
 */


static irqreturn_t irq_handler(int irq, void *devid)
{

	pr_info("%s: the process :%s pid:%i\n",__func__, current->comm, current->pid);
	if(!in_interrupt())
		pr_info("%s:in process context\n",__func__);
	else
		pr_info("%s:in interrupt context\n",__func__);
	return IRQ_HANDLED;
}

static int isr_init(void)
{
	int ret;
	printk("Hello World\n");

	/*
	 * IRQF_SHARED: 
	 *
	 */

	ret = request_irq(irq, irq_handler, IRQF_SHARED,
				KBUILD_MODNAME, &dev_id);
	/*
	 *Arguments:
	 *
	 * arg1: The interrupt number being requested
	 *
	 * arg2: Pointer to the handler function being called
	 *
	 * arg3: Flags whether you want the IRQ to be shared 
	 *
	 * arg4: Name in the /proc/interrupts file
	 *
	 * arg5: Pointer to the shared data.This will also be useful while freeing the irq when the
	 * interrupt is shared
	 */
	
	pr_info("The process is %s with pid:%i\n",current->comm, current->pid);
	if(!in_interrupt())
		pr_info("%s:in process context\n",__func__);
	else
		pr_info("%s:in interrupt context\n",__func__);

	return 0;
}


static void isr_exit(void)
{
	printk("Good Bye process:%s with pid:%i\n",current->comm, current->pid);
	free_irq(irq, &dev_id);

}
MODULE_DESCRIPTION(KBUILD_MODNAME); /*This macro expands to the module name*/
module_init(isr_init);
module_exit(isr_exit);
