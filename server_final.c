#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>              
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>
#include <error.h>
#include <errno.h>
#define size 5000
#define client_size 5








// client list
typedef struct client{
    int pid;    // each new connection invokes a new process(child/fork)
    char ip_address[100]; // client's IP Address
    int write_pipe[2];  // process listing communication
    int read_pipe[2];   // process listing communication
    int sockfd; // socket assigned for communication
    int c_port; // client port

} clients;

clients client_list[client_size]; // client_list initialized
int client_counter; //  maintain the current of clients connected 

// process list... It will be implemented as a linked list  
typedef struct process{
    char name_process[200];
    pid_t pid;
    char status[100];
    char start_time[9];
    char end_time[9];
    struct process * next_node;
} node;

node * head_main;

// Time Struct
struct time {
	int hour, minute, second;
};
// Function prototype
void Time_Diff(struct time t1, struct time t2, struct time *t3);
void Kill_node(node * head, int pid, char * sig);

char delimiter[1] = {' '};

// Intelligent Server - Server Commands will be handled in a seperate thread
// Thread  1  - Server Commands

void * Intelligent_Server(void * arg){

    while(1) {
        char display_message[] = "Server Commands Portal\n Enter Commands: \n 1-clients (list of all clients)\n 2-client [ip] (list specific client)\n 3-lists (list of all processes)\n";        
        int ret=0;
        ret = write(STDOUT_FILENO,display_message,strlen(display_message));
                    if (ret<0){ perror("Can't print display message");}

        char input_buff[100];            
        ret = read(STDIN_FILENO,input_buff,100);    
                    if (ret<0){ perror("Can't read Input");}
            input_buff[ret-1] = '\0';
            char *input_list[10];  // this list will store the tokens from the input entered by user
            char * token = NULL;
            token = strtok(input_buff,delimiter);
            int count_token=0;
            while(token!=NULL) {
                input_list[count_token] =token;
                count_token++;
                token = strtok(NULL,delimiter);
            }
            
            // INPUT MANIPULATION
            if (strcmp(input_list[0],"clients")==0) {
                    for(int i=0;i<client_counter;i++) {
                        char pid_print[100], ip_print[100], port_print[100];
                        int l,m,n;
                        write(STDOUT_FILENO,"Client's PID:\n",15);
                        l= sprintf(pid_print,"%d",client_list[i].pid);
                        write(STDOUT_FILENO,pid_print,l);
                        
                        write(STDOUT_FILENO,"Client's IP:\n",15);
                        strcpy(ip_print,client_list[i].ip_address);
                        write(STDOUT_FILENO,ip_print,strlen(ip_print));

                        write(STDOUT_FILENO,"Client's Port:\n",16);
                        n= sprintf(port_print,"%d",client_list[i].c_port);
                        write(STDOUT_FILENO,port_print,n);
                    }


            }

            
            
            
            
            
            
            
            if (strcmp(input_list[0],"client")==0) {
                // check next token for IP Address
                // store the value in arbitrary variable and check through the whole client list. 
                // when found, return the details of that client and exit the loop

                    for (int j=0;j<client_counter;j++){
                        if(client_list[j].pid == atoi(input_list[j])){
                                        char pid_print[100], ip_print[100], port_print[100];
                        int l,m,n;
                        write(STDOUT_FILENO,"Client's PID:\n",15);
                        l= sprintf(pid_print,"%d",client_list[j].pid);
                        write(STDOUT_FILENO,pid_print,l);
                        
                        write(STDOUT_FILENO,"Client's IP:\n",15);
                        strcpy(ip_print,client_list[j].ip_address);
                        write(STDOUT_FILENO,ip_print,strlen(ip_print));

                        write(STDOUT_FILENO,"Client's Port:\n",16);
                        n= sprintf(port_print,"%d",client_list[j].c_port);
                        write(STDOUT_FILENO,port_print,n);                                        }
                    }
            }





            if (strcmp(input_list[0],"lists")==0) {
                    // list all the processes 
                    // for each client, pipes will be used to bring the list from the child process
                    // each single iteration will print the list for one client, hence accumulating the list at the end of the loop
                    
                    /* there are two pipes for communication between client's parent and child process
                        via write_pipe[1] write list command to the client's parent process
                        and extract the list via read_pipe[1]
                        --- close write-pipe[0] (reading end for write)
                        --- close read-pipe[1] (reading end for read)
                            */
                    char list[size];
                    for(int k=0;k<client_counter;k++){
                        close(client_list[k].write_pipe[0]);
                        close(client_list[k].read_pipe[1]);
                        int check;
                        check = write(client_list[k].write_pipe[1],"list",4);
                            if (check ==-1) {perror("Cannot write to pipe");}
                        char buffer[size];
                        check = read(client_list[k].read_pipe[0],buffer,size);
                            if (check ==-1) {perror("Cannot read from pipe");}
                    
                        buffer[check] = '\0'; // string functions work on NULL terminated Strings
                        strcat(list,buffer); // join the extracted list to the master list 

                    }
                    int x=0;
                    x = write(STDOUT_FILENO,list,strlen(list)+1);
                    if (x ==-1) {perror("Error at Printing the Entire List");}

            }
        }
    }
            // print the Entire Linked List
