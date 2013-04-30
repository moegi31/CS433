#include "Playfair.h"
#include <stdio.h>
#include <string.h>
/**
 * Sets the key to use
 * @param key - the key to use
 * @return - True if the key is valid and False otherwise
 */
bool Playfair::setKey(const string& key)
{
	string mKey = key; 

	for(string::iterator iter=mKey.begin(); iter !=mKey.end(); ++iter)
	{
		if(!isalpha(*iter)) return false;
	}
	
	createPlayfairMatrix(mKey);
	return true;
}


struct playfair_char_t 
{
	char letter;
	int row;
	int col;
};


/**	
 * Encrypts a plaintext string
 * @param plaintext - the plaintext string
 * @return - the encrypted ciphertext string
 */
string Playfair::encrypt(const string& plaintext)
{
	string pfPlaintext = plaintext;
	
	int ptextsize = plaintext.size();
	
	for(string::iterator it=pfPlaintext.begin(); it !=pfPlaintext.end(); ++it)
	{
		*it = toupper(*it);
		if(*it == 'J') 
		{
			*it = 'I';
		}
	}
	
	string ciphertext = "";
	playfair_char_t first, second;

	string::iterator iter=pfPlaintext.begin();
	
	while (iter !=pfPlaintext.end())
	{
		first.letter = *iter;
		
		bool skip_second = false;
		
		if ((iter+1) == pfPlaintext.end())
		{
			second.letter = 'X';
			skip_second = true;
		}
		else
		{
			second.letter = *(iter+1);

			if (second.letter == first.letter)
			{
				skip_second = true;
				second.letter = 'X';
			}
		}

		
		bool first_found = false, second_found = false;

		for (int row=0; row<5; row++)
		{
			for (int col=0; col<5; col++)
			{
				if(first.letter == pfMatrix[row][col])
				{
					first.row = row;
					first.col = col;
					first_found = true;
				}
				if(second.letter == pfMatrix[row][col])
				{
					second.row = row;
					second.col = col;
					second_found = true;
				}
			}
		}

		// exit if unacceptable character is in plaintext
		if (!(first_found && second_found)) 
		{
			printf("Character <%c | %c> not found in PlayFair Table, exiting...\n", 
						first.letter, second.letter);
			exit(-1);
		}
		
		if (first.row == second.row)	//same row
		{
			int f_col = (first.col+1)%5;
			int s_col = (second.col+1)%5;
			
			ciphertext += pfMatrix[first.row][f_col];
			ciphertext += pfMatrix[second.row][s_col];
		}
		else if (first.col == second.col)	//same column
		{
			int f_row = (first.row+1)%5;
			int s_row = (second.row+1)%5;
			
			ciphertext += pfMatrix[f_row][first.col];
			ciphertext += pfMatrix[s_row][second.col];
		}
		else
		{	// swap columns to complete square pair
			ciphertext += pfMatrix[first.row][second.col];
			ciphertext += pfMatrix[second.row][first.col];
		}
		
		// if a sequential character is found, skip by 1
		if (skip_second)
		{
			++iter;
		}
		else	// skip by two
		{
			iter+=2;
		}
	}//end of plaintext, should be fully encrypted

	return ciphertext; 
}

/**
 * Decrypts a string of ciphertext
 * @param cipherText - the ciphertext
 * @return - the plaintext
 */
string Playfair::decrypt(const string& cipherText) 
{ 
	string cipher_temp=cipherText, plaintext = "";
	playfair_char_t first, second;

	for (string::iterator iter=cipher_temp.begin(); iter!=cipher_temp.end(); iter+=2)
	{
		first.letter = *iter;
		second.letter = *(iter+1);

		bool first_found = false, second_found = false;

		for (int row=0; row<5; row++)
		{
			for (int col=0; col<5; col++)
			{
				if(first.letter == pfMatrix[row][col])
				{
					first.row = row;
					first.col = col;
					first_found = true;
				}
				if(second.letter == pfMatrix[row][col])
				{
					second.row = row;
					second.col = col;
					second_found = true;
				}
			}
		}
		
		if (first.row == second.row)	//same row
		{
			int f_col = (first.col-1+5)%5;
			int s_col = (second.col-1+5)%5;
			
			plaintext += pfMatrix[first.row][f_col];
			plaintext += pfMatrix[second.row][s_col];
		}
		else if (first.col == second.col)	//same column
		{
			int f_row = (first.row-1+5)%5;
			int s_row = (second.row-1+5)%5;
			
			plaintext += pfMatrix[f_row][first.col];
			plaintext += pfMatrix[s_row][second.col];
		}
		else
		{	// swap columns to complete square pair
			plaintext += pfMatrix[first.row][second.col];
			plaintext += pfMatrix[second.row][first.col];
		}
		
	}//end of plaintext, should be fully encrypted
	return plaintext; 
		
}

void Playfair::createPlayfairMatrix(const string& key)
{
	string alphabet = "abcdefghiklmnopqrstuvwxyz";
	string fairplay_key = "";
	string mKey = key;

	for (string::iterator iter=mKey.begin(); iter != mKey.end(); ++iter)
	{
		char letter = toupper(*iter);
		if(letter == 'J') letter = 'I';
		
		if(string::npos == fairplay_key.find_first_of(letter))
		{
			fairplay_key += letter;
		}
	}
	for (string::iterator iter=alphabet.begin(); iter != alphabet.end(); ++iter)
	{
		char letter = toupper(*iter);
		
		if(string::npos == fairplay_key.find_first_of(letter))
		{
			fairplay_key += letter;
		}
	}
	
	int pfStringIdx = 0;
	for (int row=0; row<5; row++)
	{
		for (int col=0; col<5; col++)
		{
			pfMatrix[row][col] = fairplay_key[pfStringIdx];
			pfStringIdx++;
		}
	}
}
