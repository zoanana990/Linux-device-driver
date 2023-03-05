/* This file is I2C module for the platform device
 */
#include <linux/module.h>
#include <linux/platform_device.h>

#undef pr_fmt
#define pr_fmt(fmt) "[VI2C MODEUL] : " fmt 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zoanana990");
MODULE_DESCRIPTION("a virtual i2c devices");

void vi2c_release(struct device *dev){
    pr_info("I2C platform device release\n");
}

struct platform_device vi2c = {
    .name = "i2c",
    .id = 0,
    .dev = {
	.release = vi2c_release 
    }
};

static int __init vi2c_init(void)
{
    pr_info("Load the I2C platform device\n");
    return 0;
}

static void __exit vi2c_exit(void)
{
    pr_info("remove the I2C platform device\n");
    return;
}

module_init(vi2c_init);
module_exit(vi2c_exit);
