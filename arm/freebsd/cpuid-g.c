/* Print CPUID the right way
 *
 * Copyright (c) 2022, Evgenii Iuliugin.
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

#include <sys/types.h>
#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/conf.h>

static d_open_t cpuid_g_open;
static d_read_t cpuid_g_read;

static struct cdevsw cpuid_g_cdevsw = {
    .d_version = D_VERSION,
    .d_read = cpuid_g_read,
    .d_open = cpuid_g_open,
    .d_name = "cpuid-g",
};

static struct cdev *cpuid_g_dev;

static int
cpuid_g_loader(struct module *m __unused, int what, void *arg __unused)
{
    int error = 0;

    switch (what) {
    case MOD_LOAD:
        error = make_dev_p(MAKEDEV_CHECKNAME | MAKEDEV_WAITOK,
                           &cpuid_g_dev, &cpuid_g_cdevsw, 0,
                           UID_ROOT, GID_WHEEL, 0600, "cpuid-g");
        break;
    case MOD_UNLOAD:
        destroy_dev(cpuid_g_dev);
        break;
    default:
        error = EOPNOTSUPP;
    }

    return error;
}

static int
cpuid_g_read(struct cdev *dev __unused, struct uio *uio, int ioflag __unused)
{
    return 0;
}

static int
cpuid_g_open(struct cdev *dev __unused, int oflags __unused, int devtype __unused,
             struct thread *td __unused)
{
    return 0;
}

DEV_MODULE(cpuid_g, cpuid_g_loader, NULL);
