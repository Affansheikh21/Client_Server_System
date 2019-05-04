Mohammad Affan Sheikh - Multi Threaded Client Server System
Architecture

I have implemented TCP/IP Client-Server System using Multi-Threading mechanism. 
There are a total of two files. Mainly, my_server.c and cli_final.c. They contain the code chunks for server and client respectively. The client side is simple and uses a single process mechanism for sending requests to, and receiving response from the server.

The client can request for the following operations:
1- Add, followed by operands
2- Sub, followed by operands
3- Mul, followed by operands
4- Div, followed by operands
5- Run, followed by the name of the executable file
6- List, show list of running processes
7- Kill, kill a particular process - using name, or by pid

The server side is quite extensive. In order to run the server file, it needs to be compiled with -pthread flag i.e $gcc -o executable my_server.c -pthread. On execution the user needs to provide a port number, as the command line argument. This port number can be any integer value ranging from 2000 to 65535. For the execution of the client file, the user needs to run the executable along with explicit arguments of server’s IP Address, and port number (as approved by the server). 

The server is a MultiThreaded Implementation, and contain threads for performing dedicated tasks. The server is capable of storing details of all the clients connected and the total number of running processes. These structures are implemented as Array, and LinkedList respectively. 

The first thread, Intelligent Thread is responsible for making the server interactive. It is dedicated to print details regarding the server operability and enables the user to enter the following commands: 
1- Clients
2- Client [ip]/[pid]
3- Lists

Second, the main thread is used for socket configuration which mainly consist of listening to connection and accepting new clients requests. A third thread is responsible for reading from a pipe. This thread is created in the client child process and therefore there is an instance of it for every client child process. The pipe is used to communicate between the second thread and these client threads. When the second thread gets a command to get the process lists from each client, the thread runs a loop for each client and sends a request for process lists through one pipe and this is when the client thread that is read blocked, gets unblocked and then sends its process list through a second pipe to the interactive thread, which then displays all the process lists.

Infinite while loop runs in both, the Intelligent, and the main thread so that it can respond to the client 24/7. In order to cater multiple client connections, a while loop runs in the main server and after acceptance of each client’s connection, the server forks a new process, and this process then handles the client’s requests. Furthermore, the program provides the option for killing the process by it’s pid, or by name. 
Other features of the application architecture include: 
a. The process list is implemented as a linked list of a process structure. This is to ensure there is no limit to the number of processes that can be run with one client;

b. An illegal application name provided with the run command is handled by a failed execvp, which causes the pipe used between the parent child exec handler processes to close due to fcntl setf cloexec. This returns a 0 when the parent process tries to read through the pipe, indicating a failed execution. Thus, an if statement then determines whether to push the illegal run process to the process list or not.

c. A signal handler for SIGCHLD is registered and the signal handler changes the status of the process which died since it has the terminated processes ID using wait with WNOHANG and a reference to the global process linked list head, through which it can traverse, looking for the matching process, in order to change its status to dead.

Limitations

There are a number of limitations associated with this project. Firstly, the lists when displayed are not properly formatted and might be difficult to read. Secondly, the system lacks efficient concurrency mechanism (no mutex implementation) due to which the program might get blocked at different instances and can behave abnormally. The program frequently gets blocked.

The server commands which are handled by the threads might lead to server getting blocked. On the other hand, due to the usage of string functions, the concurrent overloading of input buffers might display unusual output at times. 



Architectural Diagram


