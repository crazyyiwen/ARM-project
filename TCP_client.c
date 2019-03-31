/*
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SPORT 5006
#define SIP   "192.168.1.84"
*/

#include "TCP_server.h"



typedef struct data{
	unsigned int stu_num;
	char stu_name[50];
}Data;

int sockfd = -1;

void print_err(char *str, int line, int err_no){

	printf("%d, %s: %s\n", line, str, strerror(err_no));
	exit(-1);
}

void signal_fun(int signo){
        if(signo == SIGINT){

                //close(ret_accept);
                shutdown(sockfd, SHUT_RDWR);
                exit(0);
        }
}

void *pth_fun(void *pth_arg){
	
	int ret = 0;
	Data stu_data = {0};
	
	while(1){
		
		bzero(&stu_data, sizeof(stu_data));
		ret = recv(sockfd, (void *)&stu_data, sizeof(stu_data), 0);
		if(ret > 0){
			//print_err("recv success", __LINE__, errno);
			printf("student number %d,  student name %s\n", ntohl(stu_data.stu_num), stu_data.stu_name);
		}
		else if(ret == -1){
			print_err("recv failed", __LINE__, errno);
		}
	}
}

int main(){
	
	int ret = 0;
	signal(SIGINT, signal_fun);
	/*create socket */
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if(sockfd == -1){
		print_err("socket failed\n", __LINE__, errno);
	}
	/*connect to server */
	struct sockaddr_in seraddr = {0};   // save the server IP and port
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(SPORT);
	seraddr.sin_addr.s_addr = inet_addr(SIP);  // used for LAN communication
	ret = connect(sockfd, (struct sockaddr *)&seraddr, sizeof(seraddr));
	if(ret == -1){
		print_err("connect failed\n", __LINE__, errno);
	}
	/*create child thread */
	pthread_t tid;
	ret = pthread_create(&tid, NULL, pth_fun, NULL);
	if(ret != 0){
		print_err("ptrhead_create failed", __LINE__, errno);
	}

	Data stu_data = {0};
	unsigned int tmp_num = 0;
	while(1){
		
		bzero(&stu_data, sizeof(stu_data));
		printf("input student number\n");
		scanf("%d", &tmp_num);
		stu_data.stu_num = htonl(tmp_num);
		
		printf("input student name\n");
		scanf("%s", stu_data.stu_name);
		
		ret = send(sockfd, (void *)&stu_data, sizeof(stu_data), 0);
		if(ret == -1){
			print_err("send failed", __LINE__, errno);
		}
	}
	return 0;
}
