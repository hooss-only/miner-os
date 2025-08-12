#ifndef IDT_H
#define IDT_H

#include "types.h"

#define KERNEL_DS 0x08

typedef struct {
  unsigned short low_offset; /* Lower 16 bits of handler function address */
  unsigned short sel; /* Kernel segment selector */
  unsigned char always0;
  /* First byte
   * Bit 7: "Interrupt is present"
   * Bits 6-5: Privilege level of caller (0=kernel..3=user)
   * Bit 4: Set to 0 for interrupt gates
   * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
  unsigned char flags; 
  unsigned short high_offset; /* Higher 16 bits of handler function address */
} __attribute__((packed)) idt_gate_t ;

typedef struct {
  unsigned short limit;
  unsigned int base;
} __attribute__((packed)) idt_register_t ;

#define IDT_ENTRIES 256

void set_idt_gate(int n, unsigned int handler);
void set_idt();

#endif
