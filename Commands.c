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

const int size = 1000;
char delimiter[1] = {' '};
char * token;
char printbuffer[size];
int number;

void Add();
void Sub();
void Mul();
void Div();
void Run();
void List();
void Kill();



int main(int argc, char *argv[]){


    int readfromscreen=0;
    char buff[size];
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
                //Run();
            } else if (strcmp(token, "list") == 0) {
               // List();
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
         token = strtok(NULL, delimiter);
         
           while ((token = strtok(NULL, delimiter)) != NULL) 
            {
                result*= atoi(token);

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
