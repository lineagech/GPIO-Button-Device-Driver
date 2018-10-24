#include <linux/module.h>
#include <linux/platform_device.h>

MODULE_AUTHOR("Chia-Hao");
MODULE_DESCRIPTION("pca9685 driver for tiny4412");
MODULE_LICENSE("GPL");

#define DEVNAME ""
#define DYN_ALLOC 1

static struct platform_device pca9685_dev = {
	.name = DEVNAME
};

static int pca9685_driver_probe( struct platform_device* pdev )
{
	printk("%s\n", __func__);
}

static int pca9685_driver_remove( struct platform_device* pdev )
{
	printk("%s\n", __func__);
}

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
	
	err = platform_device_register(&pca9685_dev);
	if( err ) {
		pr_err("%s(#%d): platform_device_register failed", __func__, __LINE__, err);
		return err;
	} 
	
	err = platform_driver_register(&pca9685_driver);
	if( err ) {
		dev_err(&(pca9685_dev.dev),"%s(#%d): platform_driver_register failed", __func__, __LINE__, err);
		goto err_handle;
	} 
	
	return err;
	
err_handle:
	platform_device_unregister(&pca9685_dev);
	return err;
}

static void __exit pca9685_driver_exit(void)
{	
	dev_info(&(pca9685_dev.dev), "%s(#%d)\n", __func__, __LINE__);
	platform_device_unregister(&pca9685_dev);
    platform_driver_unregister(&pca9685_driver);
}

module_init(pca9685_driver_init);
module_exit(pca9685_driver_exit);


