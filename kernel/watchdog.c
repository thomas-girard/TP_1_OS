
#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"
#include "watchdog.h"

int watchdog_value;
int watchdog_time;
struct spinlock watchdog_lock;

int
watchdogwrite(struct file *f, int user_src, uint64 src, int n)
{
  acquire(&watchdog_lock);

  int time = 0;
  for(int i = 0; i < n; i++){
    char c;
    if(either_copyin(&c, user_src, src+i, 1) == -1)
      break;
    time = c;
  }

  acquire(&tickslock);
  n = ticks - watchdog_value;
  watchdog_value = ticks;
  watchdog_time = time;
  release(&tickslock);

  release(&watchdog_lock);
  return n;
}

void watchdoginit(){
  initlock(&watchdog_lock, "watchdog_lock");
  watchdog_time = 0;


  devsw[WATCHDOG].read = 0;
  devsw[WATCHDOG].write = watchdogwrite;
}
