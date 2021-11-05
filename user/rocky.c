#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define MAXLEN 20

int freadint(char * fname){
  int res = -1;
  char buf[MAXLEN];
  int fd = open(fname, O_RDONLY);
  if(read(fd, buf, MAXLEN) > 0){
    res = atoi(buf);
  }
  int j = 0;
  for(int i = 0; i < (1 << 25); i++)
    j += i;
  close(fd);
  return res;
}

void fwriteint(char* fname, int i){
  int fd = open(fname, O_CREATE | O_RDWR);
  fprintf(fd, "%d\n", i);
  int j = 0;
  for(int i = 0; i < (1 << 25); i++)
    j += i;
  close(fd);
}

void
panic(char *s)
{
  fprintf(2, "%s\n", s);
  exit(1);
}

int
fork1(void)
{
  int pid;
  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}

void get_weather(int mut){
  acquire_mutex(mut);
  printf("Getting weather\n");
  int w = freadint("donnees_meteo");
  fwriteint("a_envoyer_a_la_terre", w);
  release_mutex(mut);
  printf("Exiting weather\n");
  sleep(2);
}

int lasttime = 0;
#define THRESHOLD 20
#define NITER 100
void watchdog(int mut, int watchdog_fd){
  while(1){
    printf("Watchdog...\n");
    acquire_mutex(mut);
    char reset = 20;
    int time_since_last_write = write(watchdog_fd, &reset, 1);
    printf("time_since_last_write = %d\n", time_since_last_write);
    release_mutex(mut);
    sleep(15);
  }
}

void transmit_to_earth(){
  printf("Transmitting...\n");
  int j = 0;
  for(int i = 0; i < (1 << 30); i++)
    j += i;
  printf("Done transmitting.\n");
  sleep(2);
}

int main(){
  int watchdog_fd;
  if((watchdog_fd = open("/watchdog", O_WRONLY)) < 0){
    mknod("/watchdog", 2, 0);
    watchdog_fd = open("/watchdog", O_WRONLY);
  }
  fwriteint("donnees_meteo", 12);
  int mut = create_mutex();
  int pid_get_weather;
  int pid_transmit_to_earth;
  int pid_watchdog;
  lasttime = uptime();
  if((pid_get_weather = fork1()) == 0){
		sleep(10);
		for(int i = 0; i < NITER; i++)
	    get_weather(mut);
    exit(0);
  } else if((pid_transmit_to_earth = fork1()) == 0){
		sleep(10);
		for(int i = 0; i < NITER; i++)
	    transmit_to_earth();
    exit(0);
  } else if((pid_watchdog = fork1()) == 0){
		sleep(10);
    watchdog(mut, watchdog_fd);
    exit(0);
  } else {
    nice(pid_watchdog, 0);
    nice(pid_transmit_to_earth, 5);
    nice(pid_get_weather, 9);
    wait(0);
    wait(0);
    wait(0);
    exit(0);
  }
}
