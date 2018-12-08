# Linux-Device-Driver

This porjects is the combination of linux drivers and OpenCV application: ![Demo](https://www.youtube.com/watch?v=fUSyv9Pux14&feature=youtu.be)

![](https://github.com/lineagech/Linux-Device-Driver/blob/master/BD.JPG)

The repository is the root file system of tiny4412 development board. It can be found that self-developed driver source code is in [](kernel_3_5_0/drivers/char). I use linux 3.5 for tiny4412 to make driver modules. If you are interested in building linux drivers, you should download linux 3.5 first and then modify [set_toolchain](https://github.com/lineagech/Linux-Device-Driver/blob/master/set_toolchain) and source it to set up building environment.

# Install Driver on the Tiny4412
```
ln -s /dev/video15 /dev/videoa 
insmod PCA9685.ko
mknod /dev/pca9685 c 250 0
insmod tiny4412_button_driver.ko
```

# Build Toolchain
[https://mega.nz/#!qZpD2AgC!KuPWG25YRwV5u7pd7rLGIUemlf8g9QZ0GFTWh8KrrfA](https://mega.nz/#!qZpD2AgC!KuPWG25YRwV5u7pd7rLGIUemlf8g9QZ0GFTWh8KrrfA)
```
sudo tar -jxvf arm-4.5.1.tar.bz2 -C /usr/local
vim ~/.bashrc
export PATH=/usr/local/arm/4.5.1/bin:$PATH
source ~/.bashrc
```

# luvcview 
[https://mega.nz/#!iEpnkYTJ!-4zJL1hINZJpK4BV8tWJrShpd3AXhpxym65LoQc753I(https://mega.nz/#!iEpnkYTJ!-4zJL1hINZJpK4BV8tWJrShpd3AXhpxym65LoQc753I)
```
arm-linux-gcc -c luvcview.c -o luvcview.o
arm-linux-gcc -shared -Wl,-soname,libluvcview.so.1 -o libluvcview.so.1.0.0 v4l2uvc.o utils.o luvcview.o color.o avilib.o
ln -s libluvcview.so.1.0.0 libluvcview.so.1
ln -s libluvcview.so.1 libluvcview.so
sudo cp libluvcview.* /usr/local/arm/4.5.1/arm-none-linux-gnueabi/lib
sudo cp libluvcview.* ~/root_mkfs/lib
```

# fbv-0.99
[https://mega.nz/#!6Z5z2SYA!hQyKZZ-2a89RyGF6oWDDw_jI90hsV7CIWZHxBJQBvAs](https://mega.nz/#!6Z5z2SYA!hQyKZZ-2a89RyGF6oWDDw_jI90hsV7CIWZHxBJQBvAs)
```
arm-linux-gcc -c fb_display.c -o fb_display.o
arm-linux-gcc -shared -Wl,-soname,libfb_display.so.1 -o libfb_display.so.1.0.0 fb_display.o
ln -s libfb_display.so.1.0.0 libfb_display.so.1
ln -s libfb_display.so.1 libfb_display.so
sudo cp libfb_display.* /usr/local/arm/4.5.1/arm-none-linux-gnueabi/lib
sudo cp libfb_display.* ~/root_mkfs/lib
```

# OpenCV
[https://mega.nz/#F!WAwjCIKS!ZNMhkVfIJ_GsI4mXMM-Crw](https://mega.nz/#F!WAwjCIKS!ZNMhkVfIJ_GsI4mXMM-Crw)
```
sudo apt-get install cmake-qt-gui
tar xvf OpenCV-2.4.0.tar.bz2
cd OpenCV-2.4.0
cmake-gui
whereis the source code:/home/cadtc/OpenCV-2.4.0
where to build teh binaryes:/home/cadtc/OpenCV-2.4.0
Tools-->configure:
Specify options for cross-compiling
	Operating System:	Linux
	Processor:		arm	
	C Compilers:		/usr/local/arm/4.5.1/bin/arm-none-linux-gnueabi-gcc
	C++ Compilers:		/usr/local/arm/4.5.1/bin/arm-none-linux-gnueabi-g++
	Target Root:		/usr/local/arm/4.5.1/arm-none-linux-gnueabi/
vi CmakeCache.txt
	CMAKE_INSTALL_PREFIX:	/usr/local/arm/4.5.1/arm-none-linux-gnueabi/
	CMAKE_EXE_LINKER_FLAGS:	-lpthread -lrt
make -j4
gedit cmake_install.cmake
  SET(CMAKE_INSTALL_PREFIX "/usr/local/arm/4.5.1/arm-none-linux-gnueabi/")
sudo make install
cd /usr/local/arm/4.5.1/arm-none-linux-gnueabi/lib
sudo cp libopencv* ~/root_mkfs/lib
sudo cp libstdc* ~/root_mkfs/lib
```

# FC application
[https://mega.nz/#F!SVxhTYII!7N7jj21ih4ffm1ie15WcwQ](https://mega.nz/#F!SVxhTYII!7N7jj21ih4ffm1ie15WcwQ)
```
arm-linux-g++ -o FCEXE FC.c -I. -lz -lpthread -lrt -lpng -ljpeg -lluvcview -lopencv_highgui -lopencv_objdetect -lopencv_imgproc -lopencv_core -lfb_display

sudo cp FCEXE ~/root_mkfs/
sudo cp lbpcascade_frontalface.xml ~/root1_mkfs
sudo cp haarcascade_* ~/root_mkfs
```
