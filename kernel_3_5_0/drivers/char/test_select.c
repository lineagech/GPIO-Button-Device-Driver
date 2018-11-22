#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <asm/uaccess.h>

#define DRIVER_NAME "test_select"

static unsigned int test_select_major = 0;
static unsigned int num_of_dev = 1;
static struct cdev test_select_cdev;
static unsigned int timeout_value = 10;

struct test_select_data {
    struct timer_list timeout;
    spinlock_t lock;
    wait_queue_head_t read_wait;
    int timeout_done;
    struct semaphore sem;
};

unsigned int test_select_poll(struct file *filp, poll_table *wait)
{
    struct test_select_data *data = filp->private_data;
    unsigned int mask = POLLOUT|POLLWRNORM;
    printk(KERN_ALERT "Call test_select_poll.\n");

    if (data == NULL)
        return -EBADFD;
    down(&data->sem);
    poll_wait(filp, &data->read_wait, wait);

    if (data->timeout_done == 1) { /* readable */
        mask |= POLLIN|POLLRDNORM;
    }
    up(&data->sem);
    printk(KERN_ALERT "%s returned (mask 0x%x)\n", __func__,  mask);
}

static void test_select_timeout(unsigned long arg)
{
    struct test_select_data *data = (struct test_select_data*)arg;
    unsigned long flags;
    printk(KERN_ALERT "Call test_select_timeout.\n");

    spin_lock_irqsave(&data->lock, flags);

    data->timeout_done = 1;
    wake_up_interruptible(&data->read_wait);

    spin_unlock_irqrestore(&data->lock, flags);
}

ssize_t test_select_write (struct file *filp, const char __user *buf, size_t count, loff_t *pos)
{
    return -EFAULT;
}

ssize_t test_select_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    struct test_select_data *data = filp->private_data;
    int i;
    unsigned char val;
    int retval;

    if (down_interruptible(&data->sem))
        return -ERESTARTSYS;
    if (data->timeout_done == 0) /* no read */
    {    
        up(&data->sem);
        if (filp->f_flags & O_NONBLOCK)    /* non-blocking mode */
            return -EAGAIN;
        do {
            retval = wait_event_interruptible_timeout(
                            data->read_wait,
                            data->timeout_done == 1,
                            1*HZ);
        if (retval == -ERESTARTSYS)
            return -ERESTARTSYS;
        } while (retval == 0);    /* timeout elapsed */

        if (down_interruptible(&data->sem))
            return -ERESTARTSYS;
    }
    val = 0xff;
    for (i = 0; i < count; i++) {
        if (copy_to_user(&buf[i], &val, 1)) {
            retval = -EFAULT;
            goto out;
        }
    }
    retval = count;

out:
    data->timeout_done = 0;
 
    /* restart timer */
    mod_timer(&data->timeout, jiffies + timeout_value*HZ);
    up(&data->sem);

    return retval;
}

static int test_select_close(struct inode *inode, struct file *filp)
{
    struct test_select_data *data = filp->private_data;
    printk(KERN_ALERT "Call test_select_close.\n");

    if (data) {
    del_timer_sync(&data->timeout);
    kfree(data);
    }

    return 0;
}

static int test_select_open(struct inode *inode, struct file *filp)
{
    struct test_select_data *data;
    printk(KERN_ALERT "Call test_select_open.\n");

    data = kmalloc(sizeof(struct test_select_data), GFP_KERNEL);
    if (data == NULL)
        return -ENOMEM;

    /* initialize members */
    spin_lock_init(&data->lock);
    init_waitqueue_head(&data->read_wait);

    // init_MUTEX(&data->sem); /* 新版 kernel 已不適用 */
    sema_init(&data->sem, 1);  /* 改用 sema_init */
    init_timer(&data->timeout);

    data->timeout.function = test_select_timeout;
    data->timeout.data = (unsigned long)data;

    filp->private_data = data;

    /* start timer */
    data->timeout_done = 0;
    mod_timer(&data->timeout, jiffies + timeout_value*HZ);

 return 0;
}

struct file_operations fops = {
 .owner = THIS_MODULE,
 .open = test_select_open,
 .release = test_select_close,
 .read = test_select_read,
 .write = test_select_write,
 .poll = test_select_poll,
};

static int test_select_init(void)
{
    dev_t dev = MKDEV(test_select_major, 0);
    int alloc_ret = 0;
    int cdev_ret = 0;

    alloc_ret = alloc_chrdev_region(&dev, 0, num_of_dev, DRIVER_NAME);
    if (alloc_ret)
        goto error;

    test_select_major = MAJOR(dev);
    cdev_init(&test_select_cdev, &fops);
    cdev_ret = cdev_add(&test_select_cdev, dev, num_of_dev);
    if (cdev_ret)
        goto error;
    printk(KERN_ALERT "%s driver(major: %d) installed.\n", DRIVER_NAME, test_select_major);
 
    return 0;
error:
    if (cdev_ret == 0)
        cdev_del(&test_select_cdev);
    if (alloc_ret == 0)
        unregister_chrdev_region(dev, num_of_dev);
 
 return -1;
}

static void test_select_exit(void)
{
    dev_t dev = MKDEV(test_select_major, 0);

    cdev_del(&test_select_cdev);
    unregister_chrdev_region(dev, num_of_dev);
 
    printk(KERN_ALERT "%s driver removed\n", DRIVER_NAME);
}

module_init(test_select_init);
module_exit(test_select_exit);

MODULE_LICENSE("GPL");
