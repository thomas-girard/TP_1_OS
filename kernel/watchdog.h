#ifndef WATCHDOG_H
#define WATCHDOG_H

extern int watchdog_value;
extern int watchdog_time;
extern struct spinlock watchdog_lock;

void watchdoginit();

#endif
