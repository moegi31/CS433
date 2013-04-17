# includes from Python's cryptography library
from Crypto.PublicKey import RSA
from Crypto import Random


random_generator = Random.new().read
key = RSA.generate(1024, random_generator)
key

import socket 
import sys
from time import gmtime, strftime

# Check the command line arguments
if len(sys.argv) != 2:
	print "USAGE: ", sys.argv[0], " <PORT> "
	exit(0)

# Get the port number 
port = int(sys.argv[1]) 

# The backlog
backlog = 100

# Create A TCP socket
listenSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 

# Bing the socket to the port 
listenSocket.bind(('',port)) 

# Start listening for incoming connections
listenSocket.listen(backlog) 

# The client message size
CLIENT_MSG_SIZE = 1024

# Service clients forever
while 1: 
	
	# Accept a connection from the client
	client, address = listenSocket.accept() 

	# Get the data from the client
	data = client.recv(CLIENT_MSG_SIZE) 

	print "GOT: ", data, " from the client"
	
	# Make sure the data was successfully received
	if data: 
        	
		# Get the current date and time
		dateAndTime = strftime("%a, %d %b %Y %X +0000", gmtime())
		
		# Send the date and time to the client
		client.send(dateAndTime) 
   
		# Close the connection to the client
		client.close()
