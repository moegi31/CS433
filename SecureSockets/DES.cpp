#include "DES.h"

#define ENC 1
#define DEC 0

/**
 * Sets the key to use
 * @param key - the key to use
 * @return - True if the key is valid and False otherwise
 */
bool DES::setKey(const string& key)
{
	/**
	 * First let's covert the char string
	 * into an integer byte string
	 */
	
	/* Get the pointer to the cstring */
	const char* cstrKey = key.c_str();
	
	/* The key error code */
	int keyErrorCode = -1;
		
	/* Check for a valid key length */
	if(key.length() / 2 != 8)
	{
		fprintf(stderr, "Invalid key length\n");
		return false;
	}

	/* A single byte */
	unsigned char singleByte = 0;	
	
	/* The key index */
	int keyIndex = 0;
	
	/* The DES key index */
	int desKeyIndex = 0;
		
	/* Go through the entire key character by character */
	while(desKeyIndex != 8)
	{
		/* Convert the key if the character is valid */
		if((this->des_key[desKeyIndex] = twoCharToHexByte(cstrKey + keyIndex)) == 'z')
			return false;
		
		/* Go to the second pair of characters */
		keyIndex += 2;	
		
		/* Increment the index */
		++desKeyIndex;
	}
	
	fprintf(stdout, "DES KEY: ");
	
	/* Print the key */
	for(keyIndex = 0; keyIndex < 8; ++keyIndex)
		fprintf(stdout, "%x", this->des_key[keyIndex]);
	
	fprintf(stdout, "\n");	
	
	
	/* Set the encryption key */
	if ((keyErrorCode = des_set_key_checked(&des_key, this->key)) != 0)
	{
		fprintf(stderr, "\nkey error %d\n", keyErrorCode);
		
		return false;
	}
	
	/* All is well */	
	return true;
}

/**
 * Converts a binary C string into a C++ string
 * @param cstring - the cstring
 * @param len - the length of the cstring
 * @return - the C++ string containing all the binary bytes
 */
string cstrToCpp(unsigned char* cstring, const int& len)
{
	/* Convert the cstring to CPP string */
	string cppStr(cstring, cstring + len);
	
	return cppStr;
}

/**	
 * Encrypts a plaintext string
 * @param plaintext - the plaintext string
 * @return - the encrypted ciphertext string
 */
string DES::encrypt(const string& plaintext)
{
	//LOGIC:
	//1. Check to make sure that the block is exactly 8 characters (i.e. 64 bits)
	if (strlen(plaintext.c_str()) != 8)
	{
		printf("DES::encrypt: Block is not 8 characters!\n");
		exit(0);
	}
	//2. Declare an array DES_LONG block[2];
	DES_LONG block[2];
	// declare ciphertext
	unsigned char bytes[9];
	// declare parts of plaintext to pass to ctol()
	char ptext[9];
	strncpy(ptext, plaintext.c_str(), 9);
	//3. Use ctol() to convert the first 4 chars into long; store the result in block[0]
	block[0] = ctol((unsigned char*)ptext);
	//4. Use ctol() to convert the second 4 chars into long; store the result in block[1]
	block[1] = ctol((unsigned char*)(ptext+4));
	//5. Perform des_encrypt1 in order to encrypt the block using this->key (see sample codes for details)
	des_encrypt1(block, key, ENC);
	//6. Convert the first ciphertext long to 4 characters using ltoc()
	memset(bytes,0,9);
	ltoc(block[0], bytes);
	//7. Convert the second ciphertext long to 4 characters using ltoc()
	ltoc(block[1], bytes+4);
	//8. Save the results in the resulting 8-byte string (e.g. bytes[8])
	bytes[8] = '\0';	
	//9. Convert the string (e.g. bytes[8]) to a C++ string.
	string result = cstrToCpp(bytes, 9);
	//10.Return the C++ string
	return result;
}

/**
 * Decrypts a string of ciphertext
 * @param ciphertext - the ciphertext
 * @return - the plaintext
 */
