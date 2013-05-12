CS433
=====
<i>Repository for Computer Science 433: Computer Security and Encryption</i>


### <i>Final Project: </i>Secure Banking:
-----------

Implement the authentication and secure communications protocols for a distributed system consisting of a bank server and a number of automatic teller machines (ATMs). Assume one bank server and 2 ATMs. The bank should be implemented as a concurrent server Public key cryptography is employed for secrecy, integrity-protection, and authentication. The bank server’s public key is stored by the ATM, with the corresponding private key stored by server the connected to that ATM. You may also assume that the bank has the public keys of the two ATMs. ATM would work like this:

1. The customer enters his/her 6-digit ID (e.g. 124356) and password.
2. The ATM contacts the bank server to verify the customers ID and password.
3. The customer selects an action to be performed, and that action is performed by the bank server.

There are five actions: display the amount of money in the account, deposit money, withdrawals, account activities (time and date when the user performed transactions and what transactions the user performs), and quit. Assume that the bank knows the ATMs public key and the ATM knows the banks public key. The ATM communicates with the bank by running a protocol that satisfies the following requirements:

1. It authenticates the customer to the bank server (ID, passwd).
2. It authenticates the ATM to the bank server through public-key encryption.
3. It preserves the confidentiality of communications between the bank and ATM.
4. The bank server takes actions in response to customer uses of an ATM.

### <i> Assignment 2: </i>
----------
Part 1: DES File Encrypter

The DES class implements the CipherInterface interface. The job of DES class is 
to encrypt/decrypt data using the DES algorithm. The class contains 3 functions 

	setKey()
	encrypt()
	decrypt()

Part 2: Secure Socket

The <i>client</i> shall perform the following functions:

1. Connect to the server using the IP address and port number specified at the command
line.
2. Prompt the user to input a string.
3. Encrypt the user-supplied string using the algorithm and key specified at the com-
mand line.
4. Send the encrypted string to the server.
5. Exit, or wait for another input from user (your choice).

When executed, the <i>server</i> shall perform the following functions:

1. Listen for connections on the specified port.
2. When the client connects, receive the encrypted string sent by the client.
3. Decrypt the specified string using the algorithm and key specified at the command
line.
4. Print the decrypted string.
5. Exit, or wait for more connections (your choice).

