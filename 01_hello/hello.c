#include <linux/module.h>

/* This is module initialization entry point*/
static int __init my_kernel_module_init(void)
{
		/* This is kernel's printf */
		pr_info("Hello World\n");
		return 0;
}

/* This is module clean-up entry point */
static void __exit my_kernel_module_exit(void)
{
		pr_info("Good bye world");
}

/* This is registration of above entry points with kernel */
module_init(my_kernel_module_init);
module_exit(my_kernel_module_exit);

/* This is descriptive information about the module */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("zoanana990");
MODULE_DESCRIPTION("First linux kernel module");
MODULE_INFO(board, "Beaglebone Black REV A5");
