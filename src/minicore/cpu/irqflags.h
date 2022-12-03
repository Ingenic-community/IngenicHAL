#ifndef _CPU_IRQFLAGS_H_
#define _CPU_IRQFLAGS_H_

#include <asm/mipsregs.h>

static inline void local_irq_disable(void)
{
    unsigned int tmp = read_c0_status();
    tmp &= ~(1 << 0);
    write_c0_status(tmp);
}

static inline void local_irq_enable(void)
{
    unsigned int tmp = read_c0_status();
    write_c0_status(tmp | (1 << 0));
}

static inline unsigned long _local_irq_save(void)
{
    unsigned int status = read_c0_status();

    write_c0_status(status & ~(1 << 0));

    return status & (1 << 0);
}

#define local_irq_save(flags)                         \
    do {                                              \
        flags = _local_irq_save();                    \
    } while (0)

static inline void local_irq_restore(unsigned long flags)
{
    if (flags)
        local_irq_enable();
}

#endif /* _CPU_IRQFLAGS_H_ */
