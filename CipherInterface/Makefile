all:	cipher

cipher:	cipher.o Playfair.o DES.o
	g++ cipher.o Playfair.o DES.o -o cipher -lcrypto

cipher.o:	cipher.cpp
	g++ -g -c cipher.cpp 

DES.o:	DES.cpp DES.h
	g++ -g -c DES.cpp

Playfair.o:	Playfair.cpp Playfair.h CipherInterface.h
	g++ -g -c Playfair.cpp

# Uncomment this code once you add the appropriate files
#RowTransposition.o:	RowTransposition.cpp RowTransposition.h
#	g++ -g -c RowTransposition.cpp


clean:
	rm -rf *.o cipher
