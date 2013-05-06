# includes from Python's cryptography library
from Crypto.PublicKey import RSA 
from Crypto import Random
from Crypto.Hash import SHA256

# includes for sockets
import socket 
import sys

# password retrieval
import getpass

# expression modifier to ensure proper input
import re

# defined values
SERVER_MSG_SIZE = 1024

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
		filename = "privkeys/atmkey"+str(atmid)+".pem"
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
		filename = "pubkeys/bankkey.pem"
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

	# import key from private key dir
	atmkey = import_priv_atmkey(1)	
	# import bank public key
	bankpubkey = import_pub_bankkey()
	
	# create communication socket
	clientSocket = create_socket()	
	
	# generate atm signature
	message = 'I can see the matrix'
	digest = SHA256.new(message).digest()
	atmsignature = atmkey.sign( digest, None)[0]
	
	# TODO encrypt again with bank's public key and negotiate a session key
	# send signature
	clientSocket.send(str(atmsignature))
	
	# get user ID
	print "Welcome to SecureBanking\n"
	userid = raw_input("User ID(6 digit ID): ")
	
	# make sure input has only digits and has six digits
	while (not userid.isdigit()) or (len(userid) != 6 ):
		print "Please make sure your user id is 6 digits and only contains digits"
		userid = raw_input("User ID(6 digit ID): ")
	
	# get user password
	passwd = getpass.getpass("enter password: ") 
		

	# receive bank signature
	banksig = clientSocket.recv(SERVER_MSG_SIZE)
	
	# bank identity is okay
	if( bankpubkey.verify(digest, (long(banksig), )) ):
		print "\nBank Identity Verified!"
		# send userid/passwd
		cipheruserdets = bankpubkey.encrypt(userid+" "+passwd, None)[0]
		clientSocket.send(cipheruserdets)
		
		while (1):
			print "\nPlease select from the following options:\n"+\
			"[B] display the current balance of the account\n"+\
			"[D] deposit money\n"+\
			"[W] withdrawals\n"+\
			"[A] account activities\n"+\
			"[Q] quit\n"

			option = raw_input("Please enter selection as single letter: ").upper()
			while (len(option) !=1) or (not re.match("[ABDWQ]", option)):
				print "Please enter option as single letter (either A,B,D,W or Q)"
				option = raw_input("Please enter selection as single letter: ").upper() 
			
			if(option == 'Q'):
				break;
				
	else: # something is wrong with bank server
		clientSocket.close()
		print "Error validating Bank Server Identity, quiting..."
	# Close the connection to the server
	clientSocket.close() 
	

