/**
 * @file sched.h
 * @brief Task scheduler.
 * @details This file contains the task scheduler's logic and implementation.
 */

#ifndef SCHED_H
#define SCHED_H
#include "kernel/interrupts.h"

void scheduler(void) __attribute__((noreturn));
void sched(void);
void yield(void);
void ctx_switch(cpu_context_t** old, cpu_context_t* new_) __attribute__((naked));

#endif // SCHED_H
