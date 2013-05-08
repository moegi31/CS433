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
import os
filepath = os.getcwd()

# Password prompt
from getpass import getpass

# Symmetric encryption
from pyDes import *

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
		filename = filepath + "/atmKeys/atm" + atmid + ".prv"
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
		filename = filepath + "/atmKeys/bank.pub"
		f = open(filename)
	except IOError as e:
		print e
		raise
	else:
		key = RSA.importKey(f.read())
		f.close()
	return key
	
def AuthenticateServer():
	# Identify to the server the atmid
	clientSocket.send(sys.argv[3])
	clientSocket.recv(SERVER_MSG_SIZE)
	
	# Send nonce1
	nonce1 = Random.get_random_bytes(100)
	enc_nonce1 = publicB.encrypt(nonce1, None)
	
	clientSocket.send(pickle.dumps(enc_nonce1))
	
	# Recieve nonce1 back	
	dec_ver_nonce1 = clientSocket.recv(SERVER_MSG_SIZE)
	fas = pickle.loads(dec_ver_nonce1)
	ver_nonce1 = privateA.decrypt(fas)
	
	if nonce1 != ver_nonce1:
		print "nonce1 is wrong!"
		authenticated = False
		
	# Retrieve nonce2
	enc_nonce2 = clientSocket.recv(SERVER_MSG_SIZE)
	fas = pickle.loads(enc_nonce2)
	dec_nonce2 = privateA.decrypt(fas)
	
	# Encrypt nonce2 and sent it back
	ver_nonce2 = publicB.encrypt(dec_nonce2, None)	
	clientSocket.send(pickle.dumps(ver_nonce2))
	clientSocket.recv(SERVER_MSG_SIZE)
	
	# Send session key
	# Randomly generated 24 byte key  
	session_key = Random.get_random_bytes(24)
	enc_session_key = publicB.encrypt(session_key, None)
	clientSocket.send(pickle.dumps(enc_session_key))
	clientSocket.recv(SERVER_MSG_SIZE)
	
	# And a signature with it
	signed_session_key = privateA.sign(session_key, None)
	#print signed_session_key
	clientSocket.send(pickle.dumps(signed_session_key))
	
	return session_key	

def AuthenticateCustomer(session_key):
	# Get account
	AccountId = raw_input("User ID(6 digit ID): ")

	# make sure input has only digits and has six digits
	while (not AccountId.isdigit()) or (len(AccountId) != 6 ):
		print "Please make sure your user id is 6 digits and only contains numbers"
		AccountId = raw_input("User ID(6 digit ID): ")
	
	# send account id
	clientSocket.send(triple_des(session_key).encrypt(AccountId, padmode=2))
	clientSocket.recv(SERVER_MSG_SIZE)
	
	# get password
	Password = getpass()
	# send password
	clientSocket.send(triple_des(session_key).encrypt(Password, padmode=2))
	clientSocket.recv(SERVER_MSG_SIZE)
	
def GetCommands(session_key):
	
	print "\nPlease select from the following options:\n"+\
	"[B] display the current balance of the account\n"+\
	"[D] deposit money\n"+\
	"[W] withdrawals\n"+\
	"[A] account activities\n"+\
	"[Q] quit\n"	    
		
	while True:
		# Prompt to accept and send commands to server
		command = raw_input("prompt> ")
		
		command = command.lower()
		
		clientSocket.send(triple_des(session_key).encrypt(command, padmode=2))
		serverResponse = clientSocket.recv(SERVER_MSG_SIZE)
		serverResponse = triple_des(session_key).decrypt(serverResponse, padmode=2)
		
		if command == "b":
			print serverResponse
		
		if command == "d":
			command = raw_input("Enter amount> ")
			clientSocket.send(command)
			serverResponse = clientSocket.recv(SERVER_MSG_SIZE)
			print serverResponse
			
		if command == "w":
			command = raw_input("Enter amount> ")
			clientSocket.send(command)
			serverResponse = clientSocket.recv(SERVER_MSG_SIZE)
			print serverResponse
						
		if command == "a":
			activity = pickle.loads(serverResponse)
			for entry in activity:
				print entry[0], entry[1], entry[2], entry[3], "\n"
		
		if command == "q":
			break
    		    
    

# define main function
if __name__ == "__main__":
	# Check the command line arguments
	if len(sys.argv) != 4:
		print "USAGE: ", sys.argv[0], " <SERVER IP> <SERVER PORT> <ATM ID> "
		exit(0)		
	
	# import key from private key dir
	privateA = import_priv_atmkey(sys.argv[3])
	
	# import bank public key	
	publicB = import_pub_bankkey()
		
	# create communication socket
	clientSocket = create_socket()
	
	session_key = AuthenticateServer()
	
	AuthenticateCustomer(session_key)
	
	GetCommands(session_key)
		
	clientSocket.close() 
