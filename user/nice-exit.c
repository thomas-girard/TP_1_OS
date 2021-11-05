#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

int main(){
  for(int i=0; i < 100; i++){
    int pid = fork();
    if(pid>0){
      int pid2 = fork();
      if(pid2>0){
        while(1){
          if(nice(pid,4) != 1) break;
          if(nice(pid,5) != 1) break;
        }
        exit(0);
      }
    } else if(pid == 0){
      exit(0);
    }
  }
  exit(1);
  return 0;
}
