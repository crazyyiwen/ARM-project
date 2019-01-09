#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
//#include <string>
typedef char ElemType;

int main(int argc, char *argv[]){
	FILE *src_fp;
	int rd_ret;
	int wr_ret;
	int feof_ret;
	if(argc < 2){
		printf("please input src file\n");
		return -1;
	}
	src_fp = fopen(argv[1], "r");
	if(src_fp == NULL){
		printf("open src file failure\n");
		return -2;
	}
	else{
		printf("open src file sccess\n");
	}
	// start read and write then exit the while loop when operation is  finished
	while(1){
		rd_ret = fgetc(src_fp);
		// no need to open the stdout again    
		if(feof(src_fp)){
			printf("read file end\n");
			break;
		}
		fputc(rd_ret, stdout);
	}

	fclose(src_fp);
	return 0;
}
