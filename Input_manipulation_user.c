#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h>
#include <errno.h>
#include <error.h>

bool inputvalidation(char buff[3]){
    // convert all kind of inputs to lower case
    for(int i = 0; i < 3; i++) {
        buff[i] = tolower(buff[i]);
    }

    if(buff[0] == 'a' && buff[1] == 'd' && buff[2] == 'd') {
        return true;
    }
    else if(buff[0] == 's' && buff[1] == 'u' && buff[2] == 'b') {
        return true;
    }
    else if(buff[0] == 'm' && buff[1] == 'u' && buff[2] == 'l') {
        return true;
    }
    else if(buff[0] == 'd' && buff[1] == 'i' && buff[2] == 'v') {
        return true;
    }
    else if(buff[0] == 'r' && buff[1] == 'u' && buff[2] == 'n') {
        return true;
    }
    else {
        return false;
    }

}

int countspaces(char input[100]){

                     int count =0;
                     int space=0;

                    for(int i=0; i<strlen(input);i++){

                        if (input[i] == ' '){
                        space++;
                        }
                    }

                            if(input[strlen(input)-2] == ' ') {
                            space--;
                            }

    count = space+1;
        return count;
   
}


int main(){
write(STDOUT_FILENO,"Enter Command:", sizeof("Enter Command:"));
char input[100];
		int count=read(STDIN_FILENO, input, 100);
		input[count-1]='\0';

        int listsize = countspaces(input);        

		char *list[listsize];
		// Begin tokenization and entering tokens in list
		const char delim[]=" ";
		char *token;
		token=strtok(input, delim);
		const char newline[]="\n";
		int i=0;
		while (token!= NULL) {
			list[i]=token;
			i++;
			token=strtok(NULL,delim);
		}
		 

            for (int j=0;j<listsize;j++){

                printf("%s\n",list[j]);
            }

            printf("%d\n",listsize);

}
