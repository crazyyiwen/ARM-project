#include <sys/types.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct msgbuf{
	long type;
	char voltage[124];
	char ID[4];
};
int main(){
	int msg_id;
	int read_ret;
	struct msgbuf send_buf, receive_buf;
	int key;
	int pid;
	//-----------------------------------------------------
	key = ftok("./b.c", 'a');
	if(key < 0){
		printf("create key failure\n");
		return -2;
	}
	msg_id = msgget(key,IPC_CREAT |  0777);
	if(msg_id < 0){
		printf("create message queue failure\n");
		return -1;
	}
	printf("create message queue success msg_id=%d\n", msg_id);
	//-----------------------------------------------------
	system("ipcs -q");
	pid = fork();
	if(pid > 0){
		// init send_buf
		send_buf.type = 100;
		// write message
		while(1){
			memset(send_buf.voltage, 0, 124);
			printf("please input message:\n");
			fgets(send_buf.voltage, 124, stdin);
			msgsnd(msg_id, (void *)&send_buf, strlen(send_buf.voltage), 0);
		}
	}
	if(pid == 0){
		while(1){
			memset(receive_buf.voltage, 0, 124);
			msgrcv(msg_id, (void *)&receive_buf, 124, 200, 0);
			printf("receive message from message queue:%s", receive_buf.voltage);
		}
	}
	msgctl(msg_id, IPC_RMID, NULL);
	return 0;
}
