#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "libc/stdint.h"
#include "descriptor_tables.h"

#define ISR1 1
#define ISR2 2
#define ISR3 3
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
#define IRQ_COUNT 16

extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void init_irq();
void init_interrupts();

typedef struct registers {
      uint32_t ds;                  // Data segment selector
      uint32_t edi, esi, ebp, useless_value, ebx, edx, ecx, eax; // Pushed by pusha.
      uint32_t int_no, err_code;    // Interrupt number and error code (if applicable)
      uint32_t eip, cs, eflags, esp, ss; // Pushed by the processor automatically.
  } registers_t;

// Enables registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the first parameter
typedef void (*isr_t)(registers_t*, void*);

// Structure to hold information about an interrupt handler
struct int_handler_t {
  int num;
  isr_t handler;
  void *data;
};

// Define an interrupt handler
void register_irq_handler(int irq, isr_t handler, void* ctx);
void register_interrupt_handler(uint8_t n, isr_t handler, void*);

static struct int_handler_t int_handlers[IDT_ENTRIES];
static struct int_handler_t irq_handlers[IRQ_COUNT];

#define IRQ_COUNT 16

#endif