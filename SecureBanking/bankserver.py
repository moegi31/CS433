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

# Database imports and variables
import sqlite3
conn = sqlite3.connect("BankDatabase.sqlite")
cursor = conn.cursor()

# Currency formatting
import locale
locale.setlocale( locale.LC_ALL, '' )

# Datetime
import datetime
from dateutil import parser

import os
filepath = os.getcwd()

import string

# includes for Threading
from multiprocessing import Process
MAX_ATM_THREADS = 2

# Symmetric encryption
from pyDes import *


def import_pub_atmkey(atmid):
	try:
		filename = filepath + "/bankKeys/atm" + atmid + ".pub"
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
		filename = filepath + "/bankKeys/bank.prv"
		f = open(filename)
	except IOError as e:
		print e
		raise
	else:
		key = RSA.importKey(f.read())
		f.close()
	return key
	
def Authenticate(atm_client):
	# Receive atm and set the public key
	atmID = atm_client.recv(CLIENT_MSG_SIZE)
	atm_client.send("Thank you")
	publicA = import_pub_atmkey(atmID)
		
	# Receive Nonce1
	enc_nonce1 = atm_client.recv(CLIENT_MSG_SIZE)
	dec_nonce1 = privateB.decrypt(pickle.loads(enc_nonce1))

	# Send back Nonce1 and Nonce2
	nonce2 = Random.get_random_bytes(100)
	ver_nonce1 = publicA.encrypt(dec_nonce1, None)
	enc_nonce2 = publicA.encrypt(nonce2, None)

	atm_client.send(pickle.dumps(ver_nonce1))
	atm_client.send(pickle.dumps(enc_nonce2))

	# Receive nonce2, session, and sig of session
	ver_nonce2 = atm_client.recv(CLIENT_MSG_SIZE)
	atm_client.send("Thank you")
	enc_session_key = atm_client.recv(CLIENT_MSG_SIZE)
	atm_client.send("Thank you")
	dec_session_key = atm_client.recv(CLIENT_MSG_SIZE)
			
	# Verify nonce2
	dec_ver_nonce2 = privateB.decrypt(pickle.loads(ver_nonce2))

	if dec_ver_nonce2 != nonce2:
		return False, atmID					
		
	# Retrieve session_key
	session_key = privateB.decrypt(pickle.loads(enc_session_key))

	# verifiy the signature matches
	if not (publicA.verify(session_key, pickle.loads(dec_session_key))):
		return False, atmID
	
	print "ATM " + atmID + " has connected." 
	
	return session_key, atmID
	
	
def AuthenticateCustomer(atm_client, session_key):
	AccountId = atm_client.recv(CLIENT_MSG_SIZE)
	AccountId = int(triple_des(session_key).decrypt(AccountId, padmode=2))
	atm_client.send("Thank you")
	
	Password = atm_client.recv(CLIENT_MSG_SIZE)
	Password = triple_des(session_key).decrypt(Password, padmode=2)
	atm_client.send("Thank you")
	
	# Query account from 
	sql = "SELECT AccountID, Password FROM ClientAccounts WHERE AccountId=?"
	cursor.execute(sql, [AccountId])
	account = cursor.fetchone()
	
	# Check to see if account exists
	if account is None:
		return False
		print "1"
	elif account[0] == AccountId and account[1] == Password:
		return AccountId
	else:
		print "2"
		return False
		
def HandleClient(atm_client):
		# Verify ATM connection
		session_key, atmID = Authenticate(atm_client)
		if ( session_key == False ):
			print "Failed to authenticate"
			return
		
		# Verify customer using ATM
		result = AuthenticateCustomer(atm_client, session_key)
		if ( result == False ):
			print "Failed to authenticate customer"
			return
		else:
			print "Client " + str(result) + " has signed in."
		AccountId = int(result)
		
		# Loop until client asks to disconnet
		GetCommands(AccountId, atm_client, session_key)
		
		# Logging messages
		print "Client " + result + " has signed out."
		print "ATM " + int(atmID) + " has disconnected."
		
def GetCommands(AccountId, atm_client, session_key):
	while 1:
		# Receive encrypted command from client and decrypt it.  
		command = atm_client.recv(CLIENT_MSG_SIZE)
		command = triple_des(session_key).decrypt(command, padmode=2)
		
		# Process the command accordingly
		if command == 'b':
			# Get balance
			print "#" + str(AccountId) + " - balance request."
			GetBalance(AccountId, atm_client, session_key)
        
		elif command == 'd':
			# Make deposit
			print "#" + str(AccountId) + " - deposit request."
			MakeDeposit(AccountId, atm_client, session_key)
        
		elif command == 'w':
			# Make withdrawl
			print "#" + str(AccountId) + " - withdrawl request."
			MakeWithdrawl(AccountId, atm_client, session_key)
        
		elif command == 'a':
			# Get activity
			print "#" + str(AccountId) + " - activity request."
			GetActivity(AccountId, atm_client, session_key)
		   
		elif command == 'q':
			# Quit
			break
		
		else:
			print "#" + str(AccountId) + " - unrecognized request."
			
			
