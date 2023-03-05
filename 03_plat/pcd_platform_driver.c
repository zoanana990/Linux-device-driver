#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include "platform.h"

/* MACRO DEFINITION */
#define MAX_DEVICES	2

/* print message with module */
#undef pr_fmt
#define pr_fmt(fmt) "[%s] : " fmt, __func__


/* Add module description */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zoanana990");
MODULE_DESCRIPTION("pseudo memory character device driver");

/* Device private data structure */
struct pcdev_private_data
{
    struct pcdev_platform_data 	pdata;
    char 			*buffer;
    dev_t 			dev_num;
    struct cdev 		cdev;
};

				     
/* Driver private data structure */
struct pcdrv_private_data
{
    int 			total_devices;
    dev_t 			device_num_base;
    struct class 		*class_pcd;
    struct device 		*device_pcd;
};

/* global variable */
struct pcdrv_private_data pcdrv_data;

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
    .owner = THIS_MODULE,
    .open = pcd_open,
    .write = pcd_write,
    .read = pcd_read,
    .llseek = pcd_lseek,
    .release = pcd_release,
};

/* gets called when the device is found from the system */
int pcd_platform_driver_probe(struct platform_device *pdev)
{
    int ret;

    /* the structure */
    struct pcdev_private_data *dev_data;

    /* in the device, platform_data */
    struct pcdev_platform_data *pdata;

    /* 1. Get the platform data */
    pdata = (struct pcdev_platform_data *)dev_get_platdata(&pdev->dev);
    if(!pdata)
    {
	pr_info("No platform data available\n");
	ret = -ENOMEM;
	goto out;
    }   

    /* 2. Dynamically allocate memory for the device private data */   
    dev_data = kzalloc(sizeof(*dev_data), GFP_KERNEL);
    if(!dev_data)
    {
	pr_info("Cannot allocate memory\n");
	ret = -ENOMEM;
	goto out;
    }
    
    dev_set_drvdata(&pdev->dev,dev_data);    

    /* load the device information to the driver */
    dev_data->pdata.size = pdata->size;
    dev_data->pdata.perm = pdata->perm;
    dev_data->pdata.serial_number = pdata->serial_number;

    pr_info("Device serial number = %s\n", dev_data->pdata.serial_number);
    pr_info("Device size = %d\n", dev_data->pdata.size);
    pr_info("Device permission = %d\n", dev_data->pdata.perm);

    /* 3. Dynamically allocate memory for the device buffer using size information from the platform data */
    dev_data->buffer = kzalloc(dev_data->pdata.size, GFP_KERNEL);
    if(!dev_data->buffer)
    {
	pr_info("Cannot allocate memory\n");
	ret = -ENOMEM;
	goto dev_data_free;
    }

    /* 4. Get the device number */
    dev_data->dev_num = pcdrv_data.device_num_base + pdev->id;

    /* 5. Do cdev init and cdev add */
    cdev_init(&dev_data->cdev, &pcd_ops);
    dev_data->cdev.owner = THIS_MODULE;
    ret = cdev_add(&dev_data->cdev, dev_data->dev_num, 1);
    if(ret)
    {
	pr_err("Cdev add fail\n");
	goto buffer_free;
    }

    /* 6. Create device file for the detected platform device */
    pcdrv_data.device_pcd = device_create(pcdrv_data.class_pcd, NULL, dev_data->dev_num, NULL, "pcdev-%d", pdev->id);
    if(IS_ERR(pcdrv_data.device_pcd))
    {
	pr_err("Device create failed\n");
	ret = PTR_ERR(pcdrv_data.device_pcd);
	goto cdev_del;
    }

    pr_info("A device is detected\n");
    return 0;
    
/* ----------------------------------------------------------------- Error handling */
cdev_del:
    cdev_del(&dev_data->cdev);

buffer_free:
    kfree(dev_data->buffer);

dev_data_free:
    kfree(dev_data->buffer);

out:
    return ret;
}

/* when the device is removed from the system */
int pcd_platform_driver_remove(struct platform_device *pdev)
{
    struct pcdev_private_data *dev_data = dev_get_drvdata(&pdev->dev);

    /* 1. Remove a device that was created with device_create() */
    device_destroy(pcdrv_data.class_pcd, dev_data->dev_num);

    /* 2. Remove a cdev entry from the system */
    cdev_del(&dev_data->cdev);

    /* 3. From the memory heil by the device */
    kfree(dev_data->buffer);
    kfree(dev_data);

    pr_info("A device is removed\n");
    return 0;
}

struct platform_driver pcd_platform_driver = 
{
    .probe = pcd_platform_driver_probe,
    .remove = pcd_platform_driver_remove,
    .driver = {
	    .name = "pseudo-char-device"
    }
};

static int __init pcd_module_init(void)
{
    int ret;
    
    /* 1. Dynamically allocate a device number for MAX_DEVICES */
    ret = alloc_chrdev_region(&pcdrv_data.device_num_base, 0, MAX_DEVICES, "platform_driver");
    if(ret < 0)
    {
	pr_err("Allocate character device number failed\n");
	goto init_failed;
    }
    /* 2. Create device class under /sys/class */
    pcdrv_data.class_pcd = class_create(THIS_MODULE, "platform_class");
    if(IS_ERR(pcdrv_data.class_pcd))
    {
	pr_err("Class create failed\n");
	ret = PTR_ERR(pcdrv_data.class_pcd);
	goto unreg_chrdev;
    }
    /* 3. Register a platform driver */

    platform_driver_register(&pcd_platform_driver);

    pr_info("platform driver register successfully\n");
    return 0;

unreg_chrdev:
    unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);

init_failed:
    return ret;

}

static void __exit pcd_module_cleanup(void)
{
    /* unregister the platform driver */
    platform_driver_unregister(&pcd_platform_driver);

    /* destroy the /sys/class */
    class_destroy(pcdrv_data.class_pcd);

    /* unregister the device number */
    unregister_chrdev_region(pcdrv_data.device_num_base, MAX_DEVICES);
    
    pr_info("platform driver register failed\n");
}

module_init(pcd_module_init);
module_exit(pcd_module_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zoanana990");
MODULE_DESCRIPTION("A pseudo character platform driver which handles n platform pcdevs");
