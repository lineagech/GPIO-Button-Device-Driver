#include <linux/init.h> // modules
#include <linux/module.h> // module
#include <linux/types.h> // dev_t type
#include <linux/fs.h> // chrdev allocation
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/fcntl.h>

#include <linux/timer.h>

#define DEVNUM_COUNT 			4
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
};

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

static void buttons_setup_dev(struct buttons_dev* buttons_dev, int index)
{
	int err;
	cdev_init(&dev->c_dev, &button_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &button_ops;
	err = cdev_add(&dev->cdev, devno, 1);
	if(err){
		printk(KERN_NOTICE "Error %d adding button", err, index);
	}
}

static int __init tiny4412_buttons_init(void)
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
}

static int __exit tiny4412_buttons_exit(void)
{
	
}







































