#pragma once
#include "kernel/interrupts.h"

void scheduler(void) __attribute__((noreturn));
void sched(void);
void yield(void);
void __attribute__((naked)) ctx_switch(cpu_context_t** old, cpu_context_t* new_);