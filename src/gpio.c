#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_DESCRIPTION("A device driver to interface with GPIO on the Raspberry Pi 4");
MODULE_AUTHOR("Kelan Ige");
MODULE_LICENSE("GPL v2");

static int __init gpio_module_init(void) {
    printk(KERN_INFO "gpio_module_init\n");
    return 0;
}

static void __exit gpio_module_exit(void) {
    printk(KERN_INFO "gpio_module_exit\n");
}

module_init(gpio_module_init);
module_exit(gpio_module_exit);
