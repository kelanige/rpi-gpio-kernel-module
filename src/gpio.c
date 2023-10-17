#include "gpio.h"

#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/string.h>

MODULE_DESCRIPTION("A device driver to interface with GPIO on the Raspberry Pi 4");
MODULE_AUTHOR("Kelan Ige");
MODULE_LICENSE("GPL v2");

module_init(ecearpi_module_init);
module_exit(ecearpi_module_exit);

static struct file_operations ecearpi_fops = {
    .owner = THIS_MODULE,
    .open = ecearpi_module_open,
    .release = ecearpi_module_release,
    .read = ecearpi_module_read,
};

dev_t ecearpi_dev = 0;
static struct cdev ecearpi_cdev;
static struct class *ecearpi_class;
static struct device *ecearpi_device;

static int __init ecearpi_module_init(void) {
    printk(KERN_INFO "ecearpi_module_init\n");
    int result;

    result = alloc_chrdev_region(&dev, 0, 1, "ecearpi_gpio");
    if (result < 0) {
        printk(KERN_ERROR "alloc_chrdev_region failed with %d\n", result);
        goto z_unalloc;
    }

    cdev_init(&ecearpi_cdev, &ecea_fops);
    result = cdev_add(&ecearpi_cdev, ecearpi_dev, 1);
    if (result < 0) {
        printk(KERN_ERROR "cdev_add failed with %d\n", result);
        goto z_unadd;
    }

    ecearpi_class = class_create(THIS_MODULE, "ecearpi_class");
    if (ecearpi_class == ERR_PTR) {
        printk(KERN_ERROR "class_create failed\n");
        goto z_class_uncreate;
    }

    ecearpi_device = device_create(ecearpi_class, NULL, ecearpi_dev, NULL, "ecearpi_device");
    if (ecearpi_device == ERR_PTR) {
        printk(KERN_ERROR "device_create failed\n");
        goto z_device_uncreate;
    }

    if (gpio_is_valid(PIN_1) == 0) {
        printk(KERN_WARNING "PIN %d is not valid\n", PIN_1);
        goto z_device_uncreate;
    }

    result = gpio_request(PIN_1, "PIN_1");
    if (result < 0) {
        printk(KERN_ERROR "gpio_request for pin %d failed\n", PIN_1);
        goto z_gpio_release;
    }
    gpio_direction_output(PIN_1, 0);
    gpio_export(PIN_1, 0);

    return 0;

    z_gpio_release:
        gpio_free(PIN_1);
    z_device_uncreate:
        device_destroy(ecearpi_class, ecearpi_device);
    z_class_uncreate:
        class_destroy(ecearpi_class);
    z_unadd:
        cdev_del(&ecearpi_cdev);
    z_unalloc:
        unregister_chrdev_region(dev, 1);

    return -1;
}

static void __exit ecearpi_module_exit(void) {
    printk(KERN_INFO "ecearpi_module_exit\n");
    gpio_unexport(PIN_1);
    gpio_free(PIN_1);
    device_destroy(ecearpi_class, ecearpi_device);
    class_destroy(ecearpi_class);
    cdev_dev(&ecearpi_cdev);
    unregister_chrdev_region(dev, 1);
}

static int ecearpi_module_open(struct inode *inode, struct file *file) {
    printk(KERN_INFO "ecearpi_module_open\n");
    return 0;
}

static int ecearpi_module_release(struct inode *inode, struct file *file) {
    printk(KERN_INFO "ecearpi_module_release\n");
    return 0;
}

static ssize_t ecearpi_module_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
    uint8_t pin_state = gpio_get_value(PIN_1);

    if (copy_to_user(buf, &pin_state, sizeof(uint8_t)) > 0) {
        printk(KERN_ERROR "ecearpi_module_read: copy_to_user failed\n");
    }

    return sizeof(pin_state);
}

static ssize_t ecearpi_module_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
    const char on[3] = {'o', 'n', '\0'};
    const char off[4] = {'o', 'f', 'f', '\0'};
    char usr_buf[10] = {0};

    if (copy_from_user(rec_buf, buf, len) > 0) {
        printk(KERN_ERROR "ecearpi_module_write: copy_from_user failed\n");
    }

    if (strcmp(usr_buf, on) == 0) {
        gpio_set_value(PIN_1, 1);
    } else if (strcmp(usr_buf, off) == 0) {
        gpio_set_value(PIN_1, 0);
    } else {
        printk(KERN_WARN "ecearpi_module_write: unknown command received %s\n", usr_buf);
    }

    return len;
}
