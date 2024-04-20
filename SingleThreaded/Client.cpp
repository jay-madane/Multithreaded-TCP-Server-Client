#include<bits/stdc++.h>
#include<winsock2.h>
using namespace std;

typedef int socklen_t;

#define PORT 8010
#define BUFFER_SIZE 1024

void runClient(){
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
    if(clientSocket < 0){
        cerr<<"Error initializing socket";
        return;
    }

    // preparing the sockaddr_in structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    memcpy(&serverAddr.sin_addr.s_addr, server->h_addr, server->h_length);
    serverAddr.sin_port = htons(PORT);

    // connecting
    int connection = connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if(connection < 0){
        cerr<<"Connection failed";
        return;
    }

    // receive from server
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
    if(bytesReceived < 0){
        cerr<<"Error receiving message\n";
        closesocket(clientSocket);
        return;
    }
    buffer[bytesReceived] = '\0';
    cout<<"Response from server: "<<buffer<<'\n';
    closesocket(clientSocket);
}

int main(){
    WSADATA wsaData;
    int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(startupResult != 0){
        cerr << "WSAStartup failed: " << startupResult << endl;
        return 1;
    }

    for(int i=0; i<100; i++){
        runClient();
    }

    WSACleanup(); // Clean up Winsock
    return 0;
}