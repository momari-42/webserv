#include <iostream>
#include <vector>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h>

#define BUFFER_SIZE 1024

// Helper function to create and bind a socket to a port
int create_listening_socket(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("Set socket options failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Binding failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 10) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Make the socket non-blocking
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    std::cout << "Listening on port " << port << "\n";
    return sockfd;
}

int main() {
    std::vector<int> ports;
    ports.push_back(8080); // Ports to listen on
    ports.push_back(8081); // Ports to listen on
    std::vector<int> listening_sockets;

    // Create listening sockets for each port
    for (int port : ports) {
        listening_sockets.push_back(create_listening_socket(port));
    }

    // Create a kqueue
    int kq = kqueue();
    if (kq == -1) {
        perror("kqueue creation failed");
        exit(EXIT_FAILURE);
    }

    // Register listening sockets with kqueue
    struct kevent changes[ports.size()];
    for (size_t i = 0; i < listening_sockets.size(); ++i) {
        EV_SET(&changes[i], listening_sockets[i], EVFILT_READ, EV_ADD, 0, 0, NULL);
    }

    if (kevent(kq, changes, listening_sockets.size(), NULL, 0, NULL) == -1) {
        perror("kevent register failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is running...\n";

    // Main event loop
    while (true) {
        struct kevent events[10];
        memset(events, 0, sizeof(events));
        std::cout << "============================================================" << std::endl;
        int nev = kevent(kq, NULL, 0, events, 10, NULL);
        std::cout << "************************************************************" << std::endl;
        if (nev == -1) {
            perror("kevent failed");
            break;
        }

        for (int i = 0; i < nev; ++i) {
            int fd = events[i].ident;

            // Check if it's a listening socket
            auto it = std::find(listening_sockets.begin(), listening_sockets.end(), fd);
            if (it != listening_sockets.end()) {
                // Accept a new connection
                std::cout << "9beeeeeeeel" << std::endl;
                sockaddr_in client_address;
                socklen_t client_len = sizeof(client_address);
                int client_fd = accept(fd, (struct sockaddr*)&client_address, &client_len);
                if (client_fd < 0) {
                    perror("Accept failed");
                    continue;
                }

                std::cout << "New connection accepted\n";

                // Make the client socket non-blocking
                fcntl(client_fd, F_SETFL, O_NONBLOCK);

                // Register the client socket for reading
                struct kevent client_event;
                EV_SET(&client_event, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                if (kevent(kq, &client_event, 1, NULL, 0, NULL) == -1) {
                    perror("kevent client register failed");
                    close(client_fd);
                }
            } else {
                // Handle data from a client socket
                std::cout << "baaaaaaaaa3d" << std::endl;
                char buffer[BUFFER_SIZE];
                ssize_t bytes_read = 0;
                std::string request_data = "";

                // Read all available data from the socket
                while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[bytes_read] = '\0';
                    request_data += buffer;  // Accumulate the data

                    // Check if a complete request has been received
                    if (request_data.find("\r\n\r\n") != std::string::npos) {
                        // We have a complete HTTP request
                        break;
                    }
                }

                if (bytes_read == 0) {
                    std::cout << "Client disconnected\n";
                    close(fd);
                    continue;
                } else if (bytes_read < 0 && errno != EAGAIN && errno != EWOULDBLOCK) {
                    perror("read error");
                    close(fd);
                    continue;
                }

                // Process the complete request data
                std::cout << "Received request:\n" << request_data << "\n";

                // Send an HTTP response
                std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
                write(fd, response.c_str(), response.size());
                close(fd);  // Close the connection after sending the response
            }
        }
    }


    // Cleanup
    for (int sockfd : listening_sockets) {
        close(sockfd);
    }
    close(kq);

    return 0;
}
