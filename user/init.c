// init: The initial user-level program

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


char consoleX[] = "consoleX";

char* make_consoleX(int i){
  if(i < 1 || i > 9){
    printf("Console number can not be more than 9.\n");
    exit(1);
  }
  int l = strlen(consoleX);
  char* str = malloc(l+1);
  strcpy(str, consoleX);
  str[7] = '0' + i;
  return str;
}


char *argv[] = { "sh", 0, 0 };

void shloop(int cur_console){

  char* cons = make_consoleX(cur_console);
  int fd;
  if((fd = open(cons, O_RDWR)) < 0){
    mknod(cons, 1, cur_console);
    fd = open(cons, O_RDWR);
  }
  dup(0);  // stdout
  dup(0);  // stderr
  int pid, wpid;

  for(;;){
    printf("init: starting sh\n");
    pid = fork();
    if(pid < 0){
      printf("init: fork failed\n");
      exit(1);
    }
    if(pid == 0){
      argv[1] = cons;
      exec("sh", argv);
      printf("init: exec sh failed\n");
      exit(1);
    }
    while((wpid=wait(0)) >= 0 && wpid != pid){
      //printf("zombie!\n");
    }
  }

}

int
main(void)
{

  int nb_consoles = 3;

  int cur_console = 0;
  int pid;

  while(cur_console++ < nb_consoles){
    pid = fork();
    if (pid < 0){
      printf("init: fork failed\n");
      exit(1);
    }
    if (pid == 0){
      shloop(cur_console);
      return 1;
    }
  }
  while(1) wait(0);
  return 0;
}
