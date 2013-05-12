#include "Caesar.h"


bool Caesar::setKey(const string& key)
{
	string key_temp = key;
	for(string::iterator it=key_temp.begin(); it !=key_temp.end(); ++it)
	{
		if (!isdigit(*it)) return false;
	}
	mKey = atoi(key_temp.c_str()) % 26;
	return true;
}

string Caesar::encrypt(const string& plaintext)
{
	string pt_temp = plaintext;
	string ciphertext = "";
	
	for(string::iterator iter=pt_temp.begin(); iter!=pt_temp.end(); ++iter)
	{
		if(isalpha(*iter))	
		{
			*iter = toupper(*iter);
			*iter = (((*iter-65)+mKey)%26)+65;
		}
		ciphertext += *iter;
	}

	return ciphertext;
}

string Caesar::decrypt(const string& ciphertext)
{
	string ct_temp = ciphertext;
	string plaintext = "";
	
	for(string::iterator iter=ct_temp.begin(); iter!=ct_temp.end(); ++iter)
	{
		if(isalpha(*iter))	
		{
			*iter = toupper(*iter);
			char cipher_char = *iter - mKey;
			if(cipher_char < 65) cipher_char +=26;
			*iter = cipher_char;
		}
		plaintext += *iter;
	}
	return plaintext;
}
