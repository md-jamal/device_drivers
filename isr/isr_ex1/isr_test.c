#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

/*
 *Check the irq line of the keyboard by doing multiple times cat /proc/interrupts check which irq 
 *is incrementing and write the number here or pass as an argument
 */
static int irq = 17;
int dev_id = 4;
module_param(irq, int, S_IRUGO);
MODULE_LICENSE("GPL");


/*
 *When the kernel receives an interrupt, it invokes sequentially each registered handler on the line
 * Therefore it is important that the handler must be capable of distinguishing whether it generated a
 * given interrupt.(using some status registers)
 */



/*
 * An interrupt handler returns two special values: IRQ_NONE or IRQ_HANDLED.
 * The former is returned when the interrupt handler detects an interrupt for which its device was
 * not the originator.The latter is returned if the interrupt handler was correctly invoked,and its
 * device did indeed cause the interrupt
 */

static irqreturn_t irq_handler(int irq, void *devid)
{

	pr_info("%s:dev_id:%d\n","isr_test",*(int *)devid);
//	disable_irq(irq); //Dont do this this will make your mouse and keyboard to stop working
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
