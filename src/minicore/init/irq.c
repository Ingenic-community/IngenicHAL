
#include <asm/mipsregs.h>
#include <cpu/irq.h>

#include <cpu/irqflags.h>

#define NULL ((void*)0)
#define assert

void intc_init_irq(void);
void gpio_init_irq(void);

void gpio_enable_irq(int irq);
void gpio_disable_irq(int irq);

void gpio_startup_irq(int irq);
void gpio_shutdown_irq(int irq);

void intc_enable_irq(int irq);
void intc_disable_irq(int irq);

#define INTCTL_SOFT_IRQ        (1 << 2)
#define INTCTL_CH_IRQ          (1 << 1)
#define INTCTL_INTC_IRQ        (1 << 0)

typedef void (*irq_handler_t)();

static irq_handler_t irq_funcs[IRQ_NUMS];
static void *irq_datas[IRQ_NUMS];

unsigned char is_enable[(IRQ_NUMS + 7) / 8];

static inline int is_cpu_irq(int irq)
{
    return IRQ_CPU_SOFT <= irq && irq <= IRQ_CPU_INTC;
}

static inline int is_intc_irq(int irq)
{
    return IRQ_INTC_START <= irq && irq < IRQ_INTC_END;
}

static inline int is_gpio_irq(int irq)
{
    return IRQ_GPIO_START <= irq && irq < IRQ_GPIO_END;
}

void request_irq_disabled(int irq, unsigned int irq_flags,
                irq_handler_t handler, const char *name, void *data)
{
    assert(0 <= irq && irq < IRQ_NUMS);
    assert(handler);

    assert(!irq_funcs[irq]);

    irq_funcs[irq] = handler;

    irq_datas[irq] = data;
//
//    if (is_gpio_irq(irq))
//        gpio_startup_irq(irq);
}

void enable_irq(int irq)
{
	assert(0 <= irq && irq < IRQ_NUMS);
	assert(irq_funcs[irq]);

	if (is_cpu_irq(irq))
		return;

//	if (is_intc_irq(irq)) {
//		intc_enable_irq(irq - IRQ_INTC_START);
//		return;
//	}
//
//	if (is_gpio_irq(irq)) {
//		gpio_enable_irq(irq - IRQ_GPIO_START);
//		return;
//	}

//    panic("irq not valid: %d\n", irq);
}


void request_irq(int irq, unsigned int irq_flags,
                irq_handler_t handler, const char *name, void *data)
{
    request_irq_disabled(irq, irq_flags, handler, name, data);

    enable_irq(irq);
}

void release_irq(int irq)
{
    assert(0 <= irq && irq < IRQ_NUMS);
    assert(irq_funcs[irq]);
    assert(!is_enable[irq]);

//    if (is_gpio_irq(irq))
//        gpio_shutdown_irq(irq);

    irq_funcs[irq] = NULL;
}


void disable_irq(int irq)
{
    assert(0 <= irq && irq < IRQ_NUMS);
    assert(irq_funcs[irq]);

//    if (is_cpu_irq(irq))
//        return;
//
//    if (is_intc_irq(irq)) {
//        intc_disable_irq(irq);
//        return;
//    }

    if (is_gpio_irq(irq)) {
//        gpio_disable_irq(irq);
        return;
    }

//    panic("irq not valid: %d\n", irq);
}

void handle_irq(int irq)
{
    assert(irq_funcs[irq]);

    irq_funcs[irq](irq, irq_datas[irq]);
}

#define INTCTL_SOFT_IRQ      (1 << 2)
#define INTCTL_CH_IRQ        (1 << 1)
#define INTCTL_INTC_IRQ      (1 << 0)

void exception_entry_c(void)
{
    // unsigned int c0_status = read_c0_status();
    unsigned int c0_intctl = read_c0_intctl();

    // printf("c0 status: %x\n", read_c0_status());
    // printf("c0 intctl: %x\n", read_c0_intctl());

    if (c0_intctl & INTCTL_SOFT_IRQ) {
        handle_irq(IRQ_CPU_SOFT);
        return;
    }

    if (c0_intctl & INTCTL_CH_IRQ) {
        handle_irq(IRQ_CPU_DMA_CHANNEL);
        return;
    }

    if (c0_intctl & INTCTL_INTC_IRQ) {
        handle_irq(IRQ_CPU_INTC);
        return;
    }

    // panic("irq: may be some other exception\n");

    return;
}

unsigned int value = (1 << 7) | (1 << 8);

#include <cpu/ffs.h>

void irq_init(void)
{
//    intc_init_irq();
//    gpio_init_irq();

    local_irq_enable();

    // printf("ffs: %x %x\n", value, __ffs(value));
}
