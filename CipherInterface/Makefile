all:	cipher

cipher:	cipher.o Playfair.o DES.o Caesar.o Vigenre.o RailFence.o RowTransposition.o
	g++ cipher.o Playfair.o DES.o Caesar.o Vigenre.o RailFence.o RowTransposition.o -o cipher -lcrypto

cipher.o:	cipher.cpp CipherInterface.h Playfair.h Caesar.h Vigenre.h RailFence.h RowTransposition.h
	g++ -g -c cipher.cpp 

DES.o:	DES.cpp DES.h CipherInterface.h
	g++ -g -c DES.cpp

Playfair.o:	Playfair.cpp Playfair.h CipherInterface.h
	g++ -g -c Playfair.cpp

Caesar.o:	Caesar.cpp Caesar.h CipherInterface.h
	g++ -g -c Caesar.cpp

Vigenre.o: CipherInterface.h Vigenre.cpp Vigenre.h
	g++ -g -c Vigenre.cpp

RailFence.o: RailFence.h RailFence.cpp CipherInterface.h
	g++ -g -c RailFence.cpp

RowTransposition.o: RowTransposition.h RowTransposition.cpp CipherInterface.h
	g++ -g -c RowTransposition.cpp

clean:
	rm -rf *.o cipher
