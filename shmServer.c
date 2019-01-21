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

	signal(SIGUSR2, myfun);
	p = (struct mybuf *)shmat(shm_id, NULL, 0);
	if(p == NULL){
		printf("mapping failure\n");
	}
	printf("mapping success\n");
	//-----------------------------------------------------
	p->pid = getpid();	 // write server pid to share memory
	pause();		 // wait client to read serer pid 
	pid = p->pid;		 // get client pid
	while(1){
		printf("parent process starts writing share memory\n");
		fgets(p->buf, 128, stdin);
		kill(pid, SIGUSR1);  
		pause();	// wait client process read
	}
	
	//shmdt(p);
	//shmctl(shm_id, IPC_RMID, NULL);
	//printf("share memory data %s\n", p);
	return 0;
}
