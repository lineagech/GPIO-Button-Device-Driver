# Linux-Device-Driver

This porjects is a demo of combination of linux driver and OpenCV application:

![](https://github.com/lineagech/Linux-Device-Driver/blob/master/BD.JPG)

The repository is the root file system of tiny4412 development board. It can be found that self-developed driver source code is in [](kernel_3_5_0/drivers/char). I use linux 3.5 for tiny4412 to make driver modules. If you are interested in building linux drivers, you should download linux 3.5 first and then modify set_toolchain and source it to set up building environment.

'''
ln -s /dev/video15 /dev/videoa 
insmod PCA9685.ko
mknod /dev/pca9685 c 250 0
insmod tiny4412_button_driver.ko
'''

# Build Toolchain
'''
[https://mega.nz/#!qZpD2AgC!KuPWG25YRwV5u7pd7rLGIUemlf8g9QZ0GFTWh8KrrfA](https://mega.nz/#!qZpD2AgC!KuPWG25YRwV5u7pd7rLGIUemlf8g9QZ0GFTWh8KrrfA)
'''

# OpenCV
'''
[https://mega.nz/#!qZpD2AgC!KuPWG25YRwV5u7pd7rLGIUemlf8g9QZ0GFTWh8KrrfA](https://mega.nz/#!qZpD2AgC!KuPWG25YRwV5u7pd7rLGIUemlf8g9QZ0GFTWh8KrrfA)
'''
