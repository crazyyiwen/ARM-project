#include <sys/types.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
void myfun(int signum){
	return;
}
int main(){
	int shm_id;
	int key;
	char *p;
	int pid;
	shm_id = shmget(IPC_PRIVATE, 128, IPC_CREAT | 077);
	if(shm_id < 0){
		printf("create share memory failure\n");
		return -1;
	}
	printf("create share memory success shm_id=%d\n", shm_id);
	pid = fork();
	if(pid > 0){	
		signal(SIGUSR2, myfun);
		p = (char *)shmat(shm_id, NULL, 0);
		if(p == NULL){
			printf("parent process:shmat failure\n");
			return -3;
		}
		printf("parent process:shmat success\n");
		while(1){
			printf("parent process starts writing share memory\n");
			fgets(p, 128, stdin);
			kill(pid, SIGUSR1);  
			pause();
		}
	}
	if(pid == 0){
		signal(SIGUSR1, myfun);
		p = (char *)shmat(shm_id, NULL, 0);
		if(p == NULL){
			printf("child process:shmat failure\n");
			return -4;
		}
		printf("child process:shmat success\n");
		while(1){
			pause();
			printf("share memory data:%s\n", p);
			kill(getppid(), SIGUSR2);
		}
	}
	//shmdt(p);
	//shmctl(shm_id, IPC_RMID, NULL);
	//printf("share memory data %s\n", p);
	return 0;
}
