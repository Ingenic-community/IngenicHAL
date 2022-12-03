#ifndef _CPU_HOST_CPU_H_
#define _CPU_HOST_CPU_H_

int host_cpu_read(void *buf, unsigned int size);

int host_cpu_write(void *buf, unsigned int size);

int host_cpu_has_readable_data(void);

void host_cpu_set_irq_callback(void (*cb)(void));

void host_cpu_irq_init(void);

void mcu_notify_host(int len);

int mcu_test_host_busy(void);

#endif /* _CPU_HOST_CPU_H_ */
