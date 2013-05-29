all: client server

client:	client.o RowTransposition.o Railfence.o Vigenere.o Caesar.o Playfair.o DES.o helper.o
	g++ client.o RowTransposition.o Railfence.o Vigenere.o Caesar.o Playfair.o DES.o helper.o -o client -lcrypto

server: server.o RowTransposition.o Railfence.o Vigenere.o Caesar.o Playfair.o DES.o helper.o
	g++ server.o RowTransposition.o Railfence.o Vigenere.o Caesar.o Playfair.o DES.o helper.o -o server -lcrypto

client.o:	client.cpp
	g++ -g -c client.cpp
	
server.o:	server.cpp
	g++ -g -c server.cpp

helper.o:	helper.h helper.cpp
	g++ -c helper.cpp 

DES.o:	DES.cpp DES.h CipherInterface.h
	g++ -g -c DES.cpp
	
Playfair.o:	Playfair.cpp Playfair.h CipherInterface.h
	g++ -g -c Playfair.cpp
    
Caesar.o: Caesar.cpp Caesar.h CipherInterface.h
	g++ -g -c Caesar.cpp
    
Vigenere.o:	Vigenere.cpp Vigenere.h CipherInterface.h
	g++ -g -c Vigenere.cpp
    
Railfence.o: Railfence.cpp Railfence.h CipherInterface.h
	g++ -g -c Railfence.cpp
    
RowTransposition.o:	RowTransposition.cpp RowTransposition.h CipherInterface.h
	g++ -g -c RowTransposition.cpp

clean:
	rm -rf *.o cipher
