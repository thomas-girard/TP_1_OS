#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid;
  for(int i = 0; i < 4; i++){
    pid = fork();
    if(pid == 0){
      char* argv[] = {"print", "X", 0};
      argv[1][0] = 'A' + i;
      exec("/print", argv);
      exit(1);
    }
  }
  while(wait(0) != -1);
  exit(0);
}
