#include "RailFence.h"


bool RailFence::setKey(const string& key)
{
	string key_temp = key;
	for(string::iterator it=key_temp.begin(); it !=key_temp.end(); ++it)
	{
		if (!isdigit(*it)) return false;
	}
	mKey = atoi(key_temp.c_str());
	return true;
}

string RailFence::encrypt(const string& plaintext)
{
	string pt_temp = plaintext;
	string ciphertext = "";
	int padding = mKey-(pt_temp.size() % mKey);
	if (padding != 0)
	{
		pt_temp.append(padding, '?');
	}

	for(int rail=0; rail<mKey; rail++)
	{
		for(int col=0; col<pt_temp.size(); col+=mKey)
		{
			ciphertext += pt_temp[rail+col];
		}
	}

	return ciphertext;
}

string RailFence::decrypt(const string& ciphertext)
{
	string ct_temp = ciphertext;
	string plaintext = "";
	int rail_length = ct_temp.size()/mKey;
		
	for(int col=0; col<rail_length; col++)
	{
		for(int rail=0; rail<mKey; rail++)
		{
			plaintext += ct_temp[(rail*rail_length)+col];
		}
	}
	return plaintext;
}
