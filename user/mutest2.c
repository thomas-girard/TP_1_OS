#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char fname[] = "moncompteur";

#define MAXLEN 20

int freadint(){
  int res = -1;
  char buf[MAXLEN];
  int fd = open(fname, O_RDONLY);
  if(read(fd, buf, MAXLEN) > 0){
    res = atoi(buf);
  }
  close(fd);
  return res;
}

void fwriteint(int i){
  int fd = open(fname, O_CREATE | O_RDWR);
  fprintf(fd, "%d\n", i);
  close(fd);
}

#define NITER 100

void inc(int mut){
  for(int i = 0; i < NITER; i++){
    acquire_mutex(mut);
    int val = freadint();
    fwriteint(val+1);
    release_mutex(mut);
  }
}

void dec(int mut){
  for(int i = 0; i < NITER; i++){
    acquire_mutex(mut);
    int val = freadint();
    fwriteint(val-1);
    release_mutex(mut);
  }
}


int main(){
  fwriteint(100);
  int mut = create_mutex();
  int pid = fork();
  if(pid < 0){
    printf("error fork\n");
    exit(1);
  } else if (pid == 0){
    inc(mut);
    exit(0);
  } else {
    pid = fork();
    if(pid < 0){
      printf("error fork\n");
      exit(1);
    } else if (pid == 0){
      dec(mut);
      exit(0);
    }
    wait(0);
    wait(0);
    printf("Result = %d\n", freadint());
    exit(0);
  }
}
