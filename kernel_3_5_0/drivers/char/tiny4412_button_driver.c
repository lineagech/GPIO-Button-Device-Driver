#include <linux/init.h> // modules
#include <linux/module.h> // module
#include <linux/types.h> // dev_t type
#include <linux/fs.h> // chrdev allocation
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/fcntl.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#include <linux/timer.h>

#define DEVNUM_COUNT 			1
#define DEVNUM_NAME 			"buttons"
#define DEVNUM_MINOR_START 		0

dev_t devno;

static struct file_operation button_fops = {
	.owner 			= THIS_MODULE,
	.open 			= gpio_button_open,
	.read 			= gpio_button_read,
	.release 		= gpio_button_close,
	.poll 			= gpio_button_poll
};

struct button_dev {
	int gpio;
	int number;
	char* name;
	struct timer_list timer;
	struct cdev cdev;
};

static strcut button_dev buttons[] = {
	{ EXYNOS4_GPX3(2), 0, "KEY0" },
	{ EXYNOS4_GPX3(3), 1, "KEY1" },
	{ EXYNOS4_GPX3(4), 2, "KEY2" },
	{ EXYNOS4_GPX3(5), 3, "KEY3" },
};

/* Declare wait queue*/
static DECLARE_WAIT_QUEUE_HEAD(button_waitq);
static unsigned int button_press = 0;

void gpio_button_timer_hanlder(unsigned long _data)
{
	
}

static irqreturn_t gpio_interrupt_handler (int irq, void *dev_id)
{
	struct button_dev* dev = (struct button_dev*)dev_id;
	mod_timer(&dev->timer, jiffies + msecs_to_jiffies(40));
	return IRQ_HANDLED;
}

/* struct file 
 * 		.mode_t f_mode; // FMODE_READ, FMODE_WRITE
 * 		.loff_t f_pos;  // 64-bit, current position
 * 		.unsigend int f_flags; // O_RDONLY, O_NONBLOCK, O_SYNC
 * 		.struct file_operations* f_op;
 * 		.void* private_data; 
 * 		.struct dentry* f_dentry; // directory entry
 * */
 
 /* inode 
  * 	.dev_t i_rdev; // device files, the field contains the actual device number
  * 	.struct cdev* i_cdev; // kernel internal structure that represents char devices
  * */
 
static int gpio_button_open (struct inode* inode, struct file* file)
{
	
}
static ssize_t gpio_button_read (struct file* file, char __user* buff, size_t count, loff_t* p_off)
{
	
}
static int gpio_button_close(struct inode* inode, struct file* file)
{
	
}
static unsigned int gpio_button_poll(struct file* file, struct poll_table_struct* p_poll_table)
{
	
	/* return a bit mask indicating whether non-blocking reads or writes are possible */
}

static void buttons_setup_dev(struct button_dev* p_button_dev, int index)
{
	int err;
	cdev_init(&p_button_dev->c_dev, &button_fops);
	p_button_dev->cdev.owner = THIS_MODULE;
	p_button_dev->cdev.ops = &button_fops;
	err = cdev_add(&p_button_dev->cdev, devno, 1);
	if(err){
		printk(KERN_NOTICE "Error %d adding button", err, index);
	}
}

static void init_err_hanlder(int i)
{	
	int irq;
	for(; i >= 0; i--)
	{
		/* Delete timer */
		del_timer_sync(&button_dev[i].timer);
		
		/* Close IRQ */
		irq = gpio_to_irq(button_dev[i].gpio);
		disable_irq(irq);
		free_irq(irq, (void *)&button_dev[i]);
	}
}

static int __init tiny4412_buttons_init(void)
{	
	int i;
	int irq;
	int err;
	for( i = 0; i < ARRAY_SIZE(buttons); i++ )
	{
		/* Allocate Device Major Number and Minor Number */
		printk(KERN_INFO "dynamically allocate major and minor number...\n");
		int alloc_re = 0;
		alloc_re = alloc_chrdev_region(&devno, DEVNUM_MINOR_START, DEVNUM_COUUNT, DEVNUM_NAME);
		if( alloc_re ) {
			printk(KERN_WARNING "%s : could not allocate device number", __func__);
		}	
		else {
			printk(KERN_WARNING "%s : register with major number %l and minor number %l", __func__, MAJOR(dev), MINOR(dev));
		}
	
		/* Register Character Devices */
		buttons_setup_dev(&button_dev, i);
		
		/* Set up timer */
		init_timer( &button_dev[i].timer );
		setup_timer( &button_dev[i].timer, gpio_button_timer_hanlder, &button_dev[i] );
		
		/* Initialize IRQ */
		irq = gpio_to_irq(button_dev[i].gpio); // translate to irq number
		err = request_irq(irq, gpio_interrupt_handler, IRQ_TYPE_EDGE_FALLING, 
				  button_dev[i].name, (void *)&button_dev[i]);
		
		if( err ){
			init_err_hanlder(i);
			break;	
		}
	}
}

static int __exit tiny4412_buttons_exit(void)
{
	int i;
	int irq;
	int err;
	for( i = 0; i < ARRAY_SIZE(buttons); i++ )
	{	
		/* Unregister major/minor number */
		int major_num = MAJOR(devno);
		int minor_num = MINOR(devno);
		unregister_chrdev_region(MKDEV(major_num, minor_num), DEVNUM_MINOR_START, DEVNUM_COUUNT , DEVNUM_NAME);
		
		/* Delete timer */
		del_timer_sync(&button_dev[i].timer);
		
		/* Close irq */
		irq = gpio_to_irq(button_dev[i].gpio);
		disable_irq(irq);
		free_irq(irq, (void *)&button_dev[i]);
	}
}

