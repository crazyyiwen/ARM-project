#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
int main(){
	int fd;
	char process_inter = 0;
	fd = open("./myfifo", O_WRONLY);
	if(fd < 0){
		printf("open file failure\n");
		return -1;
	}
	printf("open myfifo success\n");

	for(int i=0; i<5; i++){
		printf("this is the first process\n");
		usleep(100);
	}
	process_inter = 1;
	write(fd, &process_inter, 1);
	usleep(5);
	while(1);
	return 0;
}
