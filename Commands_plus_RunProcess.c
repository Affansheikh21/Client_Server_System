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
#include <signal.h>
#include <sys/wait.h>

const int size = 1000;
char delimiter[1] = {' '};
char * token;
char printbuffer[size];
int number;
char buff[size];


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




int main(int argc, char *argv[]){


    int readfromscreen=0;
    signal(SIGINT,sighandler);
    write(STDOUT_FILENO,"Enter Command: ", sizeof("Enter Command: "));
    while (1) {
        
        readfromscreen = read(STDIN_FILENO, buff, size);

            char string[readfromscreen - 1];
            sscanf(buff, "%[^\n]", string);

            token = strtok(string, delimiter);
            
            if (strcmp(token, "add") == 0) {
                Add();
            } else if (strcmp(token, "sub") == 0) {
               Sub();
            } else if (strcmp(token, "mul") == 0) {
                Mul();
            } else if (strcmp(token, "div") == 0) {
                Div();
            } else if (strcmp(token, "run") == 0) {
                Run();
            } else if (strcmp(token, "list") == 0) {
                list();
            } else if (strcmp(token, "kill") == 0) {
                // Kill();
            } else if (strcmp(token, "exit") == 0) {
                
                readfromscreen = sprintf(buff, "Exit from program\n");
                write(STDOUT_FILENO, buff, readfromscreen);
                exit(0);
            } 
            
            else {
                readfromscreen = sprintf(buff, "Invalid Command \n");
                write(STDOUT_FILENO, buff, readfromscreen);
            }

        }




    }
     void Add(){
         int sum = 0;
           while ((token = strtok(NULL, delimiter)) != NULL) 
            {
                sum = sum + atoi(token);
            }

            	number = sprintf(printbuffer, "The answer is: %.2d \n", sum);
	write(STDOUT_FILENO, printbuffer, number);
    }

    void Sub(){
         int result;
         token = strtok(NULL, delimiter);
         result = atoi(token);
         
           while ((token = strtok(NULL, delimiter)) != NULL) 
            {
                result-= atoi(token);

            }

            	number = sprintf(printbuffer, "The answer is: %.2d \n", result);
	write(STDOUT_FILENO, printbuffer, number);
    }

    void Mul(){
         int result =1;
         int temp=0;
         token = strtok(NULL, delimiter);
         
           while ((token = strtok(NULL, delimiter)) != NULL) 
            {
                temp = atoi(token);
                result= result*temp;

            }

            	number = sprintf(printbuffer, "The answer is: %.2d \n", result);
	write(STDOUT_FILENO, printbuffer, number);
    }

    void Div(){
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

            	number = sprintf(printbuffer, "The answer is: %.2f \n", (float)result);
	write(STDOUT_FILENO, printbuffer, number);
    }

   /* void Run(){
    
    char * args[size] = {};
    int index=0;
    pid_t c_pid, pid;
    int status;
	
    
while ((token = strtok(NULL, delimiter)) != NULL) 
            {
                args[index] = token;
                index++;
                }
            args[index+1] = NULL;

  
    c_pid = fork();
    if (c_pid == 0) {
     //execute                                                                                                                                                            
            int ret=0;


            ret = execvp(args[0],args); 
                 if (ret == -1) {
                     perror("Error: ");

                    }
    //only get here if exec failed                                                                                                                                             
    perror("execvp failed");
  }
  else if (c_pid > 0){
    /* PARENT */
     // signal(SIGCHLD,sighandler);
     
/*
    int waitret =  waitpid(c_pid,&status,WNOHANG);
    if  (waitret < 0){
      perror("wait");
      _exit(1);
    }
    //printf("Parent: finished\n");

  }

  else{
    perror("fork failed");
    _exit(1);
  }
    }
    */

   void Run(){

char buff[size];

	if (ctr == MAX_LIMIT) {
		number =
				sprintf(buff,
						"Process table is full. Cannot create more processes!\n");
		write(STDOUT_FILENO, buff, number);
		return;
	}

	char* next_token = strtok(NULL, delimiter);
	if (next_token == NULL) {
		number = sprintf(buff, "No argument provided! \n");
		write(STDOUT_FILENO, buff, number);
		return;
	}

	// Create a pipe
	int my_pipe[2];
	pipe(my_pipe);

	int my_pid = fork();
	if (my_pid == -1) {
		perror("FORK FAILED");
		return;
	}
	// child process
	else if (my_pid == 0) {
		close(my_pipe[0]);

		number = sprintf(buff, "I'm a child! My id is %d\n My parent's id is %d\n",
				getpid(), getppid());
		write(STDOUT_FILENO, buff, number);

		// send data
		number = sprintf(buff, "%d", getpid());
		number = write(my_pipe[1], buff, size);

		number = execlp(next_token, NULL);
		if (number == -1) {
			if (errno == ENOENT) {
				number = execl(next_token, NULL);
				if (number == -1) {
					perror("EXECL FAILED TOO: ");
					ctr--; // delete the entry
					return;
				}
			} else {
				perror("EXECLP FAILED");
				ctr--; // delete the entry
				return;
			}
		}

	}
	// parent process
	else {
		close(my_pipe[1]);

		number = sprintf(buff, "I'm a parent with id %d! \n", getpid());
		write(STDOUT_FILENO, buff, number);

		int created_pid;

		number = read(my_pipe[0], buff, size);
		number = sscanf(buff, "%d", &created_pid);

		process_arr[ctr].pid = created_pid;
		process_arr[ctr].isActive = true;
		process_arr[ctr].name = strdup(next_token);
		time(&process_arr[ctr++].startingTime);

		close(my_pipe[1]);
	}

}

   void list()	{
       
       char buff[size];

	bool allProccesses = false;

	char* next_token = strtok(NULL, delimiter);
	if (next_token != NULL && strcmp(next_token,"all") == 0) {
		allProccesses = true;
	}

	number = sprintf(buff, "Total processes so far: %d\n", ctr);
	number = write(STDOUT_FILENO, buff, number);

	number =
			sprintf(buff,
					"    Name   \t pid \tactive\tStarting Time \t Ending Time\t Elapsed Time\n");
	number = write(STDOUT_FILENO, buff, number);

	for (int i = 0; i < ctr; i++) {

		if (!allProccesses && !process_arr[i].isActive) continue;

		float diff = difftime(process_arr[i].endingTime,
				process_arr[i].startingTime);
		if (diff < 0) {
			time_t currTime;
			time(&currTime);
			diff = difftime(currTime, process_arr[i].startingTime);

		}

		char s[9], e[9];
		strftime(s, 9, "%X", gmtime(&process_arr[i].startingTime));
		strftime(e, 9, "%X", gmtime(&process_arr[i].endingTime));

		number = sprintf(buff, "%8s \t %lu\t%s\t %s \t %9s \t %f \n",
				process_arr[i].name, process_arr[i].pid,
				process_arr[i].isActive ? "\x1B[32mtrue\x1B[0m" : "\x1B[31mfalse\x1B[0m", s,
				process_arr[i].isActive ? "--:--:--" : e, diff);
		number = write(STDOUT_FILENO, buff, number);
	}

	return;

}

    
 