def GetBalance(AccountId, atm_client, session_key):
    
    # Get the current balance from the server
    sql = "SELECT Balance FROM ClientAccounts WHERE AccountId=?"
    cursor.execute(sql, [AccountId])
    balance = cursor.fetchone()[0]
    
    # Return the balance to the client
    balanceMessage = "Balance is " + locale.currency(balance)
    atm_client.send(triple_des(session_key).encrypt(balanceMessage, padmode=2))
    
def MakeDeposit(AccountId, atm_client, session_key):
	# Need to send a message so we can be able to read the amount
	atm_client.send(triple_des(session_key).encrypt('Send amount', padmode=2))
	
	# Get amount
	amount = atm_client.recv(CLIENT_MSG_SIZE)
	amount = triple_des(session_key).decrypt(amount, padmode=2)

	# Update balance in database
	sql = "Update ClientAccounts Set Balance = (Balance + ?) WHERE AccountId=?"
	cursor.execute(sql, [amount, AccountId])
	conn.commit()
	
	# Get new balance value
	sql = "SELECT Balance FROM ClientAccounts WHERE AccountId=?"
	cursor.execute(sql, [AccountId])
	balance = cursor.fetchone()[0]

	# Insert activity in log
	sql = "Insert into ClientActivity (AccountId, Activity, Amount, Time, Balance) values ( ?, ?, ?, ?, ?) "
	cursor.execute(sql, [AccountId, "Deposit", amount, datetime.datetime.now(), balance])
	conn.commit()  

	# Display new balance
	GetBalance(AccountId, atm_client, session_key)
    
def MakeWithdrawl(AccountId, atm_client, session_key):
	# Need to send a message so we can be able to read the amount
	atm_client.send(triple_des(session_key).encrypt('Send amount', padmode=2))
	
	# Get amount  
	amount = atm_client.recv(CLIENT_MSG_SIZE)
	amount = triple_des(session_key).decrypt(amount, padmode=2)
	
	# Update balance in database
	sql = "Update ClientAccounts Set Balance = (Balance - ?) WHERE AccountId=?"
	cursor.execute(sql, [amount, AccountId])
	conn.commit()

	# Get new balance value
	sql = "SELECT Balance FROM ClientAccounts WHERE AccountId=?"
	cursor.execute(sql, [AccountId])
	balance = cursor.fetchone()[0]    

	# Insert activity in log
	sql = "Insert into ClientActivity (AccountId, Activity, Amount, Time, Balance) values ( ?, ?, ?, ?, ?) "
	cursor.execute(sql, [AccountId, "Withdrawl", amount, datetime.datetime.now(), balance])
	conn.commit()    

	# Display new balance
	GetBalance(AccountId, atm_client, session_key)
    
def GetActivity(AccountId, atm_client, session_key):
	# Get transactions
    sql = "SELECT Activity, Amount, Balance, Time FROM ClientActivity WHERE AccountId=?"
    cursor.execute(sql, [AccountId])
    balance = cursor.fetchall()
    
    # An empty list where transactions will be kept
    activity = []
    
    # Column headers for transactions
    activity.append((string.ljust("Activity", 10), string.ljust("Amount", 10), string.ljust("Balance", 10), string.ljust("Time", 10)))
    
    # Iterate through sql response and build list
    for row in balance:
        # Make the format pretty
        time = parser.parse(row[3])
        formattedTime = time.strftime("%Y-%m-%d %H:%M")
        amount = locale.currency(row[1])
        balance1 = locale.currency(row[2])
        activity.append((string.ljust(str(row[0]), 10), string.ljust(amount, 10), string.ljust(balance1, 10) , string.ljust(formattedTime, 10)))
    
    # In order to send the list though TCP, it should be converted to a string.
    # We can do this by pickling the list. This basically turns it into a string
    # the client can reconstruct.
    activity = pickle.dumps(activity)
    atm_client.send(triple_des(session_key).encrypt(activity, padmode=2))
    
		

if __name__ == "__main__":
	# Check the command line arguments
	if len(sys.argv) != 2:
		print "USAGE: ", sys.argv[0], " <PORT> "
		exit(0)
		
	# Import known keys
	privateB = import_priv_bankkey()
	
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
		
		HandleClient(client)			
		
		# Close the connection to the client
		client.close()