char * print_list(node *head){
    
    char *buff=malloc(2000);
    
	strcpy(buff, "\nName\t\tPID\t\tStatus\t\tStart Time\t\tEnd Time\t\tElapsed Time\n");
	node *current=head;
	
    while (current!=NULL) {
		strcat(buff, current->name_process);
		strcat(buff, "\t"); strcat(buff, "\t");

		char id[20];
		int count=sprintf(id, "%d", current->pid);
		strcat(buff, id);
		strcat(buff, "\t"); strcat(buff, "\t");
		strcat(buff, current->status);
		strcat(buff, "\t"); strcat(buff, "\t");
		strcat(buff, current->start_time);
		strcat(buff, "\t"); strcat(buff, "\t");
		strcat(buff, current->end_time);
		
		if (strcmp(current->end_time, " ")!=0) {
			struct time t1,t2,t3;
			//populate t1 with start time values
			char t1_hr[2];
			memcpy(t1_hr, &current->start_time[0], 2);
			t1_hr[2]='\0';
			t1.hour=atoi(t1_hr);
			char t1_m[2];
			memcpy(t1_m, &current->start_time[3], 2);
			t1_m[2]='\0';
			t1.minute=atoi(t1_m);
			char t1_s[2];
			memcpy(t1_s, &current->start_time[6], 2);
			t1_s[2]='\0';
			t1.second=atoi(t1_s);
			//populate t2 with end time values
			char t2_hr[2];
			memcpy(t2_hr, &current->end_time[0], 2);
			t2_hr[2]='\0';
			t2.hour=atoi(t2_hr);
			char t2_m[2];
			memcpy(t2_m, &current->end_time[3], 2);
			t2_m[2]='\0';
			t2.minute=atoi(t2_m);
			char t2_s[2];
			memcpy(t2_s, &current->end_time[6], 2);
			t2_s[2]='\0';
			t2.second=atoi(t2_s);
			//Calculate Difference


			Time_Diff(t2, t1, &t3);
			strcat(buff, "\t");
			strcat(buff, "\t");
			char time_diff[50];
			int time_count=sprintf(time_diff, "%02d:%02d:%02d", t3.hour, t3.minute, t3.second);
			strcat(buff, time_diff);
		}
		strcat(buff, "\n");
		current=current->next_node;
	}
	return buff;
}



            //                 // cross check implementation
