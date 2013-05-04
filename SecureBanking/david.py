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

import string

AccountId = 123456

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
    
    #print "Account activity is empty"  
    

# Bank server prompt
while True:

    # Prompt to accept and send commands to server
    command = raw_input("prompt> ")
    
    command = command.lower()
    
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
