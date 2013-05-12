#### Name: Chris Gudea
#### Email: pulala@gmail.com
#### Partners: David Robison
#### Email: davidrobison122@gmail.com
#### Language: Python 2.7.3

#### Compilation and Running
To build, simply run

    make

To remove binary and compilation files

    make clean

Running server

	$ python bankserver.py  <PORT> 

where

* PORT: the port to have ATM clients connect on

Running client

	$ python atm.py  <SERVER IP> <SERVER PORT> <ATM ID> 

where

* SERVER IP: ip of the server (eg. 127.0.0.1)
* SERVER PORT: the server port to connect on (eg. 1234)
* ATM ID: either 1 or 2 (identifies ATM and which key to use)

#### Notes
This program has dependencies on Python version 2.5+. Some of the libraries
used are deprecated as of Python 3.0+. Python version 2.7.3 was used on the test
machine for compliation and running.

#### Extra Credit not implemented

# Secure Banking 
Implement the authentication and secure communications protocols for a distributed 
system consisting of a bank server and a number of automatic teller machines (ATMs). 
Assume one bank server and 2 ATMs. The bank should be implemented as a concurrent server

Public key cryptography is employed for secrecy, integrity-protection, and 
authentication. The bank server’s public key is stored by the ATM, with the 
corresponding private key stored by server the connected to that ATM. You may 
also assume that the bank has the public keys of the two ATMs.

ATM would work like this:

1. The customer enters his/her 6-digit ID (e.g. 124356) and password.
2. The ATM contacts the bank server to verify the customers ID and password.
3. The customer selects an action to be performed, and that action is performed by the bank server.

There are five actions: display the amount of money in the account, deposit 
money, withdrawals, account activities (time and date when the user performed 
transactions and what transactions the user performs), and quit. Assume that 
the bank knows the ATMs public key and the ATM knows the banks public key. 
The ATM communicates with the bank by running a protocol that satisfies the 
following requirements:

1. It authenticates the customer to the bank server (ID, passwd).
2. It authenticates the ATM to the bank server through public-key encryption.
3. It preserves the confidentiality of communications between the bank and ATM.
4. The bank server takes actions in response to customer uses of an ATM.

