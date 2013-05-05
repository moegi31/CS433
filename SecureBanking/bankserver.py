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
	
def Authenticate(client):
	# This variable starts out as true is set to false if error occurs
	authenticated = True 
	
	# Receive Nonce1
	enc_nonce1 = client.recv(CLIENT_MSG_SIZE)
	fas = pickle.loads(enc_nonce1) 
	dec_nonce1 = privateB.decrypt(fas)

	# Send back Nonce1 and Nonce2
	nonce2 = 'ohhi'
	ver_nonce1 = publicA.encrypt(dec_nonce1, None)
	enc_nonce2 = publicA.encrypt(nonce2, None)

	client.send(pickle.dumps(ver_nonce1))
	client.send(pickle.dumps(enc_nonce2))

	# Receive Nonce2
	ver_nonce2 = client.recv(CLIENT_MSG_SIZE)
	client.send("Thank you")
	enc_session_key = client.recv(CLIENT_MSG_SIZE)
	client.send("Thank you")
	dec_session_key = client.recv(CLIENT_MSG_SIZE)
			
	# commet here
	dec_ver_nonce2 = privateB.decrypt(pickle.loads(ver_nonce2))

	#print dec_ver_nonce2

	if dec_ver_nonce2 != nonce2:
		authenticated = False					
		
	# Retrieve session_key
	session_key = privateB.decrypt(pickle.loads(enc_session_key))

	# verifiy the signature matches
	if (publicA.verify(session_key, pickle.loads(dec_session_key))):
		print "Session key established!"	
	else:
		authenticated = False
	
	return authenticated
		

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
		
		authenticated = Authenticate(client)
			
		if ( authenticated == False ):
			print "Failed to authenticate"
		else:
			print "Authenticated"
			
		
		# Close the connection to the client
		client.close()
