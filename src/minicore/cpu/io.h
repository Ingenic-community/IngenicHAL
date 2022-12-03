#ifndef _CPU_IO_H_
#define _CPU_IO_H_

#include "asm/addrspace.h"

#define io_addr(addr) ((volatile unsigned long *)CPHYSADDR(addr))

#define virt_to_phys(addr) CPHYSADDR(addr)

static inline unsigned long inl(unsigned long addr)
{
    return *io_addr(addr);
}

static inline void outl(unsigned long val, unsigned long addr)
{
    *io_addr(addr) = val;
}

static inline unsigned long readl(unsigned long addr)
{
    return inl(addr);
}

static inline void writel(unsigned long val, unsigned long addr)
{
    outl(val, addr);
}

#endif /* _CPU_IO_H_ */
