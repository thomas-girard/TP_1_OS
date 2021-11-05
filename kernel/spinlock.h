#ifndef SPINLOCK_H
#define SPINLOCK_H

// Mutual exclusion lock.
struct spinlock {
  uint locked;       // Is the lock held?

  // For debugging:
  char *name;        // Name of lock.
  struct cpu *cpu;   // The cpu holding the lock.
  uint64 pc;
  int pid;
  uint n;
  uint nts;
};

void dump_locks();

#endif
