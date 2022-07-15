#include <sys/param.h>  /* defines used in kernel.h */
#include <sys/module.h>
#include <sys/kernel.h> /* types used in module initialization */
#include <sys/systm.h>  /* uprintf */
#include <sys/conf.h>   /* cdevsw struct */
#include <sys/uio.h>    /* uio struct */

#include "cpuid-g.h"
#include "asm-inline.h"

static d_open_t      cpuid_g_open;
static d_close_t     cpuid_g_close;
static d_read_t      cpuid_g_read;

static struct cdevsw cpuid_g_cdevsw = {
    .d_version = D_VERSION,
    .d_open = cpuid_g_open,
    .d_close = cpuid_g_close,
    .d_read = cpuid_g_read,
    .d_name = "cpuid_g",
};

static int
cpuid_g_open(struct cdev *dev __unused, int oflags __unused, int devtype __unused,
           struct thread *td __unused)
{
    return 0;
}

static int
cpuid_g_close(struct cdev *dev __unused, int fflag __unused, int devtype __unused,
            struct thread *td __unused)
{
    return 0;
}

#include "cpuid_g_read.h"

static int
cpuid_g_read(struct cdev *dev __unused, struct uio *uio, int ioflag __unused)
{
#ifdef CONFIG_ARM64
    void *cpuid = (void *)&arm64_cpuid;
    unsigned long size = sizeof arm64_cpuid;
    read_arm64_cpuid();
#else
    void *cpuid = (void *)&arm32_cpuid;
    unsigned long size = sizeof arm32_cpuid;
    read_arm32_cpuid();
#endif

    size_t amt;
    int retval = 0;

    amt = MIN(uio->uio_resid, uio->uio_offset >= size + 1 ? 0 :
              size + 1 - uio->uio_offset);

    if ((retval = uiomove(cpuid, amt, uio)) != 0) {
        uprintf("uiomove failed!\n");
    }

    return retval;
}

static struct cdev *cpuid_g_dev = NULL;

static int
cpuid_g_event_handler(struct module *module, int event_type, void *arg)
{
    int retval = 0;

    switch (event_type) {
        case MOD_LOAD:
            cpuid_g_dev = make_dev(&cpuid_g_cdevsw, 0, UID_ROOT, GID_WHEEL, 0644, "cpuid-g");
            if (cpuid_g_dev == NULL) {
                uprintf( "cpuid-g device create failed\n" );
                break;
            }
            break;

        case MOD_UNLOAD:
            if( cpuid_g_dev != NULL ) {
                destroy_dev(cpuid_g_dev);
            }
            break;

        default:
            uprintf( "cpuid_g: unknown operation\n" );
            retval = EOPNOTSUPP;
            break;
    }
    return retval;
}

DEV_MODULE(cpuid_g, cpuid_g_event_handler, NULL);
