#include "UDP.h"

int sockfd = -1;
struct sockaddr_in peer_addr = {0};

void print_err(char *str, int line, int err_no){
	
	printf("%d, %s, %s\n", line, str, strerror(err_no));
	exit(-1);
}

void *pth_fun(void *pth_arg){
	
	int ret = -1;
	char buf[100] = {0};
	int peer_addr_size = 0;
	while(1){
		
		peer_addr_size = sizeof(peer_addr);
		bzero(buf, sizeof(buf));
		/* receive B data and PORT and IP*/
		ret = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&peer_addr, &peer_addr_size);
		if(ret == -1){
			print_err("recvfrom failed", __LINE__, errno);
		}
		else if(ret > 0){

			printf("peer_IP: %s  peer_PORT: %d\n", inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
			printf("%s\n", buf);
		}
	}
	
	return NULL;
}

int main(int argc, char **argv){

	int ret = -1;
	char buf[100] = {0};
	if(argc != 3){
		printf("not enough argu\n");
	}
	/*create socket file */
	sockfd = socket(PF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1){
		print_err("socket failed", __LINE__, errno);	
	}
	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(5001);
	addr.sin_addr.s_addr = inet_addr("192.168.1.84");
	
	/*bind B PORT and IP */
	ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	if(ret == -1){
		print_err("bind fail", __LINE__, errno);
	}
	
	/*sub thread receive the data from B */
	pthread_t tid;
	ret = pthread_create(&tid, NULL, pth_fun, NULL);
	if(ret != 0){
		print_err("pthread_create failed", __LINE__, errno);
	}

	/*main thread send data */
	while(1){
		
		/*set B PORT and IP */
		peer_addr.sin_family = AF_INET;
	        peer_addr.sin_port = htons(atoi(argv[2]));
                peer_addr.sin_addr.s_addr = inet_addr(argv[1]);

		bzero(buf, sizeof(buf));
		scanf("%s", buf);
		ret = sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&peer_addr, sizeof(peer_addr));
		if(ret == -1){
			print_err("sendto failed", __LINE__, errno);
		}
	}
	
	return 0;
}
