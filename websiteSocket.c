#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define port 22565
#include <arpa/inet.h>


int main(){
    int s;
    s = socket(AF_INET, SOCK_STREAM,0);

    struct sockaddr_in Address;
    Address.sin_family = AF_INET;
    Address.sin_port = htons(port);
    Address.sin_addr.s_addr = inet_addr("127.0.0.1");


    int connection = connect(s, (struct sockaddr *) &Address, sizeof(Address));
    if(connection == -1){
        printf("CONNECTION ERROR");
    }
    char respose[256];
    recv(s, &respose, sizeof(respose),0);


    printf("%s\n",respose);
    close(s);
    return 0;


}