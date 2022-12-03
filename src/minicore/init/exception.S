#include <asm/mipsregs.h>
#include <asm/regdef.h>
#include <asm/asm.h>

    /*
     * 保存与cpu现场相关的32个通用寄存器+4个cp0寄存器
     */
    .macro save_context_registers
    .set noat
    .set noreorder

 /* sw $0,  (0*4)(k0);  zero */
    sw $1,  (1*4)(k0);
    sw $2,  (2*4)(k0);
    sw $3,  (3*4)(k0);
    sw $4,  (4*4)(k0);
    sw $5,  (5*4)(k0);
    sw $6,  (6*4)(k0);
    sw $7,  (7*4)(k0);
    sw $8,  (8*4)(k0);
    sw $9,  (9*4)(k0);
    sw $10, (10*4)(k0);
    sw $11, (11*4)(k0);
    sw $12, (12*4)(k0);
    sw $13, (13*4)(k0);
    sw $14, (14*4)(k0);
    sw $15, (15*4)(k0);
    sw $16, (16*4)(k0);
    sw $17, (17*4)(k0);
    sw $18, (18*4)(k0);
    sw $19, (19*4)(k0);
    sw $20, (20*4)(k0);
    sw $21, (21*4)(k0);
    sw $22, (22*4)(k0);
    sw $23, (23*4)(k0);
    sw $24, (24*4)(k0);
    sw $25, (25*4)(k0);
 /* sw $26, (26*4)(k0); k0 */
 /* sw $27, (27*4)(k0); k1 */
    sw $28, (28*4)(k0);
    sw $29, (29*4)(k0);
    sw $30, (30*4)(k0);
    sw $31, (31*4)(k0);
    mflo k1
    sw k1,  (32*4)(k0)
    mfhi k1
    sw k1,  (33*4)(k0)
    mfc0 k1, CP0_EPC
    sw k1,  (34*4)(k0)
   #  mfc0 k1, CP0_STATUS
   #  sw k1,  (35*4)(k0)
    .endm

    .macro restore_context_registers
    .set noat
    .set noreorder

 /* lw $0,  (0*4)(k0); zero */
    lw $1,  (1*4)(k0);
    lw $2,  (2*4)(k0);
    lw $3,  (3*4)(k0);
    lw $4,  (4*4)(k0);
    lw $5,  (5*4)(k0);
    lw $6,  (6*4)(k0);
    lw $7,  (7*4)(k0);
    lw $8,  (8*4)(k0);
    lw $9,  (9*4)(k0);
    lw $10, (10*4)(k0);
    lw $11, (11*4)(k0);
    lw $12, (12*4)(k0);
    lw $13, (13*4)(k0);
    lw $14, (14*4)(k0);
    lw $15, (15*4)(k0);
    lw $16, (16*4)(k0);
    lw $17, (17*4)(k0);
    lw $18, (18*4)(k0);
    lw $19, (19*4)(k0);
    lw $20, (20*4)(k0);
    lw $21, (21*4)(k0);
    lw $22, (22*4)(k0);
    lw $23, (23*4)(k0);
    lw $24, (24*4)(k0);
    lw $25, (25*4)(k0);
 /* lw $26, (26*4)(k0); k0 */
 /* lw $27, (27*4)(k0); k1 */
    lw $28, (28*4)(k0);
    lw $29, (29*4)(k0);
    lw $30, (30*4)(k0);
    lw $31, (31*4)(k0);
    lw k1,  (32*4)(k0)
    mtlo k1
    lw k1,  (33*4)(k0)
    mthi k1
    lw k1,  (34*4)(k0)
    mtc0 k1, CP0_EPC
   #  lw   k0, (35*4)(k0)
   #  mtc0 k0, CP0_STATUS
    .endm

/*
 * 异常处理总入口
 */
NESTED(exception_entry, 0, sp)
    .set push
    .set noat
    
    addiu k0, sp, -4 * 36
    save_context_registers
    addiu sp, sp, -4 * 36

    jal	exception_entry_c
	 nop

    move k0, sp
    addiu sp,  sp, 4*36
    restore_context_registers
    ehb
    eret
    nop

    .set pop
END(exception_entry)


