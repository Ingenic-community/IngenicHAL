main_stack_size = 2048;
sram_start = 0xF4000000;
sram_size = 8192;
MEMORY { .sram : ORIGIN = sram_start, LENGTH = sram_size }

OUTPUT_ARCH(mips)
ENTRY(_start)

SECTIONS
{
 .text :
 {
  __start = .;
  *(.start_section*)
  __text_start = .;
  *(.text*)
  __text_end = .;
 } >.sram
 
 .rodata : {
  . = ALIGN(4);
  *(SORT_BY_ALIGNMENT(.rodata*))
 } >.sram

 .data : {
  . = ALIGN(4);
  *(SORT_BY_ALIGNMENT(.data*))
 } >.sram

 .sdata : {
  . = ALIGN(4);
  *(SORT_BY_ALIGNMENT(.sdata*))
 } >.sram

 . = ALIGN(4);
 __image_copy_end = .;
 
 .bss : {
  . = ALIGN(4);
  __bss_start = .;
  *(.sbss.*)
  *(.bss.*)
  *(COMMON)
  . = ALIGN(4);
 } >.sram

 .bss_end : {
  . = ALIGN(4);
  __bss_end = .;
 } >.sram

 .user_stack : {
	. = ALIGN(8);
    _user_stack_start = .;
    . = . + main_stack_size;
    KEEP(*(.user_stack))
    _user_stack_end = .;
  } >.sram

 /DISCARD/ : {
  *(.dynbss)
  *(.dynstr)
  *(.dynamic)
  *(.interp)
  *(.hash)
  *(.gnu.*)
  *(.note.*)
  *(.plt)
  *(.got.plt)
  *(.rel.plt)
  *(.rel.dyn)
 }
}
