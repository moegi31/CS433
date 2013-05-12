#ifndef ROWTRANSPOS_H
#define ROWTRANSPOS_H

#include <string>   /* For C++ strings */
#include <stdio.h>  /* For standard I/O */
#include <stdlib.h> /* For miscellenous C functions */
#include "CipherInterface.h"

#define MAX_NUM_RAILS 9
using namespace std;

/**
 * This class implements the Row Transposition Cipher.
 * The class extends the abstract class 
 * CipherInterface.
 */

class RowTransposition: public CipherInterface
{
	/** The public members **/
	public:

		/**
		 * Sets the key to use
		 * @param key - the key to use
		 * @return - True if the key is valid and False otherwise
		 */
		virtual bool setKey(const string& key);

		/**	
		 * Encrypts a plaintext string
		 * @param plaintext - the plaintext string
		 * @return - the encrypted ciphertext string
		 */
		virtual string encrypt(const string& plaintext);

		/**
		 * Decrypts a string of ciphertext
		 * @param ciphertext - the ciphertext
		 * @return - the plaintext
		 */
		virtual string decrypt(const string& ciphertext);
	
	private:
		int mKey[MAX_NUM_RAILS];
		int column_count;
};

#endif
