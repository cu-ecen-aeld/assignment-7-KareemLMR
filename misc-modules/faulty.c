/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
 */                                                    
#include <linux/module.h>
#include <linux/init.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/fs.h> 

MODULE_LICENSE("Dual BSD/GPL");


struct faulty_dev
{
    struct cdev cdev;
};

struct faulty_dev faulty_device;

int faulty_major =   0;
int faulty_minor =   0;

int faulty_open(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t faulty_write(struct file *filp, const char __user *buf, size_t count,
                loff_t *f_pos)
{
	/* make a simple fault by dereferencing a NULL pointer */
	*(int *)0 = 0;
	return 0;
}

int faulty_release(struct inode *inode, struct file *filp)
{
    return 0;
}

ssize_t faulty_read(struct file *filp, char __user *buf, size_t count,
                loff_t *f_pos)
{
	int i;
	int ret;
	char stack_buf[4];

	/* Let's try a buffer overflow  */
	for (i = 0; i < 20; i++)
		*(stack_buf + i) = 0xff;
	if (count > 4)
		count = 4; /* copy 4 bytes to the user */
	ret = copy_to_user(buf, stack_buf, count);
	if (!ret)
		return count;
	return ret;
}

struct file_operations faulty_fops = {
    .owner =    THIS_MODULE,
    .read =     faulty_read,
    .write =    faulty_write,
    .open =     faulty_open,
    .release =  faulty_release,
};

static int faulty_setup_cdev(struct faulty_dev *dev)
{
    int err, devno = MKDEV(faulty_major, faulty_minor);

    cdev_init(&dev->cdev, &faulty_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &faulty_fops;
    err = cdev_add (&dev->cdev, devno, 1);
    if (err) {
        printk(KERN_ERR "Error %d adding faulty cdev", err);
    }
    return err;
}

static int faulty_init(void)
{
	printk(KERN_ALERT "Hello, Kareem Ibrahim aka faulty\n");
	dev_t dev = 0;
    int result;
    result = alloc_chrdev_region(&dev, faulty_minor, 1,
            "faulty");
    faulty_major = MAJOR(dev);
    if (result < 0) {
        printk(KERN_WARNING "Can't get major %d\n", faulty_major);
        return result;
    }
    memset(&faulty_device,0,sizeof(struct faulty_dev));

    result = faulty_setup_cdev(&faulty_device);

    if( result ) {
        unregister_chrdev_region(dev, 1);
    }
    return result;
}

static void faulty_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");

	dev_t devno = MKDEV(faulty_major, faulty_minor);

    cdev_del(&faulty_device.cdev);

    unregister_chrdev_region(devno, 1);
}

module_init(faulty_init);
module_exit(faulty_exit);
