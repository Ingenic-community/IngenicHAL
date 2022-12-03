#include <stdio.h>
#include <cpu/host_cpu.h>
#include <soc/cpm.h>
#include <cpu/io.h>

#include <driver/irq.h>
#include <ring_mem.h>

#define PDMA_IOBASE 0x13420000


#define DMCS (0x1030) /*MCU Control & Status*/
#define DMNMB (0x1034) /*MCU Normal MailBox*/
#define DMSMB (0x1038) /*MCU Security MailBox*/
#define DMINT (0x103c) /*MCU Interrupt*/

#define DMINT_N_IP (1 << 16)

static char in_buffer[256];
static char out_buffer[256];

DEFINE_RING_MEM(data_from_host, in_buffer);
DEFINE_RING_MEM(data_to_host, out_buffer);


int mcu_test_host_busy(void)
{
    return readl(PDMA_IOBASE + DMINT) & DMINT_N_IP;
}

void mcu_notify_host(int len)
{
    writel(len, PDMA_IOBASE + DMNMB);
}

int host_cpu_read(void *buf, unsigned int size)
{
    int len = 0;

    while (size) {
        int ret = ring_mem_read(&data_from_host, buf, size);
        if (!ret)
            break;
        len += ret;
        size -= ret;
        buf += ret;
    }

    return len;
}

int host_cpu_write(void *buf, unsigned int size)
{
    int len = 0;

    while (size) {
        int ret = ring_mem_write(&data_to_host, buf, size);
        if (!ret)
            break;
        len += ret;
        size -= ret;
        buf += ret;
    }

    return len;
}

int host_cpu_has_readable_data(void)
{
    return ring_mem_readable_size(&data_from_host);
}
