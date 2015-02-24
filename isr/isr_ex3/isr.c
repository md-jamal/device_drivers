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
//	disable_irq(irq); //Dont do this this will make your mouse and keyboard to stop working
	return IRQ_HANDLED;
}

static int isr_init(void)
{
	int ret;
	printk("Hello World\n");

	/*
	 * IRQF_TRIGGER_NONE: It is a flag to simply continue use the interrupt simply like it has 
	 * already been setup(in bootloader,or earlier in kernel)
	 *
	 */

	/*
	 *IRQF_SHARED : Use when we you want the interrupt line to be shared
	 *
	 */


	ret = request_irq(irq, irq_handler, IRQF_TRIGGER_NONE,
				KBUILD_MODNAME, &dev_id);

	/*ret = request_irq(irq, irq_handler, IRQF_SHARED,
				KBUILD_MODNAME, &dev_id);
	*/

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
