#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>  
#include <sys/msg.h>

#define BACKLOG 10
#define MAXLINE 1024


typedef struct my_msg{  
        long int msg_type;  
        char some_text[MAXLINE];
        struct sockaddr_in client_address;
        int msgqid; 
 }my_msg; 

typedef struct pthread_arg_t {
    int new_socket_fd;
    struct sockaddr_in client_address;
    int msgqid;    
} pthread_arg_t;



/* Thread routine to serve connection to client. */
void *pthread_routine(void *arg);

void *pthread_msg_receive(void *arg);

/* Signal handler to handle SIGTERM and SIGINT signals. */
void signal_handler(int signal_number);

/*for sending msg to msgque*/
int msgSend(my_msg mymsg);

/*for recieveing msg from msgque*/
int msgReceive(int msgqid);

void func(int connfd, struct sockaddr_in client_addr, int msgqid);

int port;

int main(int argc, char *argv[]) {
    int port, socket_fd, new_socket_fd;
    struct sockaddr_in address;
    pthread_attr_t pthread_attr;
    pthread_arg_t *pthread_arg;
    pthread_t pthread;
    socklen_t client_address_len;
    char buffer[MAXLINE];
    my_msg *msg_to_receive;
    long int msg_receive = 0;


    /* Get port from command line arguments or stdin. */
    port = argc > 1 ? atoi(argv[1]) : 0;
    if (!port) {
        printf("Enter Port: ");
        scanf("%d", &port);
    }

    /* Initialise IPv4 address. */
    memset(&address, 0, sizeof address);
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

     /* Create TCP socket. */
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    /* Bind address to socket. */
    if (bind(socket_fd, (struct sockaddr *)&address, sizeof address) == -1) {
        perror("bind");
        exit(1);
    }

    /* Listen on socket. */
    if (listen(socket_fd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    /* Assign signal handlers to signals. */
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("signal");
        exit(1);
    }


    int msgqid;
    msgqid=msgget((key_t)12345,0666|IPC_CREAT);
    if (msgqid == -1)
    {
        printf("Error in creating queue\n");
        exit(0);
    }
    /* Initialise pthread attribute to create detached threads. */
    if (pthread_attr_init(&pthread_attr) != 0) {
        perror("pthread_attr_init");
        exit(1);
    }
    if (pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED) != 0) {
        perror("pthread_attr_setdetachstate");
        exit(1);
    }

    while (1) {
        /* Create pthread argument for each connection to client. */
        /* TODO: malloc'ing before accepting a connection causes only one small
         * memory when the program exits. It can be safely ignored.
         */
        pthread_arg = (pthread_arg_t *)malloc(sizeof *pthread_arg);
        if (!pthread_arg) {
            perror("malloc");
            continue;
        }

        msg_to_receive = (my_msg *)malloc(sizeof *msg_to_receive);
         if (!msg_to_receive) {
            perror("malloc");
            continue;
        }

        /* Accept connection to client. */
        client_address_len = sizeof pthread_arg->client_address;
        new_socket_fd = accept(socket_fd, (struct sockaddr *)&pthread_arg->client_address, &client_address_len);
        if (new_socket_fd == -1) {
            perror("accept");
            free(pthread_arg);
            continue;
        }       

        /* Initialise pthread argument. */
        pthread_arg->new_socket_fd = new_socket_fd;
        pthread_arg->msgqid = msgqid;
        msg_to_receive->msgqid = msgqid;
        /* TODO: Initialise arguments passed to threads here.*/

        /* Create thread to serve connection to client. */
        if (pthread_create(&pthread, &pthread_attr, pthread_routine, (void *)pthread_arg) != 0) {
            perror("pthread_create failed");
            free(pthread_arg);
            continue;
        }

        if (pthread_create(&pthread, &pthread_attr, pthread_msg_receive, (void *)msg_to_receive) != 0) {
            perror("pthread_create failed");
            free(msg_to_receive);
            continue;
        }
        
    }
    return 0;
}





void *pthread_routine(void *arg) {
    pthread_arg_t *pthread_arg = (pthread_arg_t *)arg;
    int new_socket_fd = pthread_arg->new_socket_fd;
    struct sockaddr_in client_address = pthread_arg->client_address;
    int msgqid = pthread_arg->msgqid;
    /* TODO: Get arguments passed to threads here. */

    free(arg);

    /* TODO: Put client interaction code here.*/
    func(new_socket_fd, client_address, msgqid);

    close(new_socket_fd);
    return NULL;
}

void *pthread_msg_receive(void *arg) {
    my_msg *mymsg = (my_msg *)arg;
    int msgqid = mymsg->msgqid;
    struct sockaddr_in client_addr = mymsg->client_address;
    /* TODO: Get arguments passed to threads here. */
    free(arg);

    /* TODO: Put client interaction code here.*/
    msgReceive(msgqid);
    return NULL;
}

void signal_handler(int signal_number) {
    /* TODO: Put exit cleanup code here. */
    //close(new_socket_fd);
    exit(0);
}

void func(int connfd, struct sockaddr_in client_addr, int msgqid) {
    char buff[MAXLINE];
    int n;
    int running = 1;
    struct my_msg mymsg;
    memset(&mymsg, 0, sizeof(mymsg));
    int receivedConn;

    
    while(running) {
        bzero(buff, MAXLINE);
   
        // read the message from client and copy it in buffer
        //read(connfd, buff, sizeof(buff));
        receivedConn = recv(connfd, &buff,MAXLINE, 0);
        if(strcmp(buff, "exit\n") == 0){
            printf("Disconnected from with:%d\n", client_addr.sin_port);
            close(connfd);
            running = 0;
            break;
        }else{
            mymsg.msg_type=1;
            mymsg.msgqid = msgqid;
            strcpy(mymsg.some_text,buff);
            port = client_addr.sin_port;
            if(receivedConn != 0){
                msgSend(mymsg);
            }
            else{
                printf("connection closed with the client:%d\n",port);
                running = 0;
            }
            bzero(buff, sizeof(buff));
        }

        //printf("%d:%s",client_addr.sin_port, buff);
        //bzero(buff, MAXLINE);        
        
        // copy server message in the buffer
        //while ((buff[n++] = getchar()) != '\n');
   
        // and send that buffer to client
        //printf("Message : ");
        //write(connfd, buff, sizeof(buff));
   
    }
}

int msgReceive(int msgid) {

    int running=1;
    long int msg_to_rec=0;
    struct my_msg my_recieved_msg;
    while(running){
        if(msgrcv(msgid,(void *)&my_recieved_msg,MAXLINE,msg_to_rec,0) == -1){
                printf("Msg not received\n"); 
                running = 0;
        }
        else{
            printf("%d: %s\n",port, my_recieved_msg.some_text);
            if(strcmp(my_recieved_msg.some_text, "exit\n") == 0){
                printf("Disconnected from %d\n",port);
                port  = 0;
                running = 0;
                break;
            }
            port  = 0;
        }
    }
    msgctl(msgid,IPC_RMID,0);
}

int msgSend(struct my_msg my_sent_msg) {

    if(msgsnd(my_sent_msg.msgqid,(void *)&my_sent_msg, MAXLINE,0)==-1)
    {
            printf("Msg not sent\n");
    }
    // else{
    //         printf("Msg sent\n");
    // }
    
}