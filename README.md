# Multithreaded TCP Server and Client

The implementation of a Multithreaded TCP Server and Client. This project comprises server-client communication implemented in C++ using Winsock2 library for Windows platforms. The server handles multiple client connections concurrently by utilizing a thread pool. The client sends requests to the server and receives responses. The motivation for the project was to understand and apply the concepts of threads, socket programming, utilization of the pthread library to manage threads and synchronise them using mutexes and condition variables.

A multithreaded server can handle multiple requests from multiple clients at the same time. Therefore, it can respond quickly and efficiently to increasing client queries, which can reduce waiting time for users.

## Important Definitions

- Winsock2: Winsock is a programming interface and the supporting program that handles input/output requests for Internet applications in a Windows operating system.

- Socket: A socket is an endpoint of a bidirectional communication channel between two programs running on the network.

- Thread Pool: A collection of worker threads that are created once and reused, instead of being created and destroyed each time a task is executed.

## Implementation

### Server
- Initialize Winsock.
- Create a thread pool to handle incoming client connections.
- Create a socket using IPv4, TCP, and IP protocol.
- Bind the socket to the server address.
- Listen for incoming connections with a specified backlog.
- Accept incoming connections from clients.
- Put the connection in a queue for processing by available threads in the thread pool.
- Read data from a file.
- Send the data to the client.
- Close the client socket after sending the response.

### Client
- Initialize Winsock.
- Get the server address.
- Create a socket and connect to the server.
- Send a request to the server.
- Receive and display the response from the server.
- Optionally, send multiple requests to the server.

## Compilation and Execution

To compile the necessary files for the Server:
```
g++ Server.cpp -o Server -lws2_32 -pthread
```

To compile the necessary files for the Client:
```
g++ Client.cpp -o Client -lws2_32
```

To start the server:
```
./Server
```

To start the client:
```
./Client
```
