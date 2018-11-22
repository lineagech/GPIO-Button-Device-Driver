ln -s /dev/video15 /dev/videoa
insmod PCA9685.ko
mknod /dev/pca9685 c 250 0
insmod tiny4412_button_driver.ko
