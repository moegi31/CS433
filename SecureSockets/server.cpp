#include <stdio.h>      /* Contains common I/O functions */
#include <sys/types.h>  /* Contains definitions of data types used in system calls */
#include <sys/socket.h> /* Includes definitions of structures needed for sockets */
#include <netinet/in.h> /* Contains constants and structures needed for internet domain addresses. */
#include <unistd.h>     /* Contains standard unix functions */
#include <stdlib.h>     /* For atoi() and exit() */
#include <string.h> 	/* For memset() */
#include <string>
#include <time.h>       /* For retrieving the time */
#include <limits.h>		/* For limits */
#include <signal.h>
#include "helper.h"     /* For helper functions */
#include "CipherInterface.h"
#include "Playfair.h"
#include "Vigenere.h"
#include "Railfence.h"
#include "Caesar.h"
#include "DES.h"
#include "RowTransposition.h"
	
using namespace std;
	
#define MAX_MESSAGE_SIZE 100000
	
int listenSocket = -1; /* File descriptor representing the listen socket */
int connectionId = -1; /* File descriptor representing file connection */

void handleShutdown(int s){
	close(connectionId);
	close(listenSocket);
	fprintf(stderr, "Handled socket clean up!\n");
	exit(1);
}
	
/* Receive message from specified socket */
void receiveMessage(const int& socket, char* message){
	int messageSize = -1; /* The message size */
	
	/* Get the size of the message */
	if((messageSize = tcp_recv_size(socket)) < 0){
		perror("tcp_recv_size");
		exit(-1);
	}
	
	/* Get the message */	
	if(tcp_recv(socket, message, messageSize) < 0){
		perror("tcp_recv");
		exit(-1);
	}
	
	/* NULL terminate the file name */
	message[messageSize] = '\0';	
}
	
int main(int argc, char** argv){
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = handleShutdown;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	int portNumber = -1; /* Port number */
	int bytesRead = 0; /* Number of bytes received */
	sockaddr_in serverAddress; /* Struct representing server address */
	sockaddr_in clientAddress; /* Struct representing client address */
	socklen_t clientLength = sizeof(clientAddress); /* Size of the client address */
	
	char message[MAX_MESSAGE_SIZE]; /* Buffer used for receiving message */
	
	/* Check number of arguments */
	if(argc < 4){
		fprintf(stderr, "USAGE: %s <LISTENING PORT> <ALG> <DECRYPTION KEY>", argv[0]); /* Report error and exit */
		exit(-1);
	}
	
	portNumber = atoi(argv[1]); /* Get the port number */
    string cipherName = argv[2];
    string key = argv[3];
	
	
	/* Check port bounds */
	if(portNumber < 0 || portNumber > 65536){
		fprintf(stderr, "Invalid port number\n");
		exit(-1);
	}
	
	fprintf(stderr, "Port  = %d\n", portNumber); /* Print the port number */
	
	/* Attempt to create socket with IPv4 and reliable TCP*/
	if((listenSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Error allocating socket\n");
		exit(-1);
	}
	
	memset(&serverAddress, 0, sizeof(serverAddress)); /* Initialize struct */
	serverAddress.sin_port = htons(portNumber); /* Convert port number to network representation */
	serverAddress.sin_family = AF_INET; /* Set the server family */
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); /* Retrieve packets from all network interfaces */
	
	/* Bind address to socket */
	if(bind(listenSocket, (sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
		perror("Error binding address to socket\n");
		exit(-1);
	}
	
	/* Listen and allow max of 100 clients */
	if(listen(listenSocket, 100) < 0){
		perror("Error listening for pending clients");
		exit(-1);
	}
	
	/* Listen to connections forever */
	while(true){
		fprintf(stderr, "Waiting for connection on port %d...\n", portNumber);
		
		/* Store client address */
		if((connectionId = accept(listenSocket, (sockaddr *)&clientAddress, &clientLength)) < 0){
			perror("Error accepting client connection\n");
			exit(-1);
		}
		
		fprintf(stderr, "Client connected!\n");
		
		bytesRead = 0; /* Reset number of bytes read */
		
		receiveMessage(connectionId, message);
        cout << message << endl;
        string inputText = message;
		
		if(cipherName == "PLF"){
            CipherInterface* cipher = new Playfair();
            cipher->setKey(key);
            string plainText = cipher->decrypt(inputText);
            cout << "Playfair Ciphertext: " << inputText << endl;
            cout << "Key: " << key << endl;
            cout << "Plaintext: " << plainText << endl;
        }else if(cipherName == "RTS"){
            CipherInterface* cipher = new RowTransposition();
            cipher->setKey(key);
            string plainText = cipher->decrypt(inputText);
            cout << "Row Transposition Ciphertext: " << inputText << endl;
            cout << "Key: " << key << endl;
            cout << "Plaintext: " << plainText << endl;
        }else if(cipherName == "RFC"){
            CipherInterface* cipher = new Railfence();
            cipher->setKey(key);
            string plainText = cipher->decrypt(inputText);
            cout << "Railfence Ciphertext: " << inputText << endl;
            cout << "Key: " << key << endl;
            cout << "Plaintext: " << plainText << endl;
        }else if(cipherName == "VIG"){
            CipherInterface* cipher = new Vigenere();
            cipher->setKey(key);
            string plainText = cipher->decrypt(inputText);
            cout << "Vigenere Ciphertext: " << inputText << endl;
            cout << "Key: " << key << endl;
            cout << "Plaintext: " << plainText << endl;
        }else if(cipherName == "CES"){
            CipherInterface* cipher = new Caesar();
            cipher->setKey(key);
            string plainText = cipher->decrypt(inputText);
            cout << "Caesar Ciphertext: " << inputText << endl;
            cout << "Key: " << key << endl;
            cout << "Plaintext: " << plainText << endl;
        }else if(cipherName == "DES"){
            CipherInterface* cipher = new DES();
            cipher->setKey(key);
            string plainText = "";
            string desBlock = "";
            for(int i = 0; i < inputText.length(); i++){
                desBlock += inputText[i];
                if((i + 1) % 8 == 0){
                    plainText += cipher->decrypt(desBlock);
                    desBlock = "";
                }
            }
            
            cout << "DES Ciphertext: " << inputText << endl;
            cout << "Key: " << key << endl;
            cout << "Plaintext: " << plainText << endl;
        }else{
            cout << "'" << cipherName << "' is an invalid cipher type." << endl;
            cout << "PLF = Playfair" << endl;
            cout << "RTS = Row Transposition" << endl;
            cout << "RFC = Railfence" << endl;
            cout << "VIG = Vigenre" << endl;
            cout << "CES = Caesar" << endl;
            return 1;
        }
		
		//fprintf(stderr, "Received Message: %s\n", plainText);				
		
		fprintf(stderr, "Disconnecting from client...\n");
		close(connectionId); /* Close socket */
		fprintf(stderr, "Disconnected from client!\n");
	}
	
	return 0;
}
