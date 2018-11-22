#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main()
{
	int dev_fd;
	dev_fd = open("/dev/pca9685",O_RDWR | O_NONBLOCK);

        sleep(1);
	ioctl(dev_fd,200,0);
	sleep(1);
	ioctl(dev_fd,108,0);


	return 0;
}
