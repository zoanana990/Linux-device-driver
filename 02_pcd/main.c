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
	    .perm = 0x1 /* RDONLY */
	},
	[1] = {
	    .buffer = device_buffer_pcdev1,
	    .size = MEM_SIZE_MAX_PCDEV1,
	    .serial_number = "ap;ijdbf",
	    .perm = 0x10 /* WRONLY */
	},
	[2] = {
	    .buffer = device_buffer_pcdev1,
	    .size = MEM_SIZE_MAX_PCDEV1,
	    .serial_number = "ap;ijdbf",
	    .perm = 0x11 /* RDWR */
	},
	[3] = {
	    .buffer = device_buffer_pcdev1,
	    .size = MEM_SIZE_MAX_PCDEV1,
	    .serial_number = "ap;ijdbf",
	    .perm = 0x11 /* RDWR */
	},
    }
};

loff_t pcd_lseek(struct file *filp, loff_t offset, int whence)
{
    return 0;
#if 0
    loff_t temp;

    pr_info("lseek requested\n");
    pr_info("Current value of the file position = %lld\n", filp->f_pos);
    
    switch(whence)
    {
        case SEEK_SET:
            if((offset > DEV_MEM_SIZE) || (offset < 0))
                return -EINVAL;
            filp->f_pos = offset;
            break;
        case SEEK_CUR:
            temp = filp->f_pos + offset;
            if((temp > DEV_MEM_SIZE) || (temp < 0))
                return -EINVAL;
            filp->f_pos = temp;
            break;
        case SEEK_END:
            temp = DEV_MEM_SIZE + offset;
            if((temp > DEV_MEM_SIZE) || (temp < 0))
                return -EINVAL;
            filp->f_pos = temp;
            break;
        default:
            return -EINVAL;
    }

    pr_info("New Value of the file position = %lld\n", filp->f_pos);
    return filp->f_pos;
#endif
}

ssize_t pcd_read(struct file * filp, char __user *buff, size_t count, loff_t *f_pos)
{
    
    struct pcdev_private_data *pcdev_data = (struct pcdev_private_data *) filp->private_data;
    int max_size = pcdev_data->size;

    pr_info("Read requested  for %zu bytes\n", count);
    pr_info("Current file position = %lld\n", *f_pos);

    /* adjust count, if it is bigger than the memory size */
    if((*f_pos + count) > max_size)
        count = max_size - *f_pos;

    /* copy the data to the user space */
    if(copy_to_user(buff, pcdev_data->buffer+(*f_pos), count))
        return -EFAULT;

    /* update the current file position */
    *f_pos += count;

    pr_info("Number of bytes successfully read = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    return count;
}

ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    return 0;
#if 0
    /* current file position */
    pr_info("Current file position = %lld\n", *f_pos);

    /* adjust count, if it is bigger than the memory size */
    if((*f_pos + count) > DEV_MEM_SIZE)
        count = DEV_MEM_SIZE - *f_pos;
    
    if(!count)
    {
	pr_err("No space on the device\n");
	return -ENOMEM;
    }
      
    /* copy the data to the user space */
    if(copy_from_user(&device_buffer[*f_pos], buff, count))
        return -EFAULT;

    /* update the current file position */
    *f_pos += count;

    pr_info("Number of bytes successfully written = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    return count;

#endif
}

int check_permission(void)
{
    /* TODO */
    return 0;
}


/* This driver is pseudo device, thus we don't need to implement it */
int pcd_open(struct inode *inode, struct file *filp)
{
    int ret, minor_n;

    struct pcdev_private_data *pcdev_data;

    /* 1. find out on which device file open was attempted by the user space 
     *    We need to use `inode` structure here
     *    `inode` member: dev_t rdev   -> device number
     *                    cdev *i_cdev -> character device
     */
    minor_n = MINOR(inode->i_rdev);
    pr_info("minor access = %d\n", minor_n);

    /* 2. get device's private data structure 
     *    container_of(ptr_member, structure_ type, name_of_member)
     *    get the address of the member
     */
    pcdev_data = container_of(inode->i_cdev, struct pcdev_private_data, cdev);

    /* Supply device private data to other methods of the driver */
    filp->private_data = pcdev_data;

    /* 3. check permission */
    ret = check_permission();
    (!ret) ? pr_info("open was successfully\n") : pr_info("open was unsuccessfully");

    pr_info("Open was successful\n");
    return 0;
}

int pcd_release(struct inode *inode, struct file *filp)
{
    pr_info("Close was successful\n");
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
    /* return value for the linux function */
    int ret, i = 0;

    /* 1. Dynamically allocate a device number */
    ret = alloc_chrdev_region(&pcdrv_data.device_number, 0, NO_OF_DEVICES, "pcd");
    if(ret < 0)
    {
	pr_err("Allocate character device failed\n");
	goto fail;
    }

    pr_info("Device number <major>:<minor> = %d:%d\n",
            MAJOR(pcdrv_data.device_number), MINOR(pcdrv_data.device_number));

    /* 2. create device class under /sys/class */
    pcdrv_data.class_pcd = class_create(THIS_MODULE, "pcd_class");
    if(IS_ERR(pcdrv_data.class_pcd))
    {
	pr_err("Class create failed\n");
	ret = PTR_ERR(pcdrv_data.class_pcd);
	goto unreg_chrdev;
    }

    for(i = 0; i < NO_OF_DEVICES; i++)
    {
	/* 3. Make character device registration through the VFS 
	 *    cdev_init(struct cdev, file_operations);
	 */
	cdev_init(&pcdrv_data.pcdev_data[i].cdev, &pcd_ops);

	/* Initialize the character device owner */
	pcdrv_data.pcdev_data[i].cdev.owner = THIS_MODULE;
    
	/* 4. Add a character device to the kernel VFS */
	ret = cdev_add(&pcdrv_data.pcdev_data[i].cdev, pcdrv_data.device_number+i, 1);
	if(ret < 0)
	{
	    pr_err("Add character device failed\n");
	    goto class_del;
	}
    
	/* 5. populate the sysfs with the device information */
	pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd, NULL, pcdrv_data.device_number+i,
					      NULL, "pcdev-%d", i+1);
	if(IS_ERR(pcdrv_data.device_pcd))
	{
	    pr_err("Device create failed\n");
	    ret = PTR_ERR(pcdrv_data.device_pcd);
	    goto cdev_del;
	}
    }
   
    pr_info("Module init successfully\n");

    return 0;
/*---------------------------------------------------------------- ERROR HANDLE */
class_del:
cdev_del:
    for(; i > 0; i--)
    {
	device_destroy(pcdrv_data.class_pcd, pcdrv_data.device_number + i);
	cdev_del(&pcdrv_data.pcdev_data[i].cdev);
    }
    class_destroy(pcdrv_data.class_pcd);

unreg_chrdev:
    unregister_chrdev_region(pcdrv_data.device_number, NO_OF_DEVICES);
fail:
    return ret;
}

static void __exit pcd_module_cleanup(void)
{
    int i;
    for(i = 0; i < NO_OF_DEVICES; i--)
    {
	device_destroy(pcdrv_data.class_pcd, pcdrv_data.device_number + i);
	cdev_del(&pcdrv_data.pcdev_data[i].cdev);
    }
    class_destroy(pcdrv_data.class_pcd);
    unregister_chrdev_region(pcdrv_data.device_number, NO_OF_DEVICES);

    return;
}

module_init(pcd_module_init);
module_exit(pcd_module_cleanup);
