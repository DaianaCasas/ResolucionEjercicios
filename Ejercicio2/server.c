#include <linux/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>


int main(int argc, char * argv[]){
  int listendFd=0, connFd=0;
  int n=0;
  struct sockaddr_in server_addr;
  char sendMsg[1025];
  char recMsg[1025];
  time_t ticks;

  listendFd = socket(AF_INET, SOCK_STREAM,0);

  memset(&server_addr, '0', sizeof(server_addr));
  memset(sendMsg, '0', sizeof(sendMsg));

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(5000); 

  bind(listendFd, (struct sockaddr*)&server_addr, sizeof(server_addr)); 

  listen(listendFd, 10); 
  /* Response Message: ASCII
  | Tipo de mensaje | Codigo Respuesta |
  Tipo de mensaje: 0210
  Codigo de respuesta: 2 digitos 
  */
  while(1){
    connFd = accept(listendFd, (struct sockaddr*)NULL, NULL); 
    ticks = time(NULL);
    read(connFd, recMsg, strlen(recMsg)); 
    fputs(recMsg, stdout);
    if(n < 0)
    {
        printf("\n Sin respuesta \n");
    }
    printf("%s",recMsg);
    snprintf(sendMsg,sizeof(sendMsg), "%s\r\n","021000");
    write(connFd, sendMsg, strlen(sendMsg));
    printf("\nSolicitud procesada\n");
    close(connFd);
    sleep(1);
  }
  return 0;
}
