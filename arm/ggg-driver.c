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
#include <asm/uaccess.h>

MODULE_LICENSE("BSD 2-Clause");
MODULE_AUTHOR("Evgeny Yulyugin <yulyugin@gmail.com>");
MODULE_DESCRIPTION("ggg-cpuid");

static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);

static int major = 0;
static atomic_t is_open = ATOMIC_INIT(0);
static uint32_t cpuid_val = 0;

static struct file_operations fops = {
  .read = device_read,
  .open = device_open,
  .release = device_release
};

static uint32_t do_cpuid(void) {
  uint32_t id = 0xaabbccdd;
  __asm__ __volatile__ (
    "mrc p15, 0, %0, c0, c0, 0"
    : "=r" (id)
    );
  return id;
}

static int __init test_init(void) {
  if ((major = register_chrdev(0, "ggg-cpuid", &fops)) < 0) {
    printk("Registering the character device failed with %d\n", major);
    return major;
  }
  printk(KERN_ALERT "ggg-cpuid module is loaded\n");
  printk("Please, create a dev file with 'mknod /dev/ggg-cpuid c %d 0'.\n", major);
  cpuid_val = do_cpuid();
  return 0;
}

static void __exit test_exit(void) {
  unregister_chrdev(major, "ggg-cpuid");
  printk(KERN_ALERT "ggg-cpuid module is unloaded!\n");
}

module_init(test_init);
module_exit(test_exit);

static int device_open(struct inode *inode, struct file *file) {
  if (atomic_add_unless(&is_open, 1, 1) == 0)
    return -EBUSY;
  return 0;
}

static int device_release(struct inode *inode, struct file *file) {
  atomic_dec(&is_open);
  return 0;
}

static ssize_t device_read(struct file *filp,
                           char *buffer,
                           size_t length,
                           loff_t * offset) {
  int count = 4;
  // Read full CPUID value or nothing
  if (length < 4)
    return 0;

  while (count--) {
    if (put_user(cpuid_val >> ((3 - count) * 8), buffer))
      break;
    buffer++;
  }

  return 4 - count;
}
