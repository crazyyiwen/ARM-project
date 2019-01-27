#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define BUFFER_SIZE  5
#define PRODUCT_CNT  30

struct product_cons{
	int buffer[BUFFER_SIZE];
	pthread_mutex_t lock;
	int readpos, writepos;
	pthread_cond_t notempty;
	pthread_cond_t notfull;
}buffer;

void init(struct product_cons *p){
	pthread_mutex_init(&p->lock, NULL);
	pthread_cond_init(&p->notempty, NULL);
	pthread_cond_init(&p->notfull, NULL);
	p->readpos = 0;
	p->writepos = 0;
}

void finish(struct product_cons *p){
	pthread_mutex_destroy(&p->lock);
	pthread_cond_destroy(&p->notempty);
	pthread_cond_destroy(&p->notfull);
	p->readpos = 0;
	p->writepos = 0;
}

void put(struct product_cons *p, int data){
	pthread_mutex_lock(&p->lock);
	if((p->writepos+1)%BUFFER_SIZE == p->readpos){
		printf("producer wait for not full \n");
		pthread_cond_wait(&p->notfull, &p->lock);
	}
	p->buffer[p->writepos] = data;
	p->writepos++;

	if(p->writepos >= BUFFER_SIZE)
		p->writepos = 0;

	pthread_cond_signal(&p->notempty);
	pthread_mutex_unlock(&p->lock);
}

int get(struct product_cons *p){
	int data;
	pthread_mutex_lock(&p->lock);
        if(p->readpos == p->writepos){
                printf("consumer wait for not empty \n");
                pthread_cond_wait(&p->notempty, &p->lock);
        }
        data = p->buffer[p->readpos];
	p->readpos++;

        if(p->readpos >= BUFFER_SIZE)
                p->readpos = 0;

        pthread_cond_signal(&p->notfull);
        pthread_mutex_unlock(&p->lock);
	return data;


}

void *producer(void *data){
	int n;
	for(n=1; n<=50; n++){
		sleep(1);
		printf("put the %d product...\n", n);
		put(&buffer, n);
		printf("put the %d product success\n", n);
	}
	printf("producer stopped\n");
	return NULL;
}

void *consumer(void *data){
	static int cnt = 0;
	int num;
	while(1){
		sleep(2);
		printf("get product...\n");
		num = get(&buffer);
		printf("get the %d product success\n", num);
		if(++cnt == PRODUCT_CNT)
			break;
	}
	printf("consumer stopped\n");
        return NULL;
}

int main(int argc, char *argv[]){
	pthread_t th_a, th_b;
	void *retval;

	init(&buffer);

	pthread_create(&th_a, NULL, producer, 0);
	pthread_create(&th_b, NULL, consumer, 0);

	pthread_join(th_a, &retval);
	pthread_join(th_b, &retval);

	finish(&buffer);

	return 0;
}
