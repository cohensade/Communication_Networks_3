#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/tcp.h>

int main(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "not receiving 5 arg\n", argv[0]);   // print error message if argument count is incorrect
        return 1;
    }

    int port = atoi(argv[2]);       // receiver port number
    char *algo = argv[4];                 // congestion control algorithm

    socklen_t length = strlen(algo) + 1;        // length of congestion control algorithm name

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

    struct timeval hathala, sof;           // time variables for measuring transfer time
    double zman = 0;                         // total transfer time
    unsigned int sum_received_bytes = 0;         // total bytes received
    struct sockaddr_in sender;                     // sender address structure
    struct sockaddr_in receiver;                   // receiver address structure
    socklen_t sender_length = sizeof(sender);         // length of sender address
    memset(&sender, 0, sizeof(sender));            // initialize memory for sender address
    memset(&receiver, 0, sizeof(receiver));        // initialize memory for receiver address

    printf("receiver niftah....\n");

    receiver.sin_addr.s_addr = INADDR_ANY;          // receiver IP address
    receiver.sin_family = AF_INET;                  // address family (IPv4)
    receiver.sin_port = htons(port);       // receiver port number

    if (bind(socketa, (struct sockaddr *)&receiver, sizeof(receiver)) < 0) {
        perror("takala in bind");                          // bind socket to the receiver address
        close(socketa);
        exit(EXIT_FAILURE);
    }

    if (listen(socketa, 1) < 0) {
        perror("takala in listen");                        // listen for incoming connections
        close(socketa);
        exit(EXIT_FAILURE);
    }

    printf("waiting for hibur...\n");
    int run = 1;                                    // run counter
    int sfirat_bytes = 0;                                  // total bytes received

    while (1) {
        int sender_socketa = accept(socketa, (struct sockaddr *)&sender, &sender_length);   // accept the connection
        if (sender_socketa < 0) {
            perror("takala in accept");                    // print error message if accept fails
            close(socketa);
            exit(EXIT_FAILURE);
        }

        printf("yesh hibur, receiving the file for the #%d time ...\n", run);

        FILE *file = fopen("received_file.txt", "wb");    // open file for writing received data
        if (file == NULL) {
            perror("takala in opening file");         // print error message if file opening fails
            exit(EXIT_FAILURE);
        }

        int bytes_received = 1;    // bytes received in each recv call
        sum_received_bytes = 0; // reset total bytes received for this run
        unsigned int expectedBytes = 2097200;    // expected bytes for each run
        char buffer[1024];  // buffer for receiving data

        gettimeofday(&hathala, NULL); // get start time for transfer
        while(bytes_received) {

            bytes_received = recv(sender_socketa, buffer, 1024, 0);   // Receive data

            if (bytes_received < 0) {
                perror("takala in receiving file");                   // print error message if recv fails
                exit(EXIT_FAILURE);
            }
            if (bytes_received == 0) {
                // not received data
                break;
            }

            sfirat_bytes += bytes_received;                      // update total bytes received
            sum_received_bytes += bytes_received;      // update total bytes received
            if  (sum_received_bytes >= expectedBytes) {
                // print data info for each time we send tthe file
                gettimeofday(&sof, NULL);                 // get end time for transfer
                double passed_time = ((sof.tv_sec - hathala.tv_sec) * 1000.0)  + ((sof.tv_usec - hathala.tv_usec) / 1000.0);
                double current_bandwidth = (sum_received_bytes / (1024 * 1024)) / (passed_time / 1000);
                printf(" - Run #%d finished.\n", run);     // print transfer completion message
                printf(" - Run #%d Data: Time=%.2fms; Speed=%.2fMB/s\n", run, passed_time, current_bandwidth);
                zman += passed_time;                    // update total transfer time
                run++;                                         // increase run counter
                printf("Waiting for Sender response...\n");     // wait for respond from sender if to send again
                sum_received_bytes = 0;                      // reset the sum of bytes received
                gettimeofday(&hathala, NULL);               // reset start time
            }
        }

        fclose(file);                   // close file
        close(sender_socketa);             // close client socket

        printf("Sender sent exit message.\n");     // print message when sender closed the connection
        break; // exit
    }

    double zman_memutza = zman / (run - 1);    // calculate average time of sending the file
    double bandwidth_memutza = (sfirat_bytes / (1024 * 1024)) / (zman / 1000);   // calculate average bandwidth

    printf("----------------------------------\n");
    printf("           * Statistics *         \n");
    printf("Average time: %.2fms\n", zman_memutza);          // print average transfer time
    printf("Average bandwidth: %.2fMB/s\n", bandwidth_memutza);   // print average bandwidth
    printf("----------------------------------\n");
    printf("Receiver end.\n");                          // print end of the receiver

    return 0;
}
