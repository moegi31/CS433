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
	
def Authenticate(client):
	# Receive atm and set the public key
	atmID = client.recv(CLIENT_MSG_SIZE)
	client.send("Thank you")
	publicA = import_pub_atmkey(atmID)
		
	# Receive Nonce1
	enc_nonce1 = client.recv(CLIENT_MSG_SIZE)
	dec_nonce1 = privateB.decrypt(pickle.loads(enc_nonce1))

	# Send back Nonce1 and Nonce2
	nonce2 = 'ohhi'
	ver_nonce1 = publicA.encrypt(dec_nonce1, None)
	enc_nonce2 = publicA.encrypt(nonce2, None)

	client.send(pickle.dumps(ver_nonce1))
	client.send(pickle.dumps(enc_nonce2))

	# Receive nonce2, session, and sig of session
	ver_nonce2 = client.recv(CLIENT_MSG_SIZE)
	client.send("Thank you")
	enc_session_key = client.recv(CLIENT_MSG_SIZE)
	client.send("Thank you")
	dec_session_key = client.recv(CLIENT_MSG_SIZE)
			
	# Verify nonce2
	dec_ver_nonce2 = privateB.decrypt(pickle.loads(ver_nonce2))

	if dec_ver_nonce2 != nonce2:
		return False					
		
	# Retrieve session_key
	session_key = privateB.decrypt(pickle.loads(enc_session_key))

	# verifiy the signature matches
	if not (publicA.verify(session_key, pickle.loads(dec_session_key))):
		return False
	
	return True
	
	
def AuthenticateCustomer():
	
	AccountId = int(client.recv(CLIENT_MSG_SIZE))
	client.send("Thank you")
	
	Password = client.recv(CLIENT_MSG_SIZE)
	client.send("Thank you")
	
	# Query account from 
	sql = "SELECT AccountID, Password FROM ClientAccounts WHERE AccountId=?"
	cursor.execute(sql, [AccountId])
	account = cursor.fetchone()
	
	# Check to see if account exists
	if account is None:
		return False
		print "1"
	elif account[0] == AccountId and account[1] == Password:
		return True
	else:
		print "2"
		return False
		
def HandleClient():
		# Verify ATM connection	
		if ( Authenticate(client) == False ):
			print "Failed to authenticate"
			return
		else:
			print "Authenticated"
		
		# Verify customer using ATM
		if ( AuthenticateCustomer() == False ):
			print "Failed to authenticate customer"
			return
		else:
			print "Customer has been verified"
			
		 GetCommands()

def GetCommands():
	while True:
		if command == 'b':
			# Get balance
			GetBalance()
        
		elif command == 'd':
			# Make deposit
			MakeDeposit()
        
		elif command == 'w':
			# Make withdrawl
			MakeWithdrawl()
        
		elif command == 'a':
			# Get activity
			GetActivity()
		   
		elif command == 'q':
			# Quit
			print "Quiting program."
			break
		
		else:
			print "Unrecognized command.  Please try again."
			
			
def GetBalance():
    
    # Display new balance
    sql = "SELECT Balance FROM ClientAccounts WHERE AccountId=?"
    cursor.execute(sql, [AccountId])
    balance = cursor.fetchone()[0]
    print "Balance is " + locale.currency(balance)
    
def MakeDeposit():
    # Get amount
    amount = raw_input("Amount: ")
    
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
    GetBalance()
    
def MakeWithdrawl():
    # Get amount
    amount = raw_input("Amount: ")
    
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
    GetBalance()
    
def GetActivity():
	# Get transactions
    sql = "SELECT Activity, Amount, Balance, Time FROM ClientActivity WHERE AccountId=?"
    cursor.execute(sql, [AccountId])
    balance = cursor.fetchall()
    
    # Print transactions
    print string.ljust("Activity", 10), string.ljust("Amount", 10), string.ljust("Balance", 10), string.ljust("Time", 10) 
    for row in balance:
        # Make the format pretty
        time = parser.parse(row[3])
        formattedTime = time.strftime("%Y-%m-%d %H:%M")
        amount = locale.currency(row[1])
        balance1 = locale.currency(row[2])
        print string.ljust(str(row[0]), 10), string.ljust(amount, 10), string.ljust(balance1, 10) , string.ljust(formattedTime, 10) 
		

if __name__ == "__main__":
	# Check the command line arguments
	if len(sys.argv) != 2:
		print "USAGE: ", sys.argv[0], " <PORT> "
		exit(0)
<<<<<<< HEAD
	# import atm pubkey and bank privkey
	atmpubkey = import_pub_atmkey(1)
	bankkey = import_priv_bankkey()

=======
		
	# Import known keys
	privateB = import_priv_bankkey()
	
>>>>>>> e5d2fc5a0b06e625bd70e33b8efd762d5ac66621
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
<<<<<<< HEAD
		client, address = listenSocket.accept() 
		# Get client's public key
		atmsig = client.recv(CLIENT_MSG_SIZE) 

		# verify atm identity
		message = 'I can see the matrix'
		digest = SHA256.new(message).digest()

		# decrypt message with bank priv key
		atmsig = bankkey.decrypt(atmsig)
		# verify with atm pub key
		if( atmpubkey.verify(digest, (long(atmsig), )) ):
			print "ATM Identity Verified! wunderbar..."
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

=======
		client, address = listenSocket.accept()
		
		HandleClient()			
		
>>>>>>> e5d2fc5a0b06e625bd70e33b8efd762d5ac66621
		# Close the connection to the client
		client.close()
