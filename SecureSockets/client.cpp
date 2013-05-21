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
#include <sys/stat.h>   /* For stat() */
//#include <sys/sendfile.h> /* For sendfile() */
#include <fcntl.h>        /* For O_RDONLY */
#include <string>
#include <iostream>
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

void sendMessage(const int& socket, const char* message){
	int messageSize = strlen(message); /* Get the size of the message */
	
	/* Send the size of the message */
	if(tcp_send_size(socket, messageSize) < 0){
		perror("tcp_send_size");
		exit(-1);
	}
	
	/* Send the actual message */	
	if(tcp_send(socket, message, messageSize) < 0){
		perror("tcp_send");
		exit(-1);
	}
}

int main(int argc, char** argv){
	int portNumber = -1; /* Port number */
	int connectionId = -1; /* File descriptor representing file connection */
	int bytesSent = 0; /* Number of bytes sent in one shot */
	off_t totalBytesSent = 0; /* Total number of bytes sent */
	sockaddr_in serverAddress; /* Struct representing server address */
	socklen_t serverLength = sizeof(serverAddress); /* Size of the server address */
	
	/* Check number of arguments */
	if(argc < 5){
		fprintf(stderr, "USAGE: %s <SERVER IP> <SERVER PORT #> <ALG> <ENCRYPTION KEY>", argv[0]); /* Report error and exit */
		exit(-1);
	}
	
	portNumber = atoi(argv[2]); /* Get the port number */
    string cipherName = argv[3];
    string key = argv[4];
	
	/* Check port bounds */
	if(portNumber < 0 || portNumber > 65536){
		fprintf(stderr, "Invalid port number\n");
		exit(-1);
	}
	
	/* Attempt to create socket with IPv4 and reliable TCP*/
	if((connectionId = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Error allocating socket\n");
		exit(-1);
	}
	serverAddress.sin_port = htons(portNumber); /* Convert port number to network representation */
	serverAddress.sin_family = AF_INET; /* Set the server family */
	
	/* Convert from string address to struct */
	if(!inet_pton(AF_INET, argv[1], &serverAddress.sin_addr)){
		perror("Error translating server address with inet_pton\n");
		exit(-1);
	}
	
	/* Connect to server and return socket for communication */
	if(connect(connectionId, (sockaddr *)&serverAddress, sizeof(sockaddr)) < 0){
		perror("Error connecting to server");
		exit(-1);
	}
	
	string inputText;
	fprintf(stderr, "Input string to encrypt: ");
	getline(cin, inputText);
    
    string cipherText;
	if(cipherName == "PLF"){
        CipherInterface* cipher = new Playfair();
        cipher->setKey(key);
        cipherText = cipher->encrypt(inputText);
        cout << "Plaintext: " << inputText << endl;
        cout << "Key: " << key << endl;
        cout << "Playfair Ciphertext: " << cipherText << endl;
    }else if(cipherName == "RTS"){
        CipherInterface* cipher = new RowTransposition();
        cipher->setKey(key);
        cipherText = cipher->encrypt(inputText);
        cout << "Plaintext: " << inputText << endl;
        cout << "Key: " << key << endl;
        cout << "Row Transposition Ciphertext: " << cipherText << endl;
    }else if(cipherName == "RFC"){
        CipherInterface* cipher = new Railfence();
        cipher->setKey(key);
        cipherText = cipher->encrypt(inputText);
        cout << "Plaintext: " << inputText << endl;
        cout << "Key: " << key << endl;
        cout << "Railfence Ciphertext: " << cipherText << endl;
    }else if(cipherName == "VIG"){
        CipherInterface* cipher = new Vigenere();
        cipher->setKey(key);
        cipherText = cipher->encrypt(inputText);
        cout << "Plaintext: " << inputText << endl;
        cout << "Key: " << key << endl;
        cout << "Vigenere Ciphertext: " << cipherText << endl;
    }else if(cipherName == "CES"){
        CipherInterface* cipher = new Caesar();
        cipher->setKey(key);
        cipherText = cipher->encrypt(inputText);
        cout << "Plaintext: " << inputText << endl;
        cout << "Key: " << key << endl;
        cout << "Caesar Ciphertext: " << cipherText << endl;
    }else if(cipherName == "DES"){
        CipherInterface* cipher = new DES();
        cipher->setKey(key);
        string desBlock = "";
        for(int i = 0; i < inputText.length(); i++){
            desBlock += inputText[i];
            if((i + 1) % 8 == 0){
                cipherText += cipher->encrypt(desBlock);
                cipherText = cipherText.substr(0, cipherText.size()-1);
                desBlock = "";
            }
        }
        if(desBlock.length() < 8 && desBlock != ""){
            for(int i = desBlock.length(); i < 8; i++){
                desBlock += " ";
            }
            cout << desBlock << endl;
            cipherText += cipher->encrypt(desBlock);
        }
        
        //cipherText = cipher->encrypt(inputText);
        cout << "Plaintext: " << inputText << endl;
        cout << "Key: " << key << endl;
        cout << "DES Ciphertext: " << cipherText << endl;
    }else{
        cout << "'" << cipherName << "' is an invalid cipher type." << endl;
        cout << "PLF = Playfair" << endl;
        cout << "RTS = Row Transposition" << endl;
        cout << "RFC = Railfence" << endl;
        cout << "VIG = Vigenre" << endl;
        cout << "CES = Caesar" << endl;
        return 1;
    }	
	
	fprintf(stderr, "Cipher Text = %s\n", cipherText.c_str()); /* Print the port number */
    sendMessage(connectionId, cipherText.c_str());	    
	
	fprintf(stderr, "Disconnecting from server...\n");
	close(connectionId); /* Close socket */
	fprintf(stderr, "Disconnected from server!\n");
	
	return 1;
}
