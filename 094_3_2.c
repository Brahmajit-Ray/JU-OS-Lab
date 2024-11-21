/*
 
 Name-Brahmajit Ray
 Roll-002211001094
 2nd Sep 13:20

Question: File Transfer using sockets

Running Command:gcc 094_3_2.c
		./a.out initialFile.txt

Output:
Files match.
Time required for double transfer: 0.006171 second

*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdio.h>

#define FILE_SIZE 104  // Input size of file to be copied

void transfer_file(int socket_fd, int fd) {
    char buffer[FILE_SIZE];
    ssize_t bytes_read;
    ssize_t bytes_written;
    ssize_t total_bytes_written = 0;

    bytes_read = read(fd, buffer, FILE_SIZE);
    if (bytes_read < 0) {
        perror("Error reading file");
        exit(EXIT_FAILURE);
    }

        while (total_bytes_written < bytes_read) {
        bytes_written = write(socket_fd, buffer + total_bytes_written, bytes_read - total_bytes_written);
        if (bytes_written < 0) {
            perror("Error writing to socket");
            exit(EXIT_FAILURE);
        }
        total_bytes_written += bytes_written;
    }
}

void receive_file(int socket_fd, const char *file_name) {
    char buffer[FILE_SIZE];
    ssize_t bytes_read;
    ssize_t total_bytes_read = 0;

        while (total_bytes_read < FILE_SIZE) {
        bytes_read = read(socket_fd, buffer + total_bytes_read, FILE_SIZE - total_bytes_read);
        if (bytes_read < 0) {
            perror("Error reading from socket");
            exit(EXIT_FAILURE);
        }
        if (bytes_read == 0) {
            break; // End of transmission
        }
        total_bytes_read += bytes_read;
    }

    int fd = open(file_name, O_WRONLY);
    if (fd < 0) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_written = write(fd, buffer, total_bytes_read);
    if (bytes_written < 0) {
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }

    close(fd);
}

int compare_files(const char *file1, const char *file2) {
    char buffer1[FILE_SIZE], buffer2[FILE_SIZE];
    int fd1 = open(file1, O_RDONLY);
    int fd2 = open(file2, O_RDONLY);

    if (fd1 < 0 || fd2 < 0) {
        perror("Error opening files for comparison");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read1 = read(fd1, buffer1, FILE_SIZE);
    ssize_t bytes_read2 = read(fd2, buffer2, FILE_SIZE);

    close(fd1);
    close(fd2);

    return (bytes_read1 == bytes_read2) && (memcmp(buffer1, buffer2, bytes_read1) == 0);
}

int main(int argc, char *argv[]) {
   if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_to_transfer>\n", argv[0]);
        exit(EXIT_FAILURE);
   }

    int socket_fds[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, socket_fds) < 0) {
        perror("Error creating socket pair");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Error forking");
        exit(EXIT_FAILURE);
    }

    struct timeval start, end;

    if (pid > 0) { // Parent process
        close(socket_fds[1]);

        int fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            perror("Error opening file...Parent Process");
            exit(EXIT_FAILURE);
        }

        gettimeofday(&start, NULL);
        transfer_file(socket_fds[0], fd);
        receive_file(socket_fds[0], "received_from_child.txt");
        gettimeofday(&end, NULL);

        close(fd);
        close(socket_fds[0]);

        if (compare_files(argv[1], "received_from_child.txt")) {
            printf("Files match.\n");
        } else {
            printf("Files do not match.\n");
        }

        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
        double elapsed = seconds + microseconds * 1e-6;
        printf("Time required for double transfer: %.6f seconds\n", elapsed);

    } else { // Child process
        close(socket_fds[0]);

        receive_file(socket_fds[1], "received_from_parent.txt");
        int fd = open("received_from_parent.txt", O_RDONLY);
        if (fd < 0) {
            perror("Error opening received file");
            exit(EXIT_FAILURE);
        }

        transfer_file(socket_fds[1], fd);
        close(fd);
        close(socket_fds[1]);

        exit(EXIT_SUCCESS);
    }

    return 0;
}
