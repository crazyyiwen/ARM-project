/*
 *int pthread_join(pthread_t tid, void **rval)
调用该函数的线程会一直阻塞 直到指定的线程tid调用pthread_exit 从启动历程返回或者被取消
参数tid就是制定线程的id
参数rval是指定线程的返回码 如果线程被取消 那么rval被置为PTHREAD_CANCELED
返回值：该函数调用成功会返回0 失败会返回错误码
调用pthread_join会使指定的线程处于分离状态 如果指定线程处于分离状态 那么调用就会失败
pthread_detach可以分离一个线程 线程可以自己分离自己
int pthread_detach(pthread_t thread)
成功返回0 失败返回错误码
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

void *thread_fun1(void *arg){
	printf("thread1\n");
	return (void *)1;
}

void *thread_fun2(void *arg){
	printf("thread2\n");
	pthread_detach(pthread_self());
	pthread_exit((void *)2);
}

int main(){
	int err1, err2;
	pthread_t tid1, tid2;
	void *rval1, *rval2;

	err1 = pthread_create(%tid1, NULL, thread_fun1, NULL);
	err2 = pthread_create(%tid2, NULL, thread_fun2, NULL);

	if(err1 || err2){
		printf("create new thread failed\n");
		return 0;
	}

	printf("main thread\n");
	printf("join1 rval is %d\n", pthread_join(tid1, &rval1));
	printf("join2 rval is %d\n", pthread_join(tid2, &rval2));
	
	printf("trhead 1 exit code is %d\n", (int *)rval1);
	printf("trhead 2 exit code is %d\n", (int *)rval2);
	printf("main thread again\n");
}
