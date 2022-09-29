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
#include "../sqlite/sqlite3.h"
#include "../headers/sqlitedb.h"

#define BACKLOG 10
#define MAXLINE 1024

/*types of objects needed*/
typedef struct messanger{  
    char name[10];
    char msg[MAXLINE];
}messanger;

typedef struct my_msg{  
        long int msg_type;  
        struct sockaddr_in client_address;
        int msgqid; 
        messanger client;
        sqlite3 *dbconn;
 }my_msg; 

typedef struct pthread_arg_t {
    int new_socket_fd;
    struct sockaddr_in client_address;
    int msgqid;    
    sqlite3 *dbconn;
} pthread_arg_t;



/* Thread routine to serve connection to client. */
void *pthread_routine(void *arg);

/* Thread routine to send message to msgque. */
void *pthread_msg_receive(void *arg);

/* Signal handler to handle SIGTERM and SIGINT signals. */
void signal_handler(int signal_number);

/*for sending msg to msgque*/
int msgSend(my_msg mymsg);

/*for recieveing msg from msgque*/
int msgReceive(int msgqid, sqlite3 *db);

/* Function to recieve the data from one client and send the data to msgque, other clients & database. */
void func(int connfd, struct sockaddr_in client_addr, int msgqid, sqlite3 *db);

/* for sending the msg to other client. */
void sendToAll(messanger* sendingmsg, int curr);

/* for utilizing globally. */
int port;
int clients[100];
int n = 0;

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

    /* For relesing the bind to the socket instantly after closing. */
    const int enable = 1;
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

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

     /* INitializing Use of database. */
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc, i;

    rc = sqlite3_open("/home/madhab/Desktop/vscode_makefile/multithreaded-bot/src/database/chatbot.db", &db);

    if( rc ) {
      fprintf(stderr, "can't open database:%s\n", sqlite3_errmsg(db));
      return(0);
    } else {
        create(db);   
      fprintf(stderr, "Opened database successfully\n");
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

     /*Initialization of message queue to handle the messages . */
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
		
         /* entering connecting clients to the array. */
		clients[n] = new_socket_fd;
		n++;

        /* Initialise pthread and other arguments. */
        pthread_arg->new_socket_fd = new_socket_fd;

        pthread_arg->msgqid = msgqid;
        msg_to_receive->msgqid = msgqid;

        pthread_arg->dbconn = db;
        msg_to_receive->dbconn = db;
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
    sqlite3 *dbConn = pthread_arg->dbconn;
    /* TODO: Get arguments passed to threads here. */

    free(arg);

    /* TODO: Put client interaction code here.*/
    func(new_socket_fd, client_address, msgqid, dbConn);

    close(new_socket_fd);
    return NULL;
}

void *pthread_msg_receive(void *arg) {

    my_msg *mymsg = (my_msg *)arg;
    int msgqid = mymsg->msgqid;
    struct sockaddr_in client_addr = mymsg->client_address;
    sqlite3 *db = mymsg ->dbconn;
    /* TODO: Get arguments passed to threads here. */
    free(arg);

    /* TODO: Put client interaction code here.*/
    msgReceive(msgqid, db);
    return NULL;
}

void signal_handler(int signal_number) {
    /* TODO: Put exit cleanup code here. */
    exit(0);
}

void func(int connfd, struct sockaddr_in client_addr, int msgqid, sqlite3 *db) {

    messanger newclient;
    int j, i;
    int running = 1;
    struct my_msg mymsg;
    memset(&mymsg, 0, sizeof(mymsg));
    int receivedConn;

    
    while(running) {

        memset(&newclient, 0, sizeof(newclient));
        // read the message from client and copy it in buffer
        receivedConn = recv(connfd, &newclient,MAXLINE, 0);
        port = client_addr.sin_port;
        char id[15];
        sprintf(id, "%d", port);

         /* Handling the exits. */
       if(strcmp(newclient.msg, "exit\n") == 0){
            printf("Disconnected from with:%d\n", client_addr.sin_port);
            for (i = 0; i < n; i++) {
                if (clients[i] == connfd) {
                    j = i;
                
                    while (j < (n - 1)) {
                        clients[j] = clients[j + 1];
                        j++;
                    }
                }
            }
            n--;

            delete(db, id);
            close(connfd);
            running = 0;
            break;
        }else{
            mymsg.msg_type=1;
            mymsg.msgqid = msgqid;
            memcpy(&mymsg.client, &newclient, sizeof newclient);

         /* Main activities like sendinng to msgque, other clients and database. */
            if(receivedConn != 0){
                sendToAll(&newclient, connfd);
                msgSend(mymsg);
                insert(db, id, newclient.name, newclient.msg);
            }
             else{
                 /* handling ctrl + c. */
                printf("connection closed with the client:%d\n",port);
                for (i = 0; i < n; i++) {
                    if (clients[i] == connfd) {
                        j = i;
                    
                        while (j < (n - 1)) {
                            clients[j] = clients[j + 1];
                            j++;
                        }
                    }
                }
                n--;
            
                delete(db, id);
                memset(&newclient, 0, sizeof(newclient));
                running = 0;
            }
                        
            memset(&newclient, 0, sizeof(newclient));
        }
   
    }
}

int msgReceive(int msgid, sqlite3 *db) {

    int running=1;
    long int msg_to_rec=0;
    struct my_msg my_recieved_msg;
    struct messanger recvclient;

     /* For recieving all the clients' sent msgs. */
    while(running){
        if(msgrcv(msgid,(void *)&my_recieved_msg,MAXLINE,msg_to_rec,0) == -1){
                printf("Msg not received\n"); 
                running = 0;
        }
        else{
            recvclient = my_recieved_msg.client;
            printf("%s: %s\n",recvclient.name, recvclient.msg);
            if(strcmp(recvclient.msg, "exit\n") == 0){
                printf("Disconnected from %d\n",port);
                //delete(db, port);
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

     /* For sending messages to tht queue. */
    if(msgsnd(my_sent_msg.msgqid,(void *)&my_sent_msg, MAXLINE,0)==-1)
    {
            printf("Msg not sent\n");
    }
    
}

void sendToAll(messanger* sendingmsg, int curr) {

	int i;
	printf("sending: %s\n", sendingmsg->msg);
    char buff[MAXLINE];
    sprintf(buff, "%s : %s", sendingmsg->name, sendingmsg->msg);
     /* for sending all the msgs to the other clients. */
	for (i = 0; i < n; i++) 
	{
		if (clients[i] != curr) 
		{
			if (send(clients[i], buff, MAXLINE, 0) < 0) 
			{
				perror("sending failure...");
				continue;
			}
            printf("sending successful to %d\n", clients[i]);
		}
	}

}