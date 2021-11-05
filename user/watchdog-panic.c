#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int main(){
  int watchdog_fd;
  if((watchdog_fd = open("watchdog", O_WRONLY)) < 0){
    mknod("watchdog", 2, 1);
    watchdog_fd = open("watchdog", O_WRONLY);
  }
  while(1){
    printf("Watchdog...\n");
    char reset = 13;
    write(watchdog_fd, &reset, 1);
    sleep(15);
  }
  exit(0);
}
