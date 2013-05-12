	Collaborators: Chris Gudea, David Robison
	Email: pulala@gmail.com, davidrobison122@gmail.com
	Language: Python 2.7.3

#### Compilation and Running
To build, simply run

    make

To remove binary and compilation files

    make clean

Running server

	python bankserver.py  <PORT> 

where

* PORT: the port to have ATM clients connect on

Running client

	python atm.py  <SERVER IP> <SERVER PORT> <ATM ID> 

where

* SERVER IP: ip of the server (eg. 127.0.0.1)
* SERVER PORT: the server port to connect on (eg. 1234)
* ATM ID: either 1 or 2 (identifies ATM and which key to use)

#### Notes
This program has dependencies on Python version 2.5+. Some of the libraries
used are deprecated as of Python 3.0+. Python version 2.7.3 was used on the test
machine for compliation and running.

#### Extra Credit not implemented
