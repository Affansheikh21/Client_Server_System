#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <arpa/inet.h>




const int size = 1000;
char delimiter[1] = {' '};
char * token;
//char printbuffer[size];
//int number;
//char buff[size];


typedef struct my_process {
	int pid;
	char * name;
	bool isActive;
	bool isValid;
	time_t startingTime;
	time_t endingTime;

} my_process;

#define MAX_LIMIT 100
int ctr = 0;
// Create an array of processes
my_process process_arr[MAX_LIMIT];













int main(int argc, char * argv[]){

    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent * server;
    char buff[size];
    // char buffer[256];
        if(argc<3) {
        write(STDOUT_FILENO,"Executable, hostname port\n",sizeof("Executable, hostname port\n"));
        exit(1);
        }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET,SOCK_STREAM,0);
        if (sockfd < 0) {
            perror("Error at Socket Api ");
            exit(1);
        }

    server = gethostbyname(argv[1]);
        if (server == NULL) {
        write(STDOUT_FILENO,"Cannot resolve hostname.\n",sizeof("Cannot resolve hostname.\n"));
        exit(1);
        }

    bzero((char *) &serv_addr,sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);

    serv_addr.sin_port = htons(portno);

    // connect
    int con = connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (con <0 ) {
            perror("Error at Connect Api ");
            exit(1);  
        }


        //
            int n=0;

                while(1) {
                    bzero(buff,size);
        n = write(STDOUT_FILENO,"Enter Command: ", sizeof("Enter Command: "));
                            if (n<0) {
                            perror("Error at Write Api ");
                            exit(1);
                            }
        n = read(STDIN_FILENO,buff,size);
                            if (n<0) {
                            perror("Error at Read Api ");
                            exit(1);   
                            }

                            // if n == 0 condition unimplemented 


        n = write(sockfd,buff,strlen(buff)+1);      
                            if (n<0) {
                            perror("Error at Sending Data ");
                            exit(1);   
                            }

                    bzero(buff,size);
        n = read(sockfd,buff,size);
                            if (n<0) {
                            perror("Error at Receiving Data");
                            exit(1);   
                            }
        write(STDOUT_FILENO,buff,strlen(buff) + 1);
        
                } // while terminates
        return 0;













































}
