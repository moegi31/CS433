#ifndef CAESAR_H
#define CAESAR_H

#include <vector>   /* For vectors */
#include <string>   /* For C++ strings */
#include <stdio.h>  /* For standard I/O */
#include <stdlib.h> /* For miscellenous C functions */
#include <iostream>
#include <math.h>
#include "helper.h"
#include "CipherInterface.h"

using namespace std;

/**
 * This class implements the playfair cipher.
 * The class extends the abstract class
 * CipherInterface.
 */

class Caesar: public CipherInterface
{
	/** The public members **/
public:
    
    bool setKey(const string& key);
    string encrypt(const string& plaintext);
    string decrypt(const string& ciphertext);
    
    /* The protected members */
protected:
	
    
};

#endif
