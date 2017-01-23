#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/cdrom.h>
#include <errno.h>

void print_ioctl_error()
{
    switch (errno) {
        case ENOTTY:
        case EBADF:
            puts("Invalid file descriptor for this device, or the device is not an optical drive"); 
            break;
        case EFAULT:
            puts("Referenced an inaccessible memory area");
            break;
        case EINVAL:
            puts("Invalid request or argument");
            break;
        default:
            printf("An unknown error %d occured. Is given device an optical drive?\n", errno);
    }
}

int main(int argc, char **argv) {
    int fd;
    int result;

    if (argc != 2) {
        printf("Run as `%s dev` where `dev` is a cdrom device like `/dev/sr0`\n", argv[0]);
        return(1);
    }

    char* device = argv[1];

    //According to /usr/include/linux/cdrom.h correct arguments for
    //getting the handle via open() for cdroms are O_RDONLY | O_NONBLOCK
    fd = open(device, O_RDONLY | O_NONBLOCK);

    if (fd < 0) {
        printf("Error %d getting a device handle for %s\n", errno, device);
        return(1);
    }

    //Get status via ioctl(), third argument is the drive index in a multi-drive device 
    //(hardwired to 0 as practically noone has one of those)
    result = ioctl(fd, CDROM_DRIVE_STATUS, 0);
    close(fd);

    if (result < 0) {
        print_ioctl_error(result);
        return(1);
    }

    //If a disk is loaded return 0. For everything else (like no disk is loaded
    //the tray is open, errors, etc), return 1
    switch (result)
    {
        case CDS_DISC_OK:
            puts("Media loaded");
            return 0;
        default:
            puts("Media NOT loaded");
            return 1;
    }
}