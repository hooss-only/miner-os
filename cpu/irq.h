extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

typedef struct {
  unsigned int ds; /* Data segment selector */
  unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax; /* Pushed by pusha. */
  unsigned int int_no, err_code; /* Interrupt number and error code (if applicable) */
  unsigned int eip, cs, eflags, useresp, ss; /* Pushed by the processor automatically */
} registers_t;

void irq_install();

typedef void (*isr_t)(registers_t);
void register_interrupt_handler(unsigned char n, isr_t handler);
