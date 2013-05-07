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
	nonce2 = 'ohhi'
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
		return False					
		
	# Retrieve session_key
	session_key = privateB.decrypt(pickle.loads(enc_session_key))

	# verifiy the signature matches
	if not (publicA.verify(session_key, pickle.loads(dec_session_key))):
		return False
	
	return True
	
	
def AuthenticateCustomer(atm_client):
	
	AccountId = int(atm_client.recv(CLIENT_MSG_SIZE))
	atm_client.send("Thank you")
	
	Password = atm_client.recv(CLIENT_MSG_SIZE)
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
		if ( Authenticate(atm_client) == False ):
			print "Failed to authenticate"
			return
		else:
			print "Authenticated"
		
		# Verify customer using ATM
		result = AuthenticateCustomer(atm_client)
		if ( result == False ):
			print "Failed to authenticate customer"
			return
		else:
			print "Customer has been verified"
		AccountId = int(result)
		GetCommands(AccountId, atm_client)
		
def GetCommands(AccountId, atm_client):
	while 1:
		command = atm_client.recv(CLIENT_MSG_SIZE)

		if command == 'b':
			# Get balance
			GetBalance(AccountId, atm_client)
        
		elif command == 'd':
			# Make deposit
			MakeDeposit(AccountId, atm_client)
        
		elif command == 'w':
			# Make withdrawl
			MakeWithdrawl(AccountId)
        
		elif command == 'a':
			# Get activity
			GetActivity(AccountId)
		   
		elif command == 'q':
			# Quit
			print "Quiting program."
			break
		
		else:
			print "Unrecognized command.  Please try again."
			
			
def GetBalance(AccountId, atm_client):
    
    # Display new balance
    sql = "SELECT Balance FROM ClientAccounts WHERE AccountId=?"
    cursor.execute(sql, [AccountId])
    balance = cursor.fetchone()[0]
    # print "Balance is " + locale.currency(balance)
    atm_client.send("Balance is " + locale.currency(balance))
    
def MakeDeposit(AccountId, atm_client):
    atm_client.send("thankyou")
    # Get amount
    amount = atm_client.recv(CLIENT_MSG_SIZE)
    
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
    GetBalance(AccountId, atm_client)
    
def MakeWithdrawl(AccountId, atm_client):
	atm_client.send("thankyou")
	
	# Get amount
	amount = atm_client.recv(CLIENT_MSG_SIZE)
	
	
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
	GetBalance(AccountId)
    
def GetActivity(AccountId):
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
