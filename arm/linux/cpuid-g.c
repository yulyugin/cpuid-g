/* Print CPUID the right way
 *
 * Copyright (c) 2014, Evgenii Iuliugin.
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

#include "cpuid-g.h"
#include "asm-inline.h"
#include "read-cpuid.h"

MODULE_LICENSE("BSD 2-Clause");
MODULE_AUTHOR("Evgenii Iuliugin <yulyugin@gmail.com>");
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

static ssize_t
cpuid_g_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int not_copied = 0;

#ifdef CPUID_G_ARM64
    void *cpuid = (void *)&arm64_cpuid;
    unsigned long size = sizeof arm64_cpuid;
    read_arm64_cpuid();
#else
    void *cpuid = (void *)&arm32_cpuid;
    unsigned long size = sizeof arm32_cpuid;
    read_arm32_cpuid();
#endif

    if (length < 0)
        return 0;

    if (length > size)
        length = size;

    not_copied = copy_to_user(buffer, cpuid, length);

    return length - not_copied;
}
