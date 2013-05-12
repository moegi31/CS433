	Collaborators: Chris Gudea, David Robison
	Contact: pulala@gmail.com, davidrobison122@gmail.com
	Language: C++

#### Compilation and Running
To build, simply run

    make

To remove binary and compilation files

    make clean

Running

	./cipher <ALGORITHM> <KEY> <ENC/DEC> <INPUTFILE> <OUTPUT FILE>

where

* ALGORITHM: the type of algorithm to use (DES, PLF)
* KEY: the encryption key to use (must be 16 characters representing a 64-bit hexadecimal number).
* ENC/DEC: whether to encrypt or decrypt, respectively.
* INPUT FILE: the file from which to read the input.
* OUTPUT FILE: the file to which to write the output.

#### Notes
* Playfair: the key must not have any repeated characters
* RailFence: Padded with '?' if length isnt easily divided into #rails

With large files, the block size needs to be adjusted in source code

For every cipher but DES, encrypts 100 bytes at a time, so for some encryption techniques the ciphertext may be altered

For DES, it encrypts in 8 bit blocks

#### Extra Credit not implemented
