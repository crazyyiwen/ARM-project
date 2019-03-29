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

int ret_accept = -1;

typedef struct data{
        unsigned int stu_num;
        char stu_name[50];
}Data;

void print_err(char *str, int line, int err_no){

        printf("%d, %s: %s\n", line, str, strerror(err_no));
        exit(-1);
}
/* */
void signal_fun(int signo){
        if(signo == SIGINT){

                //close(ret_accept);
                shutdown(ret_accept, SHUT_RDWR);
                exit(0);
        }
}

/*sub thread fun */
void *pth_fun(void *arg){

        int ret = 0;
        Data stu_data = {0};
        while(1){

                bzero(&stu_data, sizeof(stu_data));
                ret = recv(ret_accept, &stu_data, sizeof(stu_data), 0);
                if(ret == -1){
                        printf("recv failed\n", __LINE__, errno);
                }
                printf("student number = %d,  name = %s\n", ntohl(stu_data.stu_num), stu_data.stu_name);
        }
}
int main(){

        int sockfd = -1;
        int ret = -1;
        /*create socket file */
        sockfd = socket(PF_INET, SOCK_STREAM, 0);
        pthread_t rec_tid;
        if(sockfd == -1){

                print_err("socket failed", __LINE__, errno);
        }
        /*bind sockaddr_in saddr  */
        struct sockaddr_in saddr;
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(SPORT);
        saddr.sin_addr.s_addr = inet_addr(SIP);
        ret = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
        if(ret == -1){
                print_err("bind failed", __LINE__, errno);
        }
        /*from possitive socket to passive socket but not for monitoring */
        ret = listen(sockfd, 3);
        if(ret == -1){
                print_err("listen failed", __LINE__, errno);
        }
        /*monitoring client */
        struct sockaddr_in clnaddr = {0};
        int clnaddr_size = sizeof(clnaddr);
        ret_accept = accept(sockfd, (struct sockaddr *)&clnaddr, &clnaddr_size);
        if(ret == -1){
                print_err("accept failed", __LINE__, errno);
        }
        printf("client_port = %d\n  client_ip = %s\n", ntohs(clnaddr.sin_port), inet_ntoa(clnaddr.sin_addr));
        /*create a thread to receive the client data */
        pthread_create(&rec_tid, NULL, &pth_fun, NULL);

        /*send message */
        Data stu_data = {0};
        unsigned int tmp_num;
        while(1){

                printf("input student number\n");
                scanf("%d", &tmp_num);
                stu_data.stu_num = htonl(tmp_num);
                printf("input student name\n");
                scanf("%s", stu_data.stu_name);
                ret = send(ret_accept, (void *)&stu_data, sizeof(stu_data), 0);
                if(ret == -1){
                        print_err("bind failed", __LINE__, errno);
                }
        }

        return 0;
}
                 
