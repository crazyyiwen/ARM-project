#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
struct mybuf{
	int pid;
	char buf[124];
};

void myfun(int signum){
	return;
}

int main(){
	int shm_id;
	int key;
	struct mybuf *p;
	int pid;
	//------------------------------------------------------
	key = ftok("./b.c", 'a');
	if(key < 0){
		printf("creat key failure\n");
		return -5;
	}
	printf("creat key success\n");

	shm_id = shmget(key, 128, IPC_CREAT | 077);
	if(shm_id < 0){
		printf("create share memory failure\n");
		return -1;
	}
	printf("create share memory success shm_id=%d\n", shm_id);
	//------------------------------------------------------

	signal(SIGUSR1, myfun);
	p = (struct mybuf *)shmat(shm_id, NULL, 0);
	if(p == NULL){
		printf("mapping failure\n");
	}
	printf("mapping success\n");

	pid = p->pid;		 // read share memory
	p->pid = getpid();	 // write client pid to share memory
	kill(pid, SIGUSR2);	 // kill signal

	while(1){
		pause();	// wait server write data to share memory
		printf("client process receive data from share memory %s", p->buf);
		kill(pid, SIGUSR2);
		
	}
	
	//shmdt(p);
	//shmctl(shm_id, IPC_RMID, NULL);
	//printf("share memory data %s\n", p);
	return 0;
}
