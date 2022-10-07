#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#include <netinet/in.h>
#include <signal.h>
#include <sys/ipc.h>  
#include <sys/msg.h>

#define SERVER_NAME_LEN_MAX 255
#define MAXLINE 1024

typedef struct messanger{  
    char name[10];
    char msg[MAXLINE];
}messanger;

void* recvMsg(void* arg);

void func(int socket_fd, struct sockaddr_in server_addr);

int main(int argc, char *argv[]) {
    char server_name[SERVER_NAME_LEN_MAX + 1] = { 0 };
    int server_port, socket_fd;
    struct hostent *server_host;
    struct sockaddr_in server_address;
    pthread_attr_t pthread_attr;
    messanger *msg_to_receive;
    pthread_t recvt;
    char buffer[MAXLINE];


    /* Get server name from command line arguments or stdin. */
    if (argc > 1) {
        strncpy(server_name, argv[1], SERVER_NAME_LEN_MAX);
    } else {
        printf("Enter Server Name: ");
        scanf("%s", server_name);
    }

    /* Get server port from command line arguments or stdin. */
    server_port = argc > 2 ? atoi(argv[2]) : 0;
    if (!server_port) {
        printf("Enter Port: ");
        scanf("%d", &server_port);
    }

    /* Get server host from server name. */
    server_host = gethostbyname(server_name);

    /* Initialise IPv4 server address with server host. */
    memset(&server_address, 0, sizeof server_address);
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server_port);
    memcpy(&server_address.sin_addr.s_addr, server_host->h_addr_list[0], server_host->h_length);

    /* Create TCP socket. */
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* Connect to socket with server address. */
    if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof server_address) == -1) {
		perror("connect");
        exit(1);
	}

    if (pthread_attr_init(&pthread_attr) != 0) {
        perror("pthread_attr_init");
        exit(1);
    }
    if (pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED) != 0) {
        perror("pthread_attr_setdetachstate");
        exit(1);
    }

    /* TODO: Put server interaction code here. */
    if (pthread_create(&recvt, &pthread_attr, &recvMsg, &socket_fd) != 0) {
            perror("pthread_create failed");
        }

	func(socket_fd,server_address);

    //pthread_join(recvt, NULL);
    close(socket_fd);
    return 0;
}

void func(int sockfd, struct sockaddr_in server_addr) {

    messanger *client;
    client = (messanger *)malloc(sizeof (messanger));
    char cname[10];
    int len;
    char cmsg[MAXLINE];
    printf("Enter Name:");
    fgets(cname, sizeof cname, stdin);
    len = strlen(cname);
        if( cname[len-1] == '\n' )
            cname[len-1] = 0;
    strcpy(client->name, cname);
    char buff[MAXLINE];
    int n;
    for (;;) {

        bzero(buff, sizeof(buff));
        //printf("%s : ", cname);
        n = 0;
        fgets(buff, sizeof buff, stdin);
        //strcpy(client->msg, buff);
        memcpy(client->msg, buff, sizeof(messanger));

        write(sockfd, client, sizeof(buff));
        bzero(buff, sizeof(buff));
        if ((strncmp(buff, "exit", 4)) == 0) {
            printf("Client Exit...\n");
            break;
        }
    }
}

void *recvMsg(void *server) {

    int fd = *(int *)server;
    messanger recvclient;
    char buff[MAXLINE];
    int res;

    while(1) {
        if(res = recv(fd, &buff, MAXLINE, 0) > 0)
            printf("\n%s\n", buff);
        else {
            if(res == -1) {
            printf("recv failed!\n");
            continue;
            }
            if(res == 0) {
                printf("Server closed!\n");
                break;
            }
        }
        bzero(buff, sizeof(buff));
        memset(&recvclient, 0, sizeof (recvclient));
    }
}
  
