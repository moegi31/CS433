# includes from Python's cryptography library
from Crypto.PublicKey import RSA 
from Crypto import Random


random_generator = Random.new().read
key = RSA.generate(1024, random_generator)

pubkey= key.publickey().exportKey()
print "length of public key "+str(len(pubkey))
import socket 
import sys

# Check the command line arguments
if len(sys.argv) != 3:
	print "USAGE: ", sys.argv[0], " <SERVER IP> <SERVER PORT> "
	exit(0)

# Get the host name (or IP)
host = sys.argv[1]

# Get the server's port number 
port = int(sys.argv[2])

# The size of the message sent by server
SERVER_MSG_SIZE = 1024

# The client socket
clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 

# Connect to the server */
clientSocket.connect((host,port)) 

# Send a string to the server
clientSocket.send(pubkey) 

# Get the date from the server
data = clientSocket.recv(SERVER_MSG_SIZE) 

print 'Received:', data, ' from the server'

# Close the connection to the server
clientSocket.close() 


