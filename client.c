 #include <stdio.h>
  2 #include <sys/types.h>
  3 #include <dirent.h>
  4 #include <string.h>
  5 #include <unistd.h>
  6 #include <fcntl.h>
  7 
  8 int main(){
  9         int src_fd, des_fd;
 10         DIR *dp;
 11         int fd, ret;
 12         struct dirent *dir;
 13         char server[128] = {0};
 14         char file[128] = {0};
 15         char buf[128] = {0};
 16 start:  printf("please input server PATH and Directory name\n");
 17         scanf("%s", server);
 18         //list server files
 19         dp = opendir(server);
 20         if(dp == NULL){
 21                 printf("open server:%s failure\n", server);
 22                 goto start;
 23         }
 24         printf("open server:%s success\n", server);
 25 
 26         while(1){
 27                 dir = readdir(dp);
                if(dir == NULL)
 29                         break;
 30                 else{
 31                         printf("inode=%ld\t file_name=%s\n", dir->d_ino, di    r->d_name);
 32                 }
 33         }
 34         printf("\nplease input download file\n");
 35         scanf("%s", file);
 36         src_fd = open(strcat(strcat(server, "/"), file), O_RDONLY);
 37         if(src_fd < 0){
 38                 printf("open download file %s failure \n", file);
 39                 return -1;
 40         }
 41         printf("open download file %s success\n", file);
 42         des_fd = open(file, O_CREAT | O_WRONLY, 0777);
 43         if(des_fd < 0){
 44                 printf("creat file %s failure\n", file);
 45                 return -2;
 46         }
 47         printf("creat file %s success\n", file);
 48 
 49         while(1){
 50                 ret = read(src_fd, buf, 128);
 51                 if(ret < 128)
                        break;
 53                 write(des_fd, buf, ret);
 54         }
 55         write(des_fd, buf, ret);
 56         close(src_fd);
 57         close(des_fd);
 58         closedir(dp);
 59         return 0;
 60 }
