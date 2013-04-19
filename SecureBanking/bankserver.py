# includes from Python's cryptography library
from Crypto.PublicKey import RSA
from Crypto import Random
from Crypto.Hash import SHA256

# socket library includes
import socket 
import sys
from time import gmtime, strftime

# The client message size
CLIENT_MSG_SIZE = 1024

# client accounts hash table
accounts = {'123456':'secret', \
			'111111':'test'}

def import_pub_atmkey(atmid):
	try:
		filename = "pubkeys/atmpubkey"+str(atmid)+".pem"
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
		filename = "privkeys/bankkey.pem"
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
		# Get client's public key
		atmsig = client.recv(CLIENT_MSG_SIZE) 
		
		# import atm pubkey
		atmpubkey = import_pub_atmkey(1)
		
		# verify atm identity
		message = 'I can see the matrix'
		digest = SHA256.new(message).digest()
		if( atmpubkey.verify(digest, (long(atmsig), )) ):
			print "ATM Identity Verified! wunderbar..."
			# import bank private key
			bankkey = import_priv_bankkey()
			# generate signature using previous digest
			banksig = bankkey.sign (digest, None)[0]
			client.send(str(banksig))

			cipheruserdets = client.recv(CLIENT_MSG_SIZE)
			plainuserdets = bankkey.decrypt(cipheruserdets)
			useracc= plainuserdets.split()

			if useracc[0] not in accounts:
				print "Record not found. Disconnecting user..."
				client.close()
		
		else:
			# unable to verify atm identity, quit
			client.close()

		# Close the connection to the client
		client.close()
