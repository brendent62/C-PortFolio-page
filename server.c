#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define port 25565
#include <arpa/inet.h>
#include <string.h>



int main(){
    FILE *f;
    f = fopen("Frontend.html", "r");
    if(!f){
        perror("error opening file");
        return 1;
    }
    char Message[256] = "You have connceted";
    int s;
    s = socket(AF_INET,SOCK_STREAM,0);

    if(s < 0){
        perror("Socket creation failed");
        return 1;
    }
    
    struct sockaddr_in servAdd;
    servAdd.sin_family = AF_INET;
    servAdd.sin_port = htons(port);
    servAdd.sin_addr.s_addr = inet_addr("127.0.0.1");
    struct sockaddr_in ClientInfo;
    socklen_t cli_len = sizeof(ClientInfo);
    char page[2000000]= "HHTP";

    int e1= bind(s, (struct sockaddr *)&servAdd, sizeof(servAdd));
    if(e1<0){
        perror("bind failed");
        return 1;
    }

    listen(s,50);
    printf("Server listening on http://127.0.0.1:%d\n", port);
    int c;


    fseek(f,0,SEEK_END);
    long fsize = ftell(f);
    fseek(f,0,SEEK_SET);
    char *html = malloc(fsize + 1);
    fread(html,1,fsize,f);
    html[fsize] = 0;
    char header[512];
    if (!html) {
        perror("Malloc failed");
        close(c);
        close(s);
        fclose(f);
        return 1;
    }


    sprintf(header,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n"
            "\r\n",
            fsize);
    while(1){
        c=accept(s,(struct sockaddr *)&ClientInfo, &cli_len);
        if (c < 0) {
            perror("Accept failed");
            close(s);
            fclose(f);
            return 1;
        }

        char request[4096];
        int bytes_received = recv(c, request, sizeof(request) - 1, 0);
        if (bytes_received > 0) {
            request[bytes_received] = '\0';
            printf("Request received:\n%s\n", request);
        }

        send(c, header, strlen(header), 0);
        send(c,html, fsize, 0);

        }
    free(html);
    close(s);
    return 0;
}