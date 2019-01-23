/*
注册和销毁清理函数
线程可以安排它退出时的清理操作 这与进程的可以用atexit函数安排进程退出时需要调用的函数类似 这样的函数称为线程清理处理程序 线程可以建立多个处理程序 处理程序记录在栈中 所以这些处理程序执行的顺序与他们注册的顺序相反
pthread_cleanup_push(void (*rtn) (void *), void *args) 注册处理程序
pthread_cleanup_pop(int excute) 清除处理程序
这两个函数要成对出现 否则编译无法通过
--当执行以下操作时调用清理函数 清理函数的参数由args传入
1 调用pthread_exit
2 响应取消请求
3 用非零参数调用pthread_cleanup_pop
 *int pthread_join(pthread_t tid, void **rval)
 *
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void *first_clean(void *arg){
	printf("%p first clean\n", arg);
	return(void *)0;
}

void *second_clean(void *arg){
        printf("%p second clean\n", arg);
        return(void *)0;
}

void *thread_fun1(void *arg){
	printf("new thread 1\n");
	pthread_cleanup_push(first_clean, "thread1");
	pthread_cleanup_push(second_clean, "thread1");

	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);

	return (void *)1;
}

void *thread_fun2(void *arg){
        printf("new thread 2\n");
        pthread_cleanup_push(first_clean, "thread2");
        pthread_cleanup_push(second_clean, "thread2");
	
	pthread_exit((void *)2);
        pthread_cleanup_pop(0);
        pthread_cleanup_pop(0);

        //pthread_exit((void *)2);
}

int main(){
	pthread_t tid1, tid2;
	int err;

	err = pthread_create(&tid1, NULL, thread_fun1, NULL);
	if(err != 0){
		printf("create new thread 1 failed\n");
		return -1;
	}
	err = pthread_create(&tid2, NULL, thread_fun2, NULL);
        if(err != 0){
                printf("create new thread 1 failed\n");
                return -1;
        }
	sleep(2);
	return 0;
}

