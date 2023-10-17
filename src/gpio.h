enum PINS
{
    PIN_1 = 1,
};

extern int __init ecearpi_module_init(void);
extern int __exit ecearpi_module_exit(void);
extern int ecearpi_module_open(struct inode *inode, struct file *file);
extern int ecearpi_module_release(struct inode *inode, struct file *file);
extern ssize_t ecearpi_module_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
extern ssize_t ecearpi_module_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);
