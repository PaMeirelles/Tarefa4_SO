#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>


void main(void){
  int msgID, id;
	msgID = msgget(0xBEEF, 0666 | IPC_CREAT);
	char buffer;	
	int res;
	for(int i=0; i < 128; i++)	
	{
		res = msgrcv(msgID, &buffer, 1, 0, 0);	
		if(res == -1) 
		{ 
			perror("Erro ao receber mensagem"); 
		}
    printf("Recebeu %c\n", buffer);
	}
}