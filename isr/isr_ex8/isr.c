#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

static int irq = 17;
int dev_id = 3;
module_param(irq, int, S_IRUGO);
MODULE_LICENSE("GPL");

static irqreturn_t irq_handler(int irq, void *devid)
{

	pr_info("%s:dev_id:%d\n",__func__,*(int *)devid);
	if(in_interrupt())
		pr_info("%s:interrupt context\n",__func__);
	else
		pr_info("%s:process context\n",__func__);
//	disable_irq(irq); //Dont do this this will make your mouse and keyboard to stop working
	return IRQ_WAKE_THREAD;
}


static irqreturn_t irq_wakeup(int irq, void *devid)
{
	pr_info("%s:dev_id:%d\n",__func__,*(int *)devid);

	if(in_interrupt())
		pr_info("%s:interrupt context\n",__func__);
	else
		pr_info("%s:process context\n",__func__);

	return IRQ_NONE;

}

static int isr_init(void)
{
	int ret;
	printk("Hello World\n");

	/*
	 * IRQF_SHARED: 
	 *
	 */

	ret = request_threaded_irq(irq, irq_handler, irq_wakeup, IRQF_SHARED,
				KBUILD_MODNAME, &dev_id);

	/*
	 *If the second argument is NULL then directly it calls the third argument .This will 
	 *not work for shared irqs i think
	 * 
	 */

//	ret = request_threaded_irq(irq, NULL, irq_wakeup, IRQF_SHARED,
//					KBUILD_MODNAME, &dev_id);

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
	
	return ret;
}


static void isr_exit(void)
{
	printk("Good Bye\n");
	synchronize_irq(irq);
	free_irq(irq, &dev_id);

}
MODULE_DESCRIPTION(KBUILD_MODNAME); /*This macro expands to the module name*/
module_init(isr_init);
module_exit(isr_exit);
