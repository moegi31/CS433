#include "Caesar.h"
#include "Vigenre.h"

bool Vigenre::setKey(const string& key)
{
	string key_temp = key;
	for(string::iterator it=key_temp.begin(); it !=key_temp.end(); ++it)
	{
		if (!isalpha(*it)) return false;
	}
	mKey = key;
	return true;
}

string Vigenre::encrypt(const string& plaintext)
{
	string pt_temp = plaintext;
	string ciphertext = "";
	Caesar caesar;
	int key_idx = 0;

	for(string::iterator iter=pt_temp.begin(); iter!=pt_temp.end(); ++iter)
	{
		if(isalpha(*iter))	
		{
			char saladKey[10];
			sprintf(saladKey, "%d", toupper(mKey[key_idx])-65);
			char saladPlain = toupper(*iter);

			caesar.setKey(saladKey);
			ciphertext +=caesar.encrypt(string(&saladPlain));
			++key_idx;
			key_idx = key_idx % mKey.size();
		}
		else
			ciphertext += *iter;
	}

	return ciphertext;
}

string Vigenre::decrypt(const string& ciphertext)
{
	string ct_temp = ciphertext;
	string plaintext = "";
	Caesar caesar;
	int key_idx = 0;
	
	for(string::iterator iter=ct_temp.begin(); iter!=ct_temp.end(); ++iter)
	{
		if(isalpha(*iter))	
		{
			char saladKey[10];
			sprintf(saladKey, "%d", toupper(mKey[key_idx])-65);
			char saladCipher = *iter;

			caesar.setKey(saladKey);
			plaintext +=caesar.decrypt(string(&saladCipher));
			++key_idx;
			key_idx = key_idx % mKey.size();
		}
		else
			plaintext += *iter;
	}

	return plaintext;
}
