#include<bits/stdc++.h>
#include<winsock2.h>
using namespace std;

typedef int socklen_t;

#define PORT 8010

void runServer(){
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);

    // create the socket - IPv4, TCP, IP protocol
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0){
        cerr<<"Error initializing socket";
        return;
    }

    // preparing the server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    // binding socket to server address
    int bindStatus = bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(bindStatus < 0){
        cerr<<"Bind Failed\n";
        return;
    }

    while(1){

        // listening for incoming connections
        int listenStatus = listen(serverSocket, 10);
        if(listenStatus < 0){
            cerr<<"Listening failed\n";
            return;
        }
        cout<<"Server is listening on port "<<PORT<<'\n';

        // accepting connection from client
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if(clientSocket < 0){
            cerr<<"Connection not accepted\n";
            return;
        }
        cout<<"Connected to Client:\n"<<"IP: "<<inet_ntoa(clientAddr.sin_addr)<<'\t'<<"Port Number: "<<ntohs(clientAddr.sin_port)<<'\n';

        // sending message to client
        const char *msg = "Hello from server!";
        int sendStatus = send(clientSocket, msg, strlen(msg), 0);
        if(sendStatus < 0){
            cerr<<"Error sending message\n";
            closesocket(clientSocket);
            return;
        }
        cout<<"Message sent to client\n";
        Sleep(1);
    }
    closesocket(clientSocket);
    closesocket(serverSocket);
}

int main(){
    WSADATA wsaData;
    int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(startupResult != 0){
        cerr << "WSAStartup failed: " << startupResult << endl;
        return 1;
    }

    runServer();

    WSACleanup(); // Clean up Winsock
    return 0;
}