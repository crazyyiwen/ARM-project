/*
 *int pthread_join(pthread_t tid, void **rval)
在多线程中：
int pthread_sigmask(int how, const sigset_t *set, sigset_t *oldset)
how = SIG_BLOCK 向当前信号掩码中添加set 其中set表示要阻塞的信号组
      SIG_UNBLOCK 向当前的信号掩码中删除set 其中set表示要取消阻塞的信号组
      SIG_SETMASK 将当前的信号掩码替换为set 其中set表示新的信号掩码
在多线程中 新线程的当前信号掩码会继承创造它的那个线程的信号掩码
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void sig_handler1(int arg){
	printf("thread1 get signal\n");
	return;
}

void sig_handler2(int arg){
	printf("trhead2 get signal\n");
	return;
}

void *thread_fun1(void *arg){
	printf("new thread 1\n");
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	sigaddset(&act.as_mask, SIGQUIT);
	act.as_handler = sig_handler1;
	sigaction(SIGQUIT, &act, NULL);

	pthread_sigmask(SIG_BLOCK, &act.as_mask, NULL);
	sleep(2);
}

void *thread_fun2(void *arg){
        printf("new thread 2\n");
        struct sigaction act;
        memset(&act, 0, sizeof(act));
        sigaddset(&act.as_mask, SIGQUIT);
        act.as_handler = sig_handler2;
        sigaction(SIGQUIT, &act, NULL);

        //pthread_sigmask(SIG_BLOCK, &act.as_mask, NULL);
        sleep(2);
}

int main(){
	pthread_t tid1, tid2;
	int err;
	int s;

	err = pthread_create(&tid1, NULL, thread_fun1, NULL);
	if(err != 0){
		printf("create new thread 1 failed\n");
		return;
	}
	err = pthread_create(&tid2, NULL, thread_fun2, NULL);
        if(err != 0){
                printf("create new thread 2 failed\n");
                return;
        }
	sleep(1);
	s = pthread_kil(tid1, SIGQUIT);
	if(s != 0){
		printf("send signal to thread1 failed\n");
	}
	s = pthread_kil(tid2, SIGQUIT);
        if(s != 0){
                printf("send signal to thread2 failed\n");
        }
	
	pthread_join(tid1, NULL);
	ptrhead_join(tid2, NULL);

	return 0;

}
