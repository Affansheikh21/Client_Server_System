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


int n =0;
//char buff[size];




void work(int);


void Add();

void Sub();
void Mul();
void Div();
void Run();
void list();
void Kill();


void sighandler(int signo) {
    if (signo == SIGINT) {
    write(STDOUT_FILENO, "\nYou pressed CTRL-C!\n", sizeof("\nYou pressed CTRL-C!\n"));
}
       if (signo == SIGCHLD) {
    write(STDOUT_FILENO, "\nChild is terminated\n", sizeof("\nChild is terminated\n"));
}

}






int main(int argc, char * argv[]){

    int sockfd, newsockfd, portno, clilen, pid;
    struct sockaddr_in serv_addr, cli_addr;

    if(argc <2) {
    write(STDOUT_FILENO,"Provide Port number. It can be any number between 2000 and 65535.\n",sizeof("Provide Port number. It can be any number between 2000 and 65535.\n"));
    exit(1);
    }
    // call socket api, for tcp ip connection
    sockfd = socket(AF_INET,SOCK_STREAM,0);
        if (sockfd < 0) {
            perror("Error at Socket Api ");
            exit(1);
        }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    // populate structure
    serv_addr.sin_family = AF_INET; //tcp/ip connection
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    // bind Api
    int bindcheck = bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (bindcheck < 0) {
            perror("Error at Bind Api ");
            exit(1);
        }

    //listen to connections. It only fails if socket fails, so no error check
    listen(sockfd,5); 
    clilen = sizeof(cli_addr);

    // real server code, handling multiple clients
    while (1) {
        newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr, &clilen);
            if (newsockfd < 0) {
                perror("Error at Accepting Connection ");
                exit(1);
            }
            // accomodating multiple connections
            
            pid = fork();
                if (pid<0){
                    perror("Error at Fork Api ");
                    exit(1);
                }

                if (pid == 0) {
                    close(sockfd);
                    // call main function
                    work(newsockfd);
                    
                }
                else {
                    close(newsockfd);
                }
    
    
    
    }
    return 0;


    }






void work(int sock){
    char buff[size];
    char displaybuffer[size];
    

            while(1) {
                bzero(buff,size);
                n = read(sock,buff,size);
        if (n < 0){
            perror("Error at Reading Data ");
            exit(1);
        }
                            // echo client's input on server's screen
                           // n = sprintf(displaybuffer, "The client entered: %s\n", buff);
                            write(STDOUT_FILENO,buff,strlen(buff) +1 );
                            
            
            char string[n - 1];
            sscanf(buff, "%[^\n]", string);

            token = strtok(string, delimiter);
            
            if (strcmp(token, "add") == 0) {
               // Add();
                            int sum = 0;
                        while ((token = strtok(NULL, delimiter)) != NULL) 
                            {
                                sum = sum + atoi(token);
                            }
                                n = sprintf(buff, "The answer is: %.2d \n", (int)sum);
                                write(sock,buff,n);
                            }
            
             else if (strcmp(token, "sub") == 0) {
              // Sub();
                            int result;
                            token = strtok(NULL, delimiter);
                            result = atoi(token);
                        while ((token = strtok(NULL, delimiter)) != NULL) 
                        {
                            result-= atoi(token);

                            }
                                n = sprintf(buff, "The answer is: %.2d \n", (int)result);
                                write(sock,buff, n);
            } 
            
            else if (strcmp(token, "mul") == 0) {
                //Mul();
            } else if (strcmp(token, "div") == 0) {
                //Div();
            
                                float result=1;
                                token = strtok(NULL, delimiter);
                                result = atof(token);
                            
                        while ((token = strtok(NULL, delimiter)) != NULL) 
                            {
                                if(strcmp(token,"0") ==0) {
                                    write(STDOUT_FILENO," Div by 0", sizeof(" Div by 0"));
                                exit(0);
                                }
                                result/= atof(token);

                            }
                                n = sprintf(buff, "The answer is: %.2f \n", (float)result);
                                write(sock,buff, n);
                            }


             else if (strcmp(token, "run") == 0) {
                //Run();
            } else if (strcmp(token, "list") == 0) {
                //list();
            } else if (strcmp(token, "kill") == 0) {
                // Kill();
            } else if (strcmp(token, "exit") == 0) {
                
                n = sprintf(buff, "Exit from program\n");
                write(sock, buff, n);
                exit(0);
            } 
            
            else {
                n = sprintf(buff, "Invalid Command \n");
                write(sock, buff, n);
            } 

}
}

/*
    void Mul(){
         int result =1;
         token = strtok(NULL, delimiter);
         
           while ((token = strtok(NULL, delimiter)) != NULL) 
            {
                result*= atoi(token);

            }

            	n = sprintf(printbuffer, "The answer is: %.2d \n", result);
	write(sock, printbuffer, n);
    }
*/
