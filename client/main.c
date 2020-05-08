#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include "client.h"

void print_usage(char *bin_name){
    printf("NopBox Client\n" \
           "Usage: %s [ arguments ]\n" \
           "-h                print this menu \n" \
           "--source <dir>    source directory \n" \
           "-t <ip>           target IP \n\n", bin_name);
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


int main(int argc, char *argv[]){
    int fd;
    const char ping[] = "ping";
    char pong[10] = {0};
    size_t len;
    struct sockaddr_in server;

    if(parse_args(argc, argv))
        return 0;

    //TODO: change according to destination IP
    fd = socket(PF_INET, SOCK_STREAM, 0);

    server.sin_family = PF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    connect(fd, (struct sockaddr *)&server, sizeof(server));

    if(send(fd, ping, sizeof(ping), 0) > 0){
        len = recv(fd, pong, 9, 0);
        if(len > 0)
            pong[len] = 0;
        printf("%s\n", pong);
    }
}
