#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2){
    printf("print: not enough arguments\n");
    exit(1);
  }
  int i = 0;
  while (1){
    printf("%s %d\n", argv[1], i);
    for(int j = 0; j < (1 << 23); j++);
    i++;
  }
  exit(0);
}
