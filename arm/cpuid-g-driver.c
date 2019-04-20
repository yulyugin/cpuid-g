/* Print CPUID the right way
 *
 * Copyright (c) 2014, Evgeny Yulyugin.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * The names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

MODULE_LICENSE("BSD 2-Clause");
MODULE_AUTHOR("Evgeny Yulyugin <yulyugin@gmail.com>");
MODULE_DESCRIPTION("cpuid-g");

static int cpuid_g_open(struct inode *, struct file *);
static int cpuid_g_release(struct inode *, struct file *);
static ssize_t cpuid_g_read(struct file *, char *, size_t, loff_t *);

static atomic_t is_open = ATOMIC_INIT(0);

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = cpuid_g_read,
    .open = cpuid_g_open,
    .release = cpuid_g_release,
};

static struct miscdevice cpuid_g_dev = {
    MISC_DYNAMIC_MINOR,
    "cpuid-g",
    &fops
};

static int __init
cpuid_g_init(void)
{
    int ret = misc_register(&cpuid_g_dev);
    if (ret)
        printk(KERN_ERR "Unable to register cpuid-g device\n");

    return ret;
}

static void __exit
cpuid_g_exit(void)
{
    misc_deregister(&cpuid_g_dev);
}

module_init(cpuid_g_init);
module_exit(cpuid_g_exit);

static int
cpuid_g_open(struct inode *inode, struct file *file)
{
    if (atomic_add_unless(&is_open, 1, 1) == 0)
      return -EBUSY;
    return 0;
}

static int
cpuid_g_release(struct inode *inode, struct file *file)
{
    atomic_dec(&is_open);
    return 0;
}

static int
put_word(uint32_t cpuid_val, char *buffer)
{
    int count = 4;
    while (count--) {
        if (put_user(cpuid_val >> ((3 - count) * 8), buffer))
          return -EFAULT;
        buffer++;
    }
    return 0;
}

static ssize_t
cpuid_g_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int count = 0;
    uint32_t id = 0;

    if (length < 0)
        return 0;

    count = length / 4;

    switch (count) {
    default:
    case 18:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c2, 5"
            : "=r" (id)
          );
        if (put_word(id, buffer + 17 * 4))
            return -EFAULT;
    case 17:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c2, 4"
            : "=r" (id)
          );
        if (put_word(id, buffer + 16 * 4))
            return -EFAULT;
    case 16:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c2, 3"
            : "=r" (id)
          );
        if (put_word(id, buffer + 15 * 4))
            return -EFAULT;
    case 15:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c2, 2"
            : "=r" (id)
          );
        if (put_word(id, buffer + 14 * 4))
            return -EFAULT;
    case 14:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c2, 1"
            : "=r" (id)
          );
        if (put_word(id, buffer + 13 * 4))
            return -EFAULT;
    case 13:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c2, 0"
            : "=r" (id)
          );
        if (put_word(id, buffer + 12 * 4))
            return -EFAULT;
    case 12:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c1, 7"
            : "=r" (id)
          );
        if (put_word(id, buffer + 11 * 4))
            return -EFAULT;
    case 11:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c1, 6"
            : "=r" (id)
          );
        if (put_word(id, buffer + 10 * 4))
            return -EFAULT;
    case 10:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c1, 5"
            : "=r" (id)
          );
        if (put_word(id, buffer + 9 * 4))
            return -EFAULT;
    case 9:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c1, 4"
            : "=r" (id)
          );
        if (put_word(id, buffer + 8 * 4))
            return -EFAULT;
    case 8:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c1, 3"
            : "=r" (id)
          );
        if (put_word(id, buffer + 7 * 4))
            return -EFAULT;
    case 7:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c1, 2"
            : "=r" (id)
          );
        if (put_word(id, buffer + 6 * 4))
            return -EFAULT;
    case 6:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c1, 1"
            : "=r" (id)
          );
        if (put_word(id, buffer + 5 * 4))
            return -EFAULT;
    case 5:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c1, 0"
            : "=r" (id)
          );
        if (put_word(id, buffer + 4 * 4))
            return -EFAULT;
    case 4:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c0, 3"
            : "=r" (id)
          );
        if (put_word(id, buffer + 3 * 4))
            return -EFAULT;
    case 3:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c0, 2"
            : "=r" (id)
          );
        if (put_word(id, buffer + 2 * 4))
            return -EFAULT;
    case 2:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c0, 1"
            : "=r" (id)
          );
        if (put_word(id, buffer + 4))
            return -EFAULT;
    case 1:
        __asm__ __volatile__ (
            "mrc p15, 0, %0, c0, c0, 0"
            : "=r" (id)
          );
        if (put_word(id, buffer))
            return -EFAULT;
    case 0:
        return count * 4;
    }
}