void * pipe_communication(void * arg){
            // read list command from write-pipe[0]
            // print the list of all the processes and send via 
    while(1) {
        int c_count = client_counter;
        close(client_list[c_count-1].write_pipe[1]);
        char input[4];

        int p_count = read(client_list[c_count-1].write_pipe[0],input,4);
                if (strcmp(input,"list")==0){
                    close(client_list[c_count-1].read_pipe[0]);
                    char * p_out = print_list((node *)arg);
                
                    int w =write(client_list[c_count-1].read_pipe[1],p_out,strlen(p_out));
                        if (w == -1) {
                            perror("Cannot retrive list");
                        }
                    
                }

        
    }
}



            // CUSTOM IMPLEMENTATION FOR SIGNAL HANDLER
    int c_pid;
    int status;
    void sig_handler(int signo){
            // wait for any child but do not block- NON BLOCKING WAIT BEHAVIOR IS ACHIEVED BY WAITPID WITH WNOHANG FLAG
        c_pid = waitpid(-1,&status,WNOHANG);
        Kill_node(head_main,c_pid,"Inactive");
    }


                    // LINKED LIST UTILITIES
                    /* 
                        - PRINT LIST
                        - PRINT ACTIVE LIST
                        - INSERT NODE
                        - DELETE NODE
                    */
char * active_list(node * head){

    // check status == "Active"
    // print only those nodes


    char *buff=malloc(2000);
    
	strcpy(buff, "\nName\t\tPID\t\tStatus\t\tStart Time\t\tEnd Time\t\tElapsed Time\n");
	node *current=head;
	
    while (current!=NULL) {

            // condition check
            if(strcmp(current->status,"Active")==0){
                strcat(buff, current->name_process);
		        strcat(buff, "\t"); strcat(buff, "\t");

		char id[20];
		int count=sprintf(id, "%d", current->pid);
		strcat(buff, id);
		strcat(buff, "\t"); strcat(buff, "\t");
		strcat(buff, current->status);
		strcat(buff, "\t"); strcat(buff, "\t");
		strcat(buff, current->start_time);
		strcat(buff, "\t"); strcat(buff, "\t");
		strcat(buff, current->end_time);
            }
            current=current->next_node;

    }
            return buff;

}




// INSERT NODE
void Insert_node(node *head, char *name, pid_t pid, char *status, char *start_time, char *end_time){


node * current = head;
// reach till the end of the list
            while(current->next_node != NULL){
                    current=current->next_node;
            }
        current->next_node = malloc(sizeof(node));
// populate the new node

    strcpy(current->next_node->name_process,name);
    strcpy(current->next_node->status,status);
    strcpy(current->next_node->start_time,start_time);
    strcpy(current->next_node->end_time,end_time);
    current->next_node->pid = pid;
}


// DELETE NODE
void Delete_node(node * head){
node * curr = head;

    while(curr->next_node->next_node!=NULL){
        curr= curr->next_node;
    }
    curr->next_node = NULL;
}



// KILL NODE BY PID
void Kill_node(node * head, int pid, char * sig){
        // finding criteria: run while loop 
        // if curr->pid == pid(parameter).. Set the end time value for that node
        time_t current_time;
        node * curr = head;
        struct tm * time_data;
        time(&current_time);
        char end_time[9];
        time_data = localtime(&current_time); // get time values - current time at that time 
         
        strftime(end_time, sizeof(end_time), "%H:%M:%S", time_data); // time formatting


            // find the node
            while(curr->pid != pid){
                curr=curr->next_node;
            }
            strcpy(curr->status,sig);


            if (strcmp(sig,"Inactive")==0) {
                    strcpy(curr->end_time,end_time);
            }
}



void Time_Diff(struct time t1, struct time t2, struct time *t3) {
// convert both the times into seconds
int seconds_t1 = t1.hour*60*60 + t1.minute*60 + t1.second;
int seconds_t2 = t2.hour*60*60 + t2.minute*60 + t2.second;
int time_diff = seconds_t1 - seconds_t2;

t3->minute= time_diff/60; //seconds/60 = minutes
t3->hour = t3->minute/60;  // hours = minutes//60
t3->second = time_diff%60;
}


int Kill_node_by_name(node * head, char * name){
       node * curr = head;
            // find the node
            // criteria :-> by name/ not NULL/ already killed
            // while name is not same and node is not null OR name gets matched and the program is aleady inactive and next node is not null
            while((strcmp(name, curr->name_process)!=0 && curr->next_node!=NULL) || (strcmp(name, curr->name_process)==0 && strcmp(curr->status, "Inactive")==0 && curr->next_node!=NULL)){
                curr=curr->next_node;
            }


           if (curr->next_node==NULL && strcmp(curr->name_process, name)!=0) {
		return -1;
	}
	else {
		Kill_node(head, curr->pid, "Inactive");
		//strcpy(current->status, "Dead");
		return curr->pid;
	}
}




