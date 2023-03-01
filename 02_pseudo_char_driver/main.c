#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>

/* MACRO DEFINITION */
#define DEV_MEM_SIZE 512

/* Add module description */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zoanana990");
MODULE_DESCRIPTION("pseudo memory character device driver");

/* pseudo device's memory */
char device_buffer[DEV_MEM_SIZE];

/* This holds the device number */
dev_t device_number;

/* Cdev variable */
struct cdev pcd_cdev;

loff_t pcd_lseek(struct file *filp, loff_t off, int whence)
{
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
}

ssize_t pcd_read(struct file * filp, char __user *buff, size_t count, loff_t *f_pos)
{
    /* current file position */
    pr_info("Current file position = %lld\n", *f_pos);

    /* adjust count, if it is bigger than the memory size */
    if((*f_pos + count) > DEV_MEM_SIZE)
        count = DEV_MEM_SIZE - *f_ops;

    /* copy the data to the user space */
    if(copy_to_user(buff, &device_buffer[*f_pos], count))
        return -EFAULT;

    /* update the current file position */
    *f_pos += count;

    pr_info("Number of bytes successfully read = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    return count;
}

ssize_t pcd_write(struct file *filp, const char __user *buff, size_t count, loff_t *f_pos)
{
    /* current file position */
    pr_info("Current file position = %lld\n", *f_pos);

    /* adjust count, if it is bigger than the memory size */
    if((*f_pos + count) > DEV_MEM_SIZE)
        count = DEV_MEM_SIZE - *f_ops;
    
    if(!count)
        return -ENOMEM;

    /* copy the data to the user space */
    if(copy_from_user(&device_buffer[*f_pos], buff, count))
        return -EFAULT;

    /* update the current file position */
    *f_pos += count;

    pr_info("Number of bytes successfully written = %zu\n", count);
    pr_info("Updated file position = %lld\n", *f_pos);

    return count;
}

/* This driver is pseudo device, thus we don't need to implement it */
int pcd_open(struct inode *inode, struct file *filp)
{
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

struct class *class_pcd;
struct device *device_pcd;

static int __init pcd_module_init(void)
{
    /* 1. Dynamically allocate a device number */
    alloc_chrdev_region(&device_number, 0, 1, "pcd");
    pr_info("Device number <major>:<minor> = %d:%d\n",
            MAJOR(device_number), MINOR(device_number));


    /* 2. Make character device registration through the VFS */
    cdev_init(&pcd_cdev, &pcd_ops);

    /* 3. Add a character device to the kernel VFS */
    cdev_add(&pcd_cdev, device_number, 1);
    pcd_cdev.owner = THIS_MODULE;
    
    /* 4. create device class under /sys/class */
    class_cpd = class_create(THIS_MODULE, "pcd_class");

    /* 5. populate the sysfs with the device information */
    device_pcd = device_create(class_pcd, NULL, device_number, NULL, "pcd");
    pr_info("Module init successfully\n");

    return 0;
}

static void __exit pcd_module_cleanup(void)
{
    device_destroy(class_pcd, device_number);
    class_destroy(class_pcd);
    udev_del(&pcd_cdev);
    unregister_chrdev_region(device_number, 1);
    pr_info("Module cleanup successfully\n");
    return;
}

module_init(pcd_module_init);
module_exit(pcd_module_cleanup);
