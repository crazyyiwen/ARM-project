#include "TCP_server.h"

//int cfd;
int sockfd;

void print_err(char *str, int line, int err_no){

	printf("%d, %s, %s\n", line, str, strerror(err_no));
	exit(-1);
}

void *pth_fun(void *pth_arg){
	
	int ret = -1;
	char buf[100] = {0};
	while(1){
		
		bzero(buf, sizeof(buf));
		ret = read(sockfd, buf, sizeof(buf));
		if(ret == -1){

			print_err("socket failed", __LINE__, errno);
		}
		else{
			
			printf("%s\n", buf);
		}	
	}	
}

int main(){
	
	int ret = 0;
	struct sockaddr_un seraddr = {0};
	sockfd = socket(PF_UNIX, SOCK_STREAM, 0);
	if(sockfd == -1){
		
		print_err("socket failed", __LINE__, errno);		
	}	
	//
	seraddr.sun_family = AF_UNIX;
	strcpy(seraddr.sun_path, "./UNIX_SOCK");
	
	ret = connect(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
	if(ret == -1){
		print_err("connect failed", __LINE__, errno);
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
		ret = write(sockfd, buf, sizeof(buf));
		if(ret == -1){
			
			print_err("write failed", __LINE__, errno);
		}
	}

	return 0;
}
