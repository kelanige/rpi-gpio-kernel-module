#include <linux/fs.h>
#include <linux/init.h>
#include <linux/types.h>

enum PINS
{
    PIN_16 = 16,
};

int __init ecearpi_module_init(void);
void __exit ecearpi_module_exit(void);
int ecearpi_module_open(struct inode *inode, struct file *file);
int ecearpi_module_release(struct inode *inode, struct file *file);
ssize_t ecearpi_module_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
ssize_t ecearpi_module_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);
