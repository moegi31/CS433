#ifndef HELPER_LIB
#define HELPER_LIB

#include <stdio.h>      /* Contains common I/O functions */
#include <sys/types.h>  /* Contains definitions of data types used in system calls */
#include <sys/socket.h> /* Includes definitions of structures needed for sockets */
#include <netinet/in.h> /* Contains constants and structures needed for internet domain addresses. */
#include <unistd.h>     /* Contains standard unix functions */
#include <stdlib.h>     /* For atoi() and exit() */
#include <string.h> 	/* For memset() */
#include <arpa/inet.h>  /* For inet_pton() */
#include <time.h>       /* For retrieving the time */
#include <limits.h>	/* For the maximum integer size */
#include <cstdlib>  
#include <cstddef>
#include <string>

#define SIZE_MSG_SIZE 100 /* The size of the buffer size message */

using namespace std;

int min(int first, int second);

/* Keeps transmitting a buffer until the buffer is transmitted */
int tcp_send(const int& socket, const void* buffer, const int& buffSize);


/* Keeps receiving a buffer until all bytes are received */
int tcp_recv(const int& socket, void* buffer, const int& buffSize);

/* Send size over socket */
int tcp_send_size(const int& socket, const int& size);

/* Receive size message */
int tcp_recv_size(const int& socket);

string IntToLetter(int intValue);
int LetterToInt(string letter);

#endif
