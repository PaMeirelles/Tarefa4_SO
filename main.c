#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
 #include <sys/ipc.h>
 #include <sys/shm.h>

#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

sem_t * mutex;
sem_t * empty;
sem_t * full;

void fill_vector(char * p){
  for(int i=0; i < 32; i++){
    p[i] = '.';
  }
}


void produce(char * p){
  int value;
  int a, b, c;
  sem_t * mutex = sem_open("/m", O_RDWR);
  sem_t * empty = sem_open("/e", O_RDWR);
  sem_t * full = sem_open("/f", O_RDWR);
  for(int i=0; i < 128; i++){
    puts(p);
    sem_wait(empty);
    sem_wait(mutex);
    
    sem_getvalue(full, &value);
    p[value] = 'X';

    
    sem_post(mutex);
    sem_post(full);
    }
  }
void consume(char *  p){
  int val;
  int a, b, c;
  sem_t * mutex = sem_open("/m", O_RDWR);
  sem_t * empty = sem_open("/e", O_RDWR);
  sem_t * full = sem_open("/f", O_RDWR);
  for(int i=0; i < 128; i++){
    sem_wait(full);
    sem_wait(mutex);
    sem_getvalue(full, &val);
    p[val] = '.';
    sem_post(mutex);
    sem_post(empty);
    }
  }
int main(void) {
  int segmento, id, pid, status;
  char * p;
  mutex = sem_open("/m", O_CREAT | O_EXCL, SEM_PERMS, 1);
  empty = sem_open("/e", O_CREAT | O_EXCL, SEM_PERMS, 32);
  full = sem_open("/f", O_CREAT | O_EXCL, SEM_PERMS, 0);

  segmento = shmget (IPC_PRIVATE, sizeof (char) * 32 + sizeof(sem_t) * 3, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);  
  p = (char *) shmat (segmento, 0, 0);

  fill_vector(p);

  if(fork() == 0){
    consume(p); 
    exit(2);

    }    
   else{
    produce(p);  
    waitpid(-1, &status, 0);
  }

  
  shmdt (p);  
  shmdt (mutex);
  shmdt (full);
  shmdt (empty);

  shmctl (segmento, IPC_RMID, 0);
  
  return 0;}
