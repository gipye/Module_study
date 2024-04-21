#include <stdio.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char **argv)
{
	int dev;
	int res;

	printf("Device driver test.\n");

	dev = open("/dev/virtual_device", O_RDWR);
	if (dev < 0) {
		fprintf(stderr, "device file open error\n");
		return -1;
	}

	res = ioctl(dev, 1);
	if (res < 0)
		fprintf(stderr, "ioctl error(%d): %s\n", errno, strerror(errno));

	res = ioctl(dev, 2);
	if (res < 0)
		fprintf(stderr, "ioctl error(%d): %s\n", errno, strerror(errno));

	res = ioctl(dev, 3);
	if (res < 0)
		fprintf(stderr, "ioctl error(%d): %s\n", errno, strerror(errno));

	close(dev);
	return 0;
}