int main(int argc, char * argv[]){

// LINKED LIST CONFIGURATION
node * head = NULL;
head = malloc(sizeof(node));
strcpy(head->name_process,"Shell");
head->name_process[strlen("Shell")] = '\0';
head->pid = getpid();
strcpy(head->status,"Active");

time_t curr_time;
struct tm * time_info;

char time_disp[9];
time(&curr_time);

time_info = localtime(&curr_time);
strftime(time_disp, sizeof(time_disp), "%H:%M:%S", time_info);
	//puts(timestring);

strcpy(head->start_time,time_disp);
strcpy(head->end_time," ");
head->next_node = NULL;
head_main = head;


__sighandler_t sig = signal(SIGCHLD,sig_handler);

    if (sig == SIG_ERR) {
        perror("Error in Signal Handling");
}


// SOCKET CONFIGURATION


 int sockfd, newsockfd, portno, clilen, pid;
    struct sockaddr_in serv_addr, cli_addr;
    char str[size];

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



    // Intelligent THREAD


        pthread_t inter_thread;
	    int th1=pthread_create(&inter_thread, NULL, Intelligent_Server, NULL);
	        if (th1) {
                perror("Intelligent Thread Failed"); 
                }

                // real server code, handling multiple clients
    while (1) {
        newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr, &clilen);
            if (newsockfd < 0) {
                perror("Error at Accepting Connection ");
                exit(1);
            }
            client_counter++;

                // Initialize Pipes
                int w_write_pipe[2];
                int r_read_pipe[2];

                pipe(w_write_pipe);
                    if(w_write_pipe<0) {
                         perror("Error at Write Pipe Initialization. Pipe Failed "); 
                         exit(1);
                    }

                pipe(r_read_pipe);
                    if(r_read_pipe<0) {
                         perror("Error at Read Pipe Initialization. Pipe Failed "); 
                         exit(1);
                    }

                client_list[client_counter-1].write_pipe[0] = w_write_pipe[0];     
                client_list[client_counter-1].write_pipe[1] = w_write_pipe[1];

                client_list[client_counter-1].read_pipe[0] =  r_read_pipe[0];
                client_list[client_counter-1].read_pipe[1] =  r_read_pipe[1];




            // accomodating multiple connections
            
            pid = fork();
                if (pid<0){
                    perror("Error at Fork Api ");
                    exit(1);
                }

                if (pid == 0) {
                  //  close(sockfd);
                   
                    // call main function
                    pthread_t p_thread;
			int th2=pthread_create(&p_thread, NULL, pipe_communication, (void *)head);
                        if (th2==-1) {
                            perror("Pipe Communication Error: ");    
                            }
                
                    char buff[size];
                    char displaybuffer[size];
                    int n=0;
                    char * token=NULL;

            while(1) {
                bzero(buff,size);
                n = read(newsockfd,buff,size);
                                if (n < 0){
                                    perror("Error at Reading Data ");
                                    exit(1);
                                }

                                if (n == 0) {
                                    perror("Ending Connection ");

                                }
                            // echo client's input on server's screen
                           // n = sprintf(displaybuffer, "The client entered: %s\n", buff);
                           // write(STDOUT_FILENO,buff,strlen(buff) +1 );
                                            
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
                                write(newsockfd,buff,n);
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
                                write(newsockfd,buff, n);
            } 
            
            else if (strcmp(token, "mul") == 0) {
                //Mul();

                    float result = atof(strtok(NULL, " "));
                    float curr_digit = 0;
                    token = strtok(NULL, " ");

                    while (token != NULL) {
                        curr_digit=atof(token);
                        result*=curr_digit;
                        token=strtok(NULL," ");
                    }
                    n = sprintf(buff, "The answer is: %.2f \n", result);
                    write(newsockfd, buff, strlen(buff)+1);

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
                                write(newsockfd,buff, n);
                            }


             else if (strcmp(token, "run") == 0) {
                //Run();
                            		token = strtok(NULL, delimiter);
                        
						char *file= token;
						char astatus[]="Active";
				
						//start time information
						time_t curr_time;
						struct tm * time_det;
						char starttime[9];
						time(&curr_time);
						time_det=localtime(&curr_time);
						strftime(starttime, sizeof(starttime), "%H:%M:%S", time_det);
						//puts(starttime);
						//create pipe for parent child communication
						int pipe3[2];
						if (pipe(pipe3)==-1) {perror("pipe3: "); exit(EXIT_FAILURE);}
						fcntl(pipe3[1], F_SETFD, FD_CLOEXEC);
						fcntl(pipe3[0], F_SETFD, FD_CLOEXEC);
						//fork to create new application process
						int cpid= fork();
						if (cpid==-1) {perror("fork: "); exit(0);}
						//child process
						if (cpid==0) {
							//close reading end of pipe
							close(pipe3[0]);
							pid_t pid= getpid();
                    
							int err=execlp(file,file,NULL);
							if (err==-1) {
								perror("exec: ");
								if (write(pipe3[1], "FAILED\n", 7)==-1) {perror("pipe3 write "); exit(EXIT_FAILURE);}
								send (newsockfd, "failed written\n", 15, 0);

							}
						}
						//parent process
						if (cpid!=0) {
							//close writing end of pipe
							close(pipe3[1]);
							//sleep(0.4);
							char in[7];
							int chldcount=read(pipe3[0], in, 7);
							if (chldcount==0) { //successfuly read from a closed pipe
								//adding the new process to the process list
								char emp[]=" ";
								Insert_node(head, file, cpid, astatus, starttime, emp);
								//debugging
								write(STDOUT_FILENO, "pushed\n", 7);
								if (send(newsockfd, "execution complete\0", 19, 0)==-1) {perror("send run"); exit(EXIT_FAILURE);}
							}
						}
				
					
                                    


            } else if (strcmp(token, "list") == 0) {
                //list();

            char * list_print = print_list(head_main);

                    int s_send = write(newsockfd,list_print,strlen(list_print)+1);
                        if (s_send == -1 ){
                            perror("Cannot send Process List");
                        }


            } else if (strcmp(token, "list-all") == 0) {
                // list-all();

            char * list_print = active_list(head_main);

                    int s_send = write(newsockfd,list_print,strlen(list_print)+1);
                        if (s_send == -1 ){
                            perror("Cannot send Process List");
                        }



            } else if (strcmp(token, "killpid") == 0) {
                char * next_tok = strtok(NULL,delimiter);
                int pid_kill  = atoi(next_tok);
                Kill_node(head_main,pid_kill,"Inactive");
                // Kill();

            } 
            else if (strcmp(token, "killname") == 0) {
                // Kill();
                char * next_tok = strtok(NULL,delimiter);
                Kill_node_by_name(head_main,next_tok);
                
            } 
            else if (strcmp(token, "exit") == 0) {
                
                n = sprintf(buff, "Exit from program\n");
                write(newsockfd, buff, n);
                exit(1);
            } 
            
            else {
                n = sprintf(buff, "Invalid Command \n");
                write(newsockfd, buff, n);
            } 

}// end while
                
                
                
                }
                else { 

                    client_list[client_counter-1].pid = pid;
                    char address_string[INET6_ADDRSTRLEN];
                    inet_ntop(AF_INET,&(cli_addr.sin_addr),address_string,INET6_ADDRSTRLEN);
                    int port = (int) ntohs(cli_addr.sin_port);
                    char ip_add[strlen(address_string)];
                    strcpy(ip_add,address_string);
                    strcpy(client_list[client_counter-1].ip_address,address_string);
                    client_list[client_counter-1].sockfd = newsockfd;
                    client_list[client_counter-1].c_port = port;
                
                
                
                //close(newsockfd);
                }





    } // end while

exit(0);
}