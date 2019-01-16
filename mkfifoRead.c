#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>

int main(){
	int fd;
	char process_inter = 0;
	fd = open("./myfifo", O_RDONLY);
	if(fd < 0){
		printf("open myfifo failure\n");
		return -1;
	}
	printf("open myfifo success\n");

	for(int i=0; i<5; i++){
		printf("this is the second process\n");
		usleep(100);
	}
	read(fd, &process_inter, 1);
	while(process_inter == 0);
	while(1);
	return 0;
}
