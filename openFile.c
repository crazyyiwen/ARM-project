1 #include <stdio.h>
  2 #include <unistd.h>
  3 #include <fcntl.h>
  4 
  5 typedef char ElemType;
  6 
  7 int main(int argc, char *argv[]){
  8         int fd;
  9         ElemType buf[] = "hello linux\n";
 10         ElemType read_buf[128] = {0};
 11         int wr_ret, rd_ret;
 12         fd = open("./a.c", O_RDWR | O_TRUNC);
 13         if(fd < 0){
 14                 printf("open file failure\n");
 15                 return -1;
 16         }
 17         printf("open file success fd=%d\n", fd);
 18         wr_ret = write(fd, buf, sizeof(buf)/sizeof(ElemType));
 19         printf("wr_ret is %d\n", wr_ret);
 20         // start read
 21         rd_ret = read(fd, read_buf, 128);
 22         printf("\n%s\n", read_buf);
 23         printf("rd_ret is %d\n", rd_ret);
 24         close(fd);
 25         return 0;
 26 }
