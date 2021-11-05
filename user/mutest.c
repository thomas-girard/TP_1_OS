#include "kernel/types.h"
#include "user/user.h"

int main(){
  int fd = create_mutex();
  acquire_mutex(fd);
  int pid = fork();
  if(pid < 0){
    printf("error fork\n");
    exit(1);
  } else if (pid == 0){
    acquire_mutex(fd);
    printf("Oui, père.\n");
    exit(0);
  } else {
    for(int i = 0; i < 1000; i++){
      printf("Fils, tu m'attendras\n");
    }
    release_mutex(fd);
    wait(0);
    exit(0);
  }
}

