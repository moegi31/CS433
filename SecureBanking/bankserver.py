# includes from Python's cryptography library
from Crypto.PublicKey import RSA
from Crypto import Random
from Crypto.Hash import SHA256

# Sending crypto across the wire
import pickle

# socket library includes
import socket 
import sys
from time import gmtime, strftime

# The client message size
CLIENT_MSG_SIZE = 2048

# client accounts hash table
accounts = {'123456':'secret', \
			'111111':'test'}

def import_pub_atmkey(atmid):
	try:
		filename = "keyBPublic.pub"
		f = open(filename)
	except IOError as e:
		print e
		raise
	else:
		key = RSA.importKey(f.read())
		f.close()
	return key

def import_priv_bankkey():
	try:
		filename = "keyAPrivate.prv"
		f = open(filename)
	except IOError as e:
		print e
		raise
	else:
		key = RSA.importKey(f.read())
		f.close()
	return key

if __name__ == "__main__":
	# Check the command line arguments
	if len(sys.argv) != 2:
		print "USAGE: ", sys.argv[0], " <PORT> "
		exit(0)
		
	# Import known keys
	privateB = import_priv_bankkey()
	publicA = import_pub_atmkey(1)
	
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
	# Service clients forever
	while 1: 
		# Accept a connection from the client
		client, address = listenSocket.accept()
		
		# This variable starts out as true is set to false if error occurs
		authenticated = True 
		
		# Receive Nonce1
		enc_nonce1 = client.recv(CLIENT_MSG_SIZE)
		fas = pickle.loads(enc_nonce1) 
		dec_nonce1 = privateB.decrypt(fas)
		
		# Uncomment for debugging
		print dec_nonce1
		
		# Send back Nonce1 and Nonce2
		nonce2 = 'ohhi'
		ver_nonce1 = publicA.encrypt(dec_nonce1, None)
		enc_nonce2 = publicA.encrypt(nonce2, None)
		
		client.send(pickle.dumps(ver_nonce1))
		client.send(pickle.dumps(enc_nonce2))
		
		# Receive Nonce2
		ver_nonce2 = client.recv(CLIENT_MSG_SIZE)
		enc_session_key = client.recv(CLIENT_MSG_SIZE)
		dec_session_key = client.recv(CLIENT_MSG_SIZE)
				
		fas = pickle.loads(ver_nonce2)        
		dec_ver_nonce2 = privateB.decrypt(fas)
		
		print dec_ver_nonce2

		if dec_ver_nonce2 == nonce2:
			print "nonce2 was correct!"
		else:
			print "nonce2 was wrong!"
			authenticated = False
			
		# Retrieve session_key
		fas = pickle.loads(enc_session_key)  
		session_key = privateB.decrypt(fas)
		
		print session_key
		
		# Retrieve signature of session_key
		faz = pickle.loads(dec_session_key)		
		
		print faz  # For some reason I need to print here or the previous line crashes.  
		
		# verifiy the signature matches
		if publicA.verify(session_key, faz):
			print "Session key established!"
			
		else:
			authenticated = False
        
        
		
		# Close the connection to the client
		client.close()