string DES::decrypt(const string& ciphertext)
{
	//LOGIC:
	//1. Check to make sure that the block is exactly 8 characters (i.e. 64 bits)
	if (strlen(ciphertext.c_str()) != 8)
	{
		printf("DES::decrypt: Block is not 8 characters!\n");
		exit(0);
	}
	//2. Declare an array DES_LONG block[2];
	DES_LONG block[2];
	// declare ciphertext
	unsigned char bytes[9];
	// declare parts of ciphertext to pass to ctol()
	char ctext[9];
	strncpy(ctext, ciphertext.c_str(), 9);
	//3. Use ctol() to convert the first 4 chars into long; store the result in block[0]
	block[0] = ctol((unsigned char*)ctext);
	//4. Use ctol() to convert the second 4 chars into long; store the result in block[1]
	block[1] = ctol((unsigned char*)(ctext+4));
	//5. Perform des_encrypt1 in order to encrypt the block using this->key (see sample codes for details)
	des_encrypt1(block, key, DEC);
	//6. Convert the first ciphertext long to 4 characters using ltoc()
	memset(bytes,0,9);
	ltoc(block[0], bytes);
	//7. Convert the second ciphertext long to 4 characters using ltoc()
	ltoc(block[1], bytes+4);
	//8. Save the results in the resulting 8-byte string (e.g. bytes[8])
	bytes[8] = '\0';	
	//9. Convert the string (e.g. bytes[8]) to a C++ string.
	string result = cstrToCpp(bytes, 9);
	//10.Return the C++ string
	return result;
}

/**
 * Converts an array of 8 characters
 * (i.e. 4 bytes/32 bits)
 * @param c - the array of 4 characters (i.e. 1-byte per/character
 * @return - the long integer (32 bits) where each byte
 * is equivalent to one of the bytes in a character array
 */
DES_LONG DES::ctol(unsigned char *c) 
{
        /* The long integer */
	DES_LONG l;
        
	l =((DES_LONG)(*((c)++)));
        l = l | (((DES_LONG)(*((c)++)))<<8L);
        l = l | (((DES_LONG)(*((c)++)))<<16L);
        l = l | (((DES_LONG)(*((c)++)))<<24L);
        return l;
};


/** 
 * Converts a long integer (4 bytes = 32 bits)
 * into an array of 8 characters.
 * @param l - the long integer to convert
 * @param c - the character array to store the result
 */
void DES::ltoc(DES_LONG l, unsigned char *c) 
{
        *((c)++)=(unsigned char)(l&0xff);
        *((c)++)=(unsigned char)(((l)>> 8L)&0xff);
        *((c)++)=(unsigned char)(((l)>>16L)&0xff);
        *((c)++)=(unsigned char)(((l)>>24L)&0xff);
}

/**
 * Converts a character into a hexidecimal integer
 * @param character - the character to convert
 * @return - the converted character, or 'z' on error
 */
unsigned char DES::charToHex(const char& character)
{
	/* Is the first digit 0-9 ? */	
	if(character >= '0' && character <= '9')	
		/* Convert the character to hex */
		return character - '0';
	/* It the first digit a letter 'a' - 'f'? */
	else if(character >= 'a' && character <= 'f')
		/* Conver the cgaracter to hex */
		return (character - 97) + 10;	
	/* Invalid character */
	else return 'z';
}

/**
 * Converts two characters into a hex integers
 * and then inserts the integers into the higher
 * and lower bits of the byte
 * @param twoChars - two charcters representing the
 * the hexidecimal nibbles of the byte.
 * @param twoChars - the two characters
 * @return - the byte containing having the
 * valud of two characters e.g. string "ab"
 * becomes hexidecimal integer 0xab.
 */
unsigned char DES::twoCharToHexByte(const char* twoChars)
{
	/* The byte */
	unsigned char singleByte;
	
	/* The second character */
	unsigned char secondChar;

	/* Convert the first character */
	if((singleByte = charToHex(twoChars[0])) == 'z') 
	{
		/* Invalid digit */
		return 'z';
	}
	
	/* Move the newly inserted nibble from the
	 * lower to upper nibble.
	 */
	singleByte = (singleByte << 4);
	
	/* Conver the second character */
	if((secondChar = charToHex(twoChars[1])) == 'z')
		return 'z'; 
	
	/* Insert the second value into the lower nibble */	
	singleByte |= secondChar;

	return singleByte;
}

