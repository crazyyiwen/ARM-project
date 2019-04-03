#include "TCP_server.h"

int cfd;

void print_err(char *str, int line, int err_no){

	printf("%d, %s, %s\n", line, str, strerror(err_no));
	exit(-1);
}

void *pth_fun(void *pth_arg){
	
	int ret = -1;
	char buf[100] = {0};
	while(1){
		
		bzero(buf, sizeof(buf));
		ret = read(cfd, buf, sizeof(buf));
		if(ret == -1){

			print_err("socket failed", __LINE__, errno);
		}
		else{
			
			printf("%s\n", buf);
		}	
	}	
}

void signal_fun(int signo){

	if(SIGINT == signo){

		remove("./UNIX_SOCK");
		exit(-1);
	}
}

int main(){
	
	int ret = 0;
	int sockfd = -1;
	struct sockaddr_un addr = {0};
	
	signal(SIGINT, signal_fun);

	sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
	if(sockfd == -1){
		
		print_err("socket failed", __LINE__, errno);		
	}	
	//
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "./UNIX_SOCK");
	ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	ret = listen(sockfd, 3);
	//
	if(ret == -1){

		print_err("listen failed", __LINE__, errno);
	}

	
	cfd = accept(sockfd, NULL, 0);
	if(cfd == -1){

		print_err("accept failed", __LINE__, errno);
	}
	
	pthread_t tid;
	ret = pthread_create(&tid, NULL, pth_fun, NULL);
	if(ret != 0){	
		
		print_err("pthread_create failed", __LINE__, errno);
	}	
	char buf[100] = {0};
	while(1){
		
		bzero(buf, sizeof(buf));
		scanf("%s", buf);
		ret = write(cfd, buf, sizeof(buf));
		if(ret == -1){
			
			print_err("write failed", __LINE__, errno);
		}
	}

	return 0;
}
