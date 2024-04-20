#include<bits/stdc++.h>
#include<winsock2.h>
#include<pthread.h>
using namespace std;

#define PORT 8010
#define BUFFER_SIZE 1024

// function for error checks
void checkError(int check, string message){
    if(check < 0){
        cerr<<message<<'\n';
        exit(1);
    }
}

// function to send request to the server
void sendRequest(){
    int clientSocket;
    struct sockaddr_in serverAddr;
    struct hostent *server;
    
    // getting server address
    server = gethostbyname("localhost");
    if(server == NULL){
        cerr<<"Unknown Server\n";
        return;
    }

    // create the socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    checkError(clientSocket, "Error initializing socket");

    // preparing the sockaddr_in structure
    memset(&serverAddr, 0, sizeof(serverAddr));

    // address family is AF INET in the Internet domain
    serverAddr.sin_family = AF_INET;

    // copy the server address to the sockaddr_in structure
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);

    // setiing TCP port number
    serverAddr.sin_port = htons(PORT);

    // connecting to the server
    int connection = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    checkError(connection, "Connection failed " + WSAGetLastError());

    // receive data from the server
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if(bytesReceived < 0){
        cerr<<"Error receiving data\n";
        closesocket(clientSocket);
        return;
    }
    buffer[bytesReceived] = '\0';
    cout<<"Response from the server: "<<buffer<<'\n';
    closesocket(clientSocket);
    return;
}

// function to send multiple requests
void runClient(int requests = 100){
    for(int i=0; i<requests; i++){
        sendRequest();
    }
}

int main(){
    WSADATA wsaData;
    int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(startupResult != 0){
        cerr<<"WSAStartup failed: "<<startupResult<<'\n';
        return 1;
    }
    int rq;
    cout<<"Enter the number of requests: ";
    cin>>rq;
    runClient(rq);
    WSACleanup(); // Clean up Winsock
}