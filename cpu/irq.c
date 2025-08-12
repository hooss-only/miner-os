#include "irq.h"

#include "idt.h"
#include "ports.h"

isr_t interrupt_handlers[256];

void irq_install() {
  // REMAP PIC
  port_byte_out(0x20, 0x11);
  port_byte_out(0xA0, 0x11);
  port_byte_out(0x21, 0x20);
  port_byte_out(0xA1, 0x28);
  port_byte_out(0x21, 0x04);
  port_byte_out(0xA1, 0x02);
  port_byte_out(0x21, 0x01);
  port_byte_out(0xA1, 0x01);
  port_byte_out(0x21, 0x0);
  port_byte_out(0xA1, 0x0); 
  
  // add IRQs
  set_idt_gate(32, (unsigned int) irq0);
  set_idt_gate(33, (unsigned int) irq1);
  set_idt_gate(34, (unsigned int) irq2);
  set_idt_gate(35, (unsigned int) irq3);
  set_idt_gate(36, (unsigned int) irq4);
  set_idt_gate(37, (unsigned int) irq5);
  set_idt_gate(38, (unsigned int) irq6);
  set_idt_gate(39, (unsigned int) irq7);
  set_idt_gate(40, (unsigned int) irq8);
  set_idt_gate(41, (unsigned int) irq9);
  set_idt_gate(42, (unsigned int) irq10);
  set_idt_gate(43, (unsigned int) irq11);
  set_idt_gate(44, (unsigned int) irq12);
  set_idt_gate(45, (unsigned int) irq13);
  set_idt_gate(46, (unsigned int) irq14);
  set_idt_gate(47, (unsigned int) irq15);
}

void register_interrupt_handler(unsigned char n, isr_t handler) {
  interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
  /* After every interrupt we need to send an EOI to the PICs
   * or they will not send another interrupt again */
  if (r.int_no >= 40) port_byte_out(0xA0, 0x20); /* slave */
  port_byte_out(0x20, 0x20); /* master */

  /* Handle the interrupt in a more modular way */
  if (interrupt_handlers[r.int_no] != 0) {
    isr_t handler = interrupt_handlers[r.int_no];
    handler(r);
  }
}
