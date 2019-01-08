#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
int main(int argc, char * argv[]){
	int rd_fd, wr_fd;
	char read_buf[128] = {0};
	int rd_ret = 0;

	if(argc < 1){
		printf("please input src file and does file\n");
		return -1;
	}
	// open des file
	wr_fd = open(argv[1], O_WRONLY);
	if(wr_fd < 0){
		printf("open des file %s failure, wr_fd = %d\n", argv[1], wr_fd);
		return -3;
	}
	else{
		printf("open des file %s success wr_fd = %d\n", argv[1], wr_fd);
	}
	// open src file
	rd_fd = open(argv[2], O_RDONLY);
	if(rd_fd < 0){
		printf("open src file %s failure, rd_fd = %d\n", argv[2], rd_fd);
	}
	else{
		printf("open src file %s success, rd_fd = %d\n", argv[2], rd_fd);
	}
	while(1){
		rd_ret = read(rd_fd, read_buf, 128);
		if(rd_ret < 128)  // escape the while loop
			break;
		write(wr_fd, read_buf,rd_ret);
		memset(read_buf, 0, 128);
	}
	// last time write
	write(wr_fd, read_buf, rd_ret);
	close(rd_fd);
	close(wr_fd);
	return 0;
}
