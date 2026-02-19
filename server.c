    #include <stdlib.h>
    #include <stdio.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #define port 8000
    #include <arpa/inet.h>
    #include <string.h>

    void file_open(int c, const char *FilePath){
        FILE *f;
        if (FilePath[0] == '/') FilePath++;
        char fpath[256];
        
        sprintf(fpath, "./pages/%s" ,FilePath);
        f = fopen(fpath, "rb");
        printf("Trying to open: '%s' (len=%zu)\n", fpath, strlen(fpath));
        if(!f){
            perror("error opening file");
            return;
        }
        fseek(f,0,SEEK_END);
        long fsize = ftell(f);
        fseek(f,0,SEEK_SET);

        char *html = malloc(fsize + 1);
        fread(html,1,fsize,f);
        html[fsize] = 0;
        fclose(f);
        
        const char *ctype = "text/plain";
        if (strstr(fpath, ".css")) ctype = "text/css";
        else if (strstr(fpath, ".html")) ctype = "text/html";

        char header[512];


        sprintf(header,
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: %s\r\n"
            "Content-Length: %ld\r\n"
            "Connection: close\r\n\r\n",
            ctype, fsize);

        send(c, header, strlen(header), 0);
        send(c, html, fsize, 0);
        free(html);
                
 

    }


    int main(){
        int s;

        
        s = socket(AF_INET,SOCK_STREAM,0);
        if(s < 0){
            perror("Socket creation failed");
            return 1;

        

        }


        struct sockaddr_in servAdd;
        servAdd.sin_family = AF_INET;
        servAdd.sin_port = htons(port);
        servAdd.sin_addr.s_addr = INADDR_ANY;
        struct sockaddr_in ClientInfo;
        socklen_t cli_len = sizeof(ClientInfo);
        

        int e1= bind(s, (struct sockaddr *)&servAdd, sizeof(servAdd));
        if(e1<0){
            perror("bind failed");
            return 1;
        }

        listen(s,50);
        printf("Server listening on http://127.0.0.1:%d\n", port);
        int c;


        

        while(1){
            c = accept(s,(struct sockaddr *)&ClientInfo, &cli_len);
            if (c < 0) {
                perror("Accept failed");
                close(s);
                return 1;
            }

            char request[4096];
            int bytes_received = recv(c, request, sizeof(request) - 1, 0);
            if (bytes_received > 0) {
                request[bytes_received] = '\0';
                printf("Request received:\n%s\n", request);
                char method[10];
                char path[1024];
                sscanf(request,"%s %s",method,path);

                printf("Method %s\n", method);
                printf("path %s\n", path);
                if (strcmp(path, "/") == 0) {
                    file_open(c, "index.html");
                    }
                else if (strcmp(path, "/projects")==0){
                    file_open(c, "projects.html");
                }
                else if (strcmp(path, "/resume") ==0){
                    file_open(c, "resume.html");
                }
                else if (strcmp(path, "/about")==0){
                    file_open(c, "about.html");
                }
                else if (strcmp(path, "/contact") ==0){
                    file_open(c, "contact.html");
                }
                else if(strcmp(path, "/css/style.css")==0){
                    file_open(c, "css/style.css");
                }

            }
 
    

            }


        close(s);
        return 0;
    }
