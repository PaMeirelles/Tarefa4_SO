#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define LIM 32	
#define MAX_LOOP 128

void produtor(int msgID);	
void consumidor(int msgID);

int main(void)
{
	int msgID, id;
	msgID = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);	/

	struct msqid_ds aux;
	
  msgctl(msgID, IPC_STAT, &aux);	
	aux.msg_qbytes = sizeof(char)*LIM;
  msgctl(msgID, IPC_SET, &aux);
	
	if(fork() == 0)		{
		produtor(msgID);
	}
	else
	{
		consumidor(msgID);
	}
}

void produtor(int msgID){
	int i, res;	
  for(i = 0; i < MAX_LOOP; i++){
		buffer.mtype = 1;
		res = msgsnd(msgID, 'X', 1, 0);	
	}	
	buffer.mtype = 2;
	res = msgsnd(msgID, &buffer, 0, 0);
}
void consumidor(int msgID)
{
	char produto;	
	int res;
	
    for(int i = 0; i < MAX_LOOP; i++){
		res = msgrcv(msgID, &buffer, sizeof(buffer.mNum), 0, 0);	// Tira mensagem da fila e coloca no buffer 

		
        if(res == -1) 
		{ 
			perror("Erro ao receber mensagem"); 
		}
		
        if(buffer.mtype == 2)	// Se a mensagem for do tipo 2, é porque o produtor parou de produzir
		{
			printf("Acabou\n");
			break;
		}

		printf("Consumidor recebendo %c da fila de mensagens\n", buffer.mNum);
		// sleep(1);
	}
}