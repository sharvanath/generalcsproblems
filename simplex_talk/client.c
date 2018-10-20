#include<unistd.h>
#include<string.h>
#include<netdb.h>
#include<stdio.h>

int main() {
 const int port = 1990;
 int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
 struct sockaddr_in s_peer;
 struct hostent * server = gethostbyname("127.0.0.1");

 bzero((char *)&s_peer.sin_addr, sizeof(s_peer.sin_addr));
 bcopy((char *)&s_peer.sin_addr.s_addr, server->h_addr, server->h_length);
 s_peer.sin_family = AF_INET;
 s_peer.sin_port = htons(port);

 int option = 0;
 if (connect(sock_fd, (struct sockaddr *) &s_peer, sizeof(s_peer)) == 0) {
   char buf[100] = "hellosdfa";
   int len = write(sock_fd, buf, 5);
   if (len < 0) {
    perror("Write error");
   } else {
     printf("len = %d\n", len);
   }

 } else
   perror("connect error");
 usleep(1000000);
 close(sock_fd);
}
