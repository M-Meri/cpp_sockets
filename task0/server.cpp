#include <iostream>
#include <unistd.h>
#include <algorithm>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include <mutex>

std::mutex m;

void handleClient(int clientSocket, std::vector<int>& clients) {
    char buffer[1024];
    int bytesReceived;

    // Add client socket to the list
    m.lock();
    clients.push_back(clientSocket);
    m.unlock();

    // Handle client communication
    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Received message from client: " << buffer << std::endl;

        // Relay the message to other clients
        for (int otherClientSocket : clients) {
            if (otherClientSocket != clientSocket) {
                send(otherClientSocket, buffer, bytesReceived, 0);
            }
        }
    }

}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    std::vector<int> clients;

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    } 

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12351);
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }


    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        return 1;
    }

    std::cout << "Server listening on port 12351" << std::endl;

    // Accept connections and handle them in separate threads
    while ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen)) != -1) {
        std::cout << "New connection accepted" << std::endl;

        
        std::thread clientThread(handleClient, clientSocket, std::ref(clients));
        clientThread.detach(); 
    }

  
    close(serverSocket);

    return 0;
}