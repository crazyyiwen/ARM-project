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
   #include <pthread.h>
   #include <signal.h>

   #define SPORT 5006
   #define SIP   "192.168.1.84"
