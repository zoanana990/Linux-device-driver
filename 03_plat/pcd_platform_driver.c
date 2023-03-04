#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

/* MACRO DEFINITION */
#define MEM_SIZE_MAX_PCDEV1	 512
#define MEM_SIZE_MAX_PCDEV2	 1024
#define MEM_SIZE_MAX_PCDEV3	 512
#define MEM_SIZE_MAX_PCDEV4	 1024

#define NO_OF_DEVICES		 4

#define RDONLY			 0x01
#define WRONLY		         0x10
#define RDWR			 0x11

/* print message with module */
#undef pr_fmt
#define pr_fmt(fmt) "[%s] : " fmt, __func__


/* Add module description */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zoanana990");
MODULE_DESCRIPTION("pseudo memory character device driver");

/* pseudo device's memory */
char device_buffer_pcdev1[MEM_SIZE_MAX_PCDEV1];
char device_buffer_pcdev2[MEM_SIZE_MAX_PCDEV2];
char device_buffer_pcdev3[MEM_SIZE_MAX_PCDEV3];
char device_buffer_pcdev4[MEM_SIZE_MAX_PCDEV4];

/* Device private data structure */
struct pcdev_private_data
{
    char 		*buffer;
    unsigned int	size;
    const char 		*serial_number;
    int 		perm;
    struct cdev 	cdev;
};

				     
/* Driver private data structure */
struct pcdrv_private_data
{
    int 			total_devices;
    dev_t 			device_number;
    struct class 		*class_pcd;
    struct device 		*device_pcd;
    struct pcdev_private_data 	pcdev_data[NO_OF_DEVICES];
};

/* global variable */
struct pcdrv_private_data pcdrv_data = 
{
    .total_devices = NO_OF_DEVICES,
    .pcdev_data = {
	[0] = {
	    .buffer = device_buffer_pcdev1,
	    .size = MEM_SIZE_MAX_PCDEV1,
	    .serial_number = "ap;ijdbf",
	    .perm = RDWR
	},
	[1] = {
	    .buffer = device_buffer_pcdev1,
	    .size = MEM_SIZE_MAX_PCDEV1,
	    .serial_number = "ap;ijdbf",
	    .perm = RDWR
	},
	[2] = {
	    .buffer = device_buffer_pcdev1,
	    .size = MEM_SIZE_MAX_PCDEV1,
	    .serial_number = "ap;ijdbf",
	    .perm = RDWR
	},
	[3] = {
	    .buffer = device_buffer_pcdev1,
	    .size = MEM_SIZE_MAX_PCDEV1,
	    .serial_number = "ap;ijdbf",
	    .perm = RDWR
	},
    }
};

loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
    return 0;
}

ssize_t pcd_read(struct file * filp, char __user *buff, size_t count, loff_t *f_pos)
{
    return 0;
}

ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    return 0;
}

int check_permission(int dev_perm, int acc_mode)
{
    pr_info("the device permission: 0x%x, the access mode is 0x%xd\n", dev_perm, acc_mode);    
	
    /* Check the access mode for the device */
    if(dev_perm == RDWR)
	return 0;

    if((dev_perm == RDONLY) && ((acc_mode & FMODE_READ) && !(acc_mode & FMODE_WRITE)))
	return 0;
    
    if((dev_perm == WRONLY) && ((acc_mode & FMODE_WRITE) && !(acc_mode & FMODE_READ)))
	return 0;

    return -EPERM;
}


/* This driver is pseudo device, thus we don't need to implement it */
int pcd_open(struct inode *inode, struct file *filp)
{
    return 0;
}

int pcd_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/* file oeprations of the driver */
struct file_operations pcd_ops = 
{
    .open = pcd_open,
    .write = pcd_write,
    .read = pcd_read,
    .llseek = pcd_lseek,
    .release = pcd_release,
};


static int __init pcd_module_init(void)
{
    return 0;
}

static void __exit pcd_module_cleanup(void)
{

}

module_init(pcd_module_init);
module_exit(pcd_module_cleanup);
