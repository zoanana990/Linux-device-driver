/* This project is aims to match the platform driver by the platform bus 
 * 1. create and register two platform devices
 * 2. create platform data
 */

#include <linux/module.h>
#include <linux/platform_device.h>

#include "platform.h"

#undef pr_fmt
#define pr_fmt(fmt) "[%s] : " fmt, __func__

struct pcdev_platform_data pcdev_pdata[2] = {
    [0] = {.size = 512, .perm = RDWR, .serial_number = "PCDEVA"},
    [1] = {.size = 1024, .perm = RDWR, .serial_number = "PCDEVX"},
};

struct platform_device platform_pcdev_1 = {
    .name = "pseudo-char-device",
    .id = 0,
    .dev = {
	.platform_data = &pcdev_pdata[0]
    }
};

struct platform_device platform_pcdev_2 = {
    .name = "pseudo-char-device",
    .id = 1,
    .dev = {
	.platform_data = &pcdev_pdata[1]
    }
};

void pcdev_release(struct device *dev)
{
    pr_info("Device released \n");
}

static int __init pcdev_platform_init(void)
{
    /* register platform device */
    platform_device_register(&platform_pcdev_1);
    platform_device_register(&platform_pcdev_2);
    
    pr_info("Device setup complete\n");

    return 0;
}

static void __exit pcdev_platform_exit(void)
{
    platform_device_unregister(&platform_pcdev_1);
    platform_device_unregister(&platform_pcdev_2);

    pr_info("Device removed complete\n");
}

module_init(pcdev_platform_init);
module_exit(pcdev_platform_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("platform module");
