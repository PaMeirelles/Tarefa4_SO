#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(void){
  int msgID, id;
	msgID = msgget(0xBEEF, 0666 | IPC_CREAT);
	struct msqid_ds aux;		
	char buffer;
	int i;
	for(i = 0; i < 128; i++){
		buffer =  i % ('Z' - 'A' + 1) + 'A';	
		msgsnd(msgID, &buffer, 1, 0);	
		printf("Enviou %c", buffer);
    }
}