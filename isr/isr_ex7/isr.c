#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

static int irq = 17;
int dev_id = 3;
module_param(irq, int, S_IRUGO);
MODULE_LICENSE("GPL");
struct timer_list list;
unsigned long flags;

static irqreturn_t irq_handler(int irq, void *devid)
{

	pr_info("%s:dev_id:%d\n",__func__,*(int *)devid);
	return IRQ_HANDLED;
}

void timer_fn(unsigned long data)
{
	pr_info("%s\n",__func__);
}

/*
 *	Enabling and disabling interrupts
 *
 *	Linux provides these functions for enabling and disabling interrupts
 *	void enable_irq(int irq);
 *	void disable_irq(int irq);
 *	void disable_irq_nosync(int irq);
 *
 *	Enabling and disabling interrupts through these functions results in updating the interrupt mask
 *	for the specified irq in the PIC(Programmable Interrupt Controller).This is visible for all
 *	processors.Disabling an interrupt waits for the completion of the currently executing interrupt
 *	handler(if any).But disabling through disable_irq_nosync returns immediately.
 *
 *
 *	Enabling and disabling interrupts for the current processor can be done by using
 *	void local_irq_save(unsigned long flags);
 *	void local_irq_disable(void);
 *
 *	Calling local_irq_save() will disable all the interrupt on the current processor and prior to 
 *	which it saves the current interrupt state into the flags (passed to this fn as argument).
 *	local_irq_disable() will disable all the interrupts but will not save the state
 *
 *	vod local_irq_restore(unsigned long flags);
 *	void local_irq_enable(void);
 *
 *	local_irq_restore() enables interrupts and restores states using flags.The local_irq_enable()
 *	function enables interrupts unconditionally
 *
 *
 *
 */


static int __init isr_init(void)
{
	int ret;
	printk("Hello World\n");


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

	local_irq_save(flags);
	/*This can be critical Section */	
	init_timer(&list);

	list.data	= 0x00;
	list.function   = timer_fn;
	list.expires    = jiffies + 22000;

	add_timer(&list);
	pr_info("Flags of irq:%lx\n",flags);

	local_irq_restore(flags);


	return 0;
}


static void isr_exit(void)
{
	printk("Good Bye\n");
	free_irq(irq, &dev_id);

}
MODULE_DESCRIPTION(KBUILD_MODNAME); /*This macro expands to the module name*/
module_init(isr_init);
module_exit(isr_exit);
