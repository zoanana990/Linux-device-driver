#include <linux/module.h>
#include <linux/fs.h>

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

/* file oeprations of the driver */
struct file_operations pcd_ops;


static int __init pcd_module_init(void)
{
    /* 1. Dynamically allocate a device number */
    alloc_chrdev_region(&device_number, 0, 1, "pcd");

    /* 2. Make character device registration through the VFS */
    cdev_init(&pcd_cdev, &pcd_ops);

    /* 3. Add a character device to the kernel VFS */
    cdev_add(&pcd_cdev, device_number, 1);
    pcd_cdev.owner = THIS_MODULE;
    
    return 0;
}

static void __exit pcd_module_cleanup(void)
{
    return;
}

module_init(pcd_module_init);
module_exit(pcd_module_cleanup);
