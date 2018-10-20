#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main() {

  const int PORT = 1990;
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in self;

  bzero(&self, sizeof(self));
  self.sin_port = htons(PORT);
  self.sin_family = AF_INET;
  self.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock_fd, (struct sockaddr *)&self, sizeof(self)) != 0) {
    perror("bind failed");
    exit(1);
  }

  if (listen(sock_fd, 10) != 0) {
    perror("listen failed");
  }

  int client_fd;
  struct sockaddr_in cl_addr;
  socklen_t cl_size = sizeof(cl_addr);
  while (1) {
    if((client_fd = accept(sock_fd, (struct sockaddr *)&cl_addr, &cl_size)) > 0) {
      char buf[100];
      int len = recv(client_fd, buf, 50, 0);
      printf("got %d bytes\n", len);
      close(client_fd);
    } else {
      perror("Error in accept");
    }
  }

}
