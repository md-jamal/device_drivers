#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

static int irq = 17;
int dev_id = 3;
module_param(irq, int, S_IRUGO);
struct timer_list list;

MODULE_LICENSE("GPL");

static irqreturn_t irq_handler(int irq, void *devid)
{

	pr_info("%s:dev_id:%d\n",__func__,*(int *)devid);
//	disable_irq(irq); //Dont do this this will make your mouse and keyboard to stop working
	return IRQ_HANDLED;
}

void timer_fn(unsigned long data)
{
	pr_info("%s\n",__func__);
	enable_irq(irq);
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

	init_timer(&list);
	list.data     = (unsigned long)0x1234;
	list.function = timer_fn;
	list.expires  = jiffies+25000;
	
	add_timer(&list);

	/*Disables the irq on the current line*/
	disable_irq(irq);

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
