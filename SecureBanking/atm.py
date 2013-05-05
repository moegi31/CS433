# includes from Python's cryptography library
from Crypto.PublicKey import RSA 
from Crypto import Random
from Crypto.Hash import SHA256

# Sending crypto across the wire
import pickle

# includes for sockets
import socket 
import sys

# password retrieval
import getpass

# expression modifier to ensure proper input
import re

# defined values
SERVER_MSG_SIZE = 2048

def create_socket():
	# Get the host name (or IP)
	host = sys.argv[1]
	# Get the server's port number 
	port = int(sys.argv[2])
	# The size of the message sent by server
	# The client socket
	mSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
	# Connect to the server 
	mSocket.connect((host,port)) 	
	return mSocket

def import_priv_atmkey(atmid):
	try:
		filename = "keyBPrivate.prv"
		f = open(filename)
	except IOError as e:
		print e
		raise
	else:
		key = RSA.importKey(f.read())
		f.close()
	return key

def import_pub_bankkey():
	try:
		filename = "keyAPublic.pub"
		f = open(filename)
	except IOError as e:
		print e
		raise
	else:
		key = RSA.importKey(f.read())
		f.close()
	return key

# define main function
if __name__ == "__main__":
	# Check the command line arguments
	if len(sys.argv) != 3:
		print "USAGE: ", sys.argv[0], " <SERVER IP> <SERVER PORT> "
		exit(0)		
		
	# This variable starts out as true is set to false if error occurs
	authenticated = True
	
	# import key from private key dir
	privateA = import_priv_atmkey(1)
	
	# import bank public key	
	publicB = import_pub_bankkey()
		
	# create communication socket
	clientSocket = create_socket()
	
	# Send nonce1
	nonce1 = 'helloworld'
	enc_nonce1 = publicB.encrypt(nonce1, None)
	
	clientSocket.send(pickle.dumps(enc_nonce1))
	
	# Recieve nonce1 back	
	dec_ver_nonce1 = clientSocket.recv(SERVER_MSG_SIZE)
	fas = pickle.loads(dec_ver_nonce1)
	ver_nonce1 = privateA.decrypt(fas)
	
	print ver_nonce1
	
	if nonce1 != ver_nonce1:
		print "nonce1 is wrong!"
		authenticated = False
	else:
		print "nonce1 is correct!"
		
	# Retrieve nonce2
	enc_nonce2 = clientSocket.recv(SERVER_MSG_SIZE)
	fas = pickle.loads(enc_nonce2)
	dec_nonce2 = privateA.decrypt(fas)
	
	# used for debugging
	print dec_nonce2
	
	# Encrypt nonce2 and sent it back
	ver_nonce2 = publicB.encrypt(dec_nonce2, None)	
	clientSocket.send(pickle.dumps(ver_nonce2))
	clientSocket.recv(SERVER_MSG_SIZE)
	
	# Send session key  
	session_key = '123'
	enc_session_key = publicB.encrypt(session_key, None)
	clientSocket.send(pickle.dumps(enc_session_key))
	clientSocket.recv(SERVER_MSG_SIZE)
	
	# And a signature with it
	signed_session_key = privateA.sign(session_key, None)
	#print signed_session_key
	clientSocket.send(pickle.dumps(signed_session_key))	

		
	clientSocket.close() 
