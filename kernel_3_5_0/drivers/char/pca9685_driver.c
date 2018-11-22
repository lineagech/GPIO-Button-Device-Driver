#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>

MODULE_AUTHOR("Chia-Hao");
MODULE_DESCRIPTION("pca9685 driver for tiny4412");
MODULE_LICENSE("GPL");

#define DRIVER_NAME "pca9685"
#define DYN_ALLOC 1

static unsigned int chrdev_reg_major = 0;

/*
static struct platform_device pca9685_dev = {
	.name = DEVNAME
};
*/

//static struct platform_device* pca9685_dev;
static struct i2c_client *client_pca9685;

static const struct i2c_device_id pca9685_id[] = {
	{
 		.name = "pca9685",
 		.driver_data = 0,
	},
};

/*
static int pca9685_driver_probe( struct platform_device* pdev )
{
	printk("%s\n", __func__);
	return 0;
}
* */
static int pca9685_driver_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	client_pca9685 = client;

	i2c_smbus_write_byte_data(client,0xFE,121);
	i2c_smbus_write_byte_data(client,0x00,0);
	i2c_smbus_write_byte_data(client,0x06,0);
	i2c_smbus_write_byte_data(client,0x07,0);
	i2c_smbus_write_byte_data(client,0x08,0x67);
	i2c_smbus_write_byte_data(client,0x09,0x00);
	printk("client_pca9685 -> %d, client-> %d\n", (int)client_pca9685, (int)client);
	return 0;
}

/*
static int pca9685_driver_remove( struct platform_device* pdev )
{
	printk("%s\n", __func__);
	return 0;
}
*/
static int pca9685_driver_remove(struct i2c_client *client)
{
	i2c_smbus_write_byte_data(client,0x08,0x99);
	i2c_smbus_write_byte_data(client,0x09,0x01);
	return 0;
}

static struct i2c_driver pca9685_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name	= "pca9685"
	},
	.id_table	= pca9685_id,
	.probe		= pca9685_driver_probe,
	.remove		= pca9685_driver_remove,
};

static int pca9685_driver_open(struct inode *inode, struct file *file)
{
	printk("pca9685_driver_open\n");
	return 0;
}

static int pca9685_driver_close(struct inode *inode, struct file *file)
{
	printk("pca9685_driver_close\n");
    return 0;
}

int pca9685_driver_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	printk("pca9685_ioctl  %d --\n",(int)cmd);
	i2c_smbus_write_byte_data(client_pca9685,0x08,((int)cmd)&0xFF);
	i2c_smbus_write_byte_data(client_pca9685,0x09,((int)cmd)>>8);

	return 0;
}

/*
static struct platform_driver pca9685_driver = {
	.driver = {
		.name = DEVNAME,
		.owner = THIS_MODULE
	},
	.probe = pca9685_driver_probe,
	.remove = pca9685_driver_remove,
};

static int __init pca9685_driver_init(void)
{
	int err;
	printk("%s\n", __func__);
	
	//err = platform_device_register(&pca9685_dev);
	pca9685_dev = platform_device_alloc(DEVNAME, 0);
	if( err ) {
		pr_err("%s(#%d): platform_device_alloc failed. %d", __func__, __LINE__, err);
		return -ENOMEM;
	} 
	
	err = platform_device_add(pca9685_dev);
	if( err ) { 
		pr_err("%s(#%d): platform_device_add failed. %d", __func__, __LINE__, err);
		goto platform_device_add_err_handle;
	}	
	
	err = platform_driver_register(&pca9685_driver);
	if( err ) {
		dev_err(&(pca9685_dev->dev),"%s(#%d): platform_driver_register failed. %d", __func__, __LINE__, err);
		goto platform_driver_register_err_handle;
	} 
	
	return err;
	
platform_device_add_err_handle:
	platform_device_put(pca9685_dev);

platform_driver_register_err_handle:
	platform_device_unregister(pca9685_dev);
	return err;
}

static void __exit pca9685_driver_exit(void)
{	
	dev_info(&(pca9685_dev->dev), "%s(#%d)\n", __func__, __LINE__);
	platform_device_unregister(pca9685_dev);
    platform_driver_unregister(&pca9685_driver);
}
*/
struct file_operations fops = {
	.owner	 = THIS_MODULE,
    .open	 = pca9685_driver_open,
    .release = pca9685_driver_close,
	.unlocked_ioctl = pca9685_driver_ioctl,
};

static int __init pca9685_driver_init(void)
{
	int ret;
	ret = register_chrdev(chrdev_reg_major, DRIVER_NAME, &fops);
	chrdev_reg_major = ret;
	return i2c_add_driver(&pca9685_driver);
}

static void __exit pca9685_driver_exit(void)
{
	unregister_chrdev(chrdev_reg_major, DRIVER_NAME);
	i2c_del_driver(&pca9685_driver);
}

module_init(pca9685_driver_init);
module_exit(pca9685_driver_exit);


