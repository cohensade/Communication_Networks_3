#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <time.h>

int main(int argc, char** argv) {
    if (argc != 7) {
        fprintf(stderr, "not receiving 7 arg\n", argv[0]); // print error message if argument count is incorrect
        return 1;
    }

    char* ip = argv[2]; // server IP address
    int port = atoi(argv[4]); // server port number
    char *algo = argv[6]; // congestion control algorithm

    socklen_t length = strlen(algo)+1; // length of congestion control algorithm name

    int socketa = socket(AF_INET, SOCK_STREAM, 0); // create a socket
    if (socketa < 0) {
        perror("takala in creating socket"); // print error message if socket creation fails
        exit(EXIT_FAILURE);
    }

    if (setsockopt(socketa, IPPROTO_TCP, TCP_CONGESTION, algo, length) != 0) {
        perror("takala settings algo"); // set congestion control algorithm
        close(socketa);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address; // server address structure
    memset(&server_address, 0, sizeof(server_address)); // initialize memory for server address
    server_address.sin_family = AF_INET; // address family (IPv4)
    server_address.sin_addr.s_addr = inet_addr(ip); // server IP address
    server_address.sin_port = htons(port); // server port number

    if (connect(socketa, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("takala connection failed"); // connect to the server
        close(socketa);
        exit(EXIT_FAILURE);
    }
    int file_size = 2097200; // file size (2MB in bytes)
    char *content = (char *)malloc(file_size); // allocate memory for content
    if (content == NULL) {
        perror("takala creating file"); // print error message if memory allocation fails
        exit(EXIT_FAILURE);
    }
    srand(time(NULL)); // seed for random number generator
    for (int i = 0; i < file_size; i++)
        content[i] = (char)(rand() % 256); // generate random content

    FILE *file = fopen("file.txt", "wb"); // open file for writing
    if (file == NULL) {
        perror("takala in opening file for writing"); // print error message if file opening fails
        exit(EXIT_FAILURE);
    }
    fwrite(content, sizeof(char), file_size, file); // write content to file
    fclose(file); // Close file

    file = fopen("file.txt", "rb"); // open file for reading
    if (file == NULL) {
        perror("takala in opening file for reading"); // print error message if file opening fails
        exit(EXIT_FAILURE);
    }

    while (1) {
        char buffer[1024]; // buffer for reading file content
        size_t kriaa; // variable to store the result of fread
        while ((kriaa = fread(buffer, sizeof(char), 1024, file)) > 0) { // read up to 1024 elements from the file into the buffer
            if (send(socketa, buffer, kriaa, 0) < 0) { // if data was successfully read, send it over the socket
                perror("takale sending file failed"); // send file content over the socket
                exit(EXIT_FAILURE);
            }
        }
        fclose(file); // close file

        int choice;
        printf("Do you want to send the file again? if yes enter 1 if not enter 0 and it will close the connection\n");
        scanf(" %d", &choice); // prompt user to send file again
        if (choice != 1) {
            break; // exit the loop
        }
        // reopen the file for reading in the next iteration
        file = fopen("file.txt", "rb");
        if (file == NULL) {
            perror("takala in opening file for reading"); // print error message if file opening fails
            exit(EXIT_FAILURE);
        }
    }
    close(socketa); // close socket

    free(content); // free memory allocated for content

    printf("hibur nisgar.\n");

    return 0;
}