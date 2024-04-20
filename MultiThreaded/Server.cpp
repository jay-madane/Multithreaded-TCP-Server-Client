#include<bits/stdc++.h>
#include<winsock2.h>
#include<pthread.h>
using namespace std;

typedef int socklen_t;

#define PORT 8010
#define SERVER_BACKLOG 500
#define THREAD_POOL_SIZE 30
#define FILE_NAME "info.txt"

// queue for storing client sockets
queue<int*> q;
pthread_t threadPool[THREAD_POOL_SIZE];

// mutex for locking the queue and prevent race conditions (when multiple threads try to access the same data at the same time.)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition = PTHREAD_COND_INITIALIZER; // lets threads wait untill something useful happens and it can do useful work

// function for error checks
void checkError(int check, string message){
    if(check < 0){
        cerr<<message<<'\n';
        exit(1);
    }
}

// function that the thread will execute
void* handleConnection(void* p_clientSocket){
    int clientSocket = *((int*)p_clientSocket);

    // reading data from file
    string msg, temp;
    char* message = NULL;
    ifstream ReadFile(FILE_NAME);
    if(!ReadFile.is_open()) {
        cerr<<"Error in opening file\n";
        closesocket(clientSocket);
        return NULL;
    }
    while(getline(ReadFile, temp)) {
        msg += temp;
    }
    ReadFile.close();

    message = new char[msg.size() + 1];
    strcpy(message, msg.c_str());

    // sending message to client
    int sendStatus = send(clientSocket, message, strlen(message), 0);
    if(sendStatus < 0){
        cerr<<"Error sending message\n";
        delete[] message;
        closesocket(clientSocket);
        return NULL;
    }
    Sleep(1000);  // test
    cout<<"Message sent to client\n";
    delete[] message;
    closesocket(clientSocket);
    return NULL;
}

// function to handle thread executions
void* threadExec(void* arg){
    while(true){
        pthread_mutex_lock(&mutex);

        // if queue is empty wait on the condition
        while(q.empty()) {
            pthread_cond_wait(&condition, &mutex);
        }

        // pop and store the work from the queue for processing
        int* p_client = q.front();
        q.pop();
        pthread_mutex_unlock(&mutex);

        // process the work and send data to the client
        handleConnection(p_client);
    }
    return NULL;
}

void runServer(){
    int serverSocket, connection;
    struct sockaddr_in serverAddr, clientAddr;  // to pass the struct in bind
    socklen_t addrLen = sizeof(struct sockaddr_in);

    // create bunch of threads to handle future connections
    for(int i=0; i<THREAD_POOL_SIZE; i++){
        pthread_create(&threadPool[i], NULL, threadExec, NULL);
    }

    // create the socket - IPv4, TCP, IP protocol
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    checkError(serverSocket, "Error initializing socket");

    // preparing the server address
    memset(&serverAddr, 0, sizeof(serverAddr));

    // address family is AF INET in the Internet domain
    serverAddr.sin_family = AF_INET;

    // address can be a specific IP or INADDR ANY (special address (0.0.0.0) meaning “any address”)
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // TCP port number 
    serverAddr.sin_port = htons(PORT);

    // binding socket to server address
    int bindStatus = bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    checkError(bindStatus, "Bind Failed");

    // listen for incoming connections
    int listenStatus = listen(serverSocket, SERVER_BACKLOG);
    checkError(listenStatus, "Listening failed");
    cout<<"Server is listening on port "<<PORT<<'\n';

    while(true){
        // accept the incoming connection
        connection = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        checkError(connection, "Connection not accepted");
        cout<<"Connected to Client:\n"<<"IP: "<<inet_ntoa(clientAddr.sin_addr)<<'\t'<<"Port Number: "<<ntohs(clientAddr.sin_port)<<'\n';

        // putting connection in a queue so that an available thread can find it
        int* clientSocket = new int;
        *clientSocket = connection;
        pthread_mutex_lock(&mutex);  // lock the mutex
        q.push(clientSocket);
        pthread_cond_signal(&condition);
        pthread_mutex_unlock(&mutex);  // unlock the mutex

        // making thread for each client connection (not optimised)
        /* pthread thread;
           pthread_create(&thread, NULL, handleConnection, clientSocket);
        */
        
        // handleConnection(clientSocket);  // testing for server without threads
    }
    closesocket(serverSocket);
}

int main(){
    WSADATA wsaData;
    int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(startupResult != 0){
        cerr<<"WSAStartup failed: "<<startupResult<<'\n';
        return 1;
    }
    runServer();
    WSACleanup(); // Clean up Winsock
    return 0;
}