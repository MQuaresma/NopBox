#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include "server.h"

void print_usage(char *bin_name){
    printf("NopBox Server\n" \
           "Usage: %s [ arguments ]\n" \
           "-h                print this menu \n" \
           "--target <dir>    target directory \n\n", bin_name);
}


/*
 * Parses the command line arguments passed to client
 */
int parse_args(int argc, char *argv[]){
    //TODO: extract the destination IP and port

    for(int i = 1; i < argc; i ++)
        if(!strncmp(argv[i], "-h", 2)){
            print_usage(*argv);
            return 1;
        }

    return 0;
}


inline int response(int fd, struct sockaddr_in *client, socklen_t *sock_len){
    char ping[10] = {0};
    const char pong[] = "pong";
    size_t len;
    int cli_sock;

    cli_sock = accept(fd, (struct sockaddr *)&client, sock_len);
    
    if(cli_sock > 0){
        len = recv(cli_sock, ping, 9, 0);
        send(cli_sock, pong, sizeof(pong), 0);
        if(len > 0)
            ping[len] = 0;
        printf("%s\n", ping);
        close(cli_sock);
        return 0;
    } else return 1;
    
}


int main(int argc, char *argv[]){
    int fd;
    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t sock_len = sizeof(struct sockaddr_in);

    if(parse_args(argc, argv))
        return 0;

    server.sin_family = PF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd > 0){
        if(!bind(fd, (struct sockaddr *)&server, sizeof(server))){
            listen(fd, 1);
            response(fd, &client, &sock_len);
        }else{
            return 1;
        }

        close(fd);
    }
}
