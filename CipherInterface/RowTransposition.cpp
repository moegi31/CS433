#include "RowTransposition.h"

bool RowTransposition::setKey(const string& key)
{
	column_count = 0;
	string key_temp = key;
	for(string::iterator it=key_temp.begin(); it !=key_temp.end(); ++it)
	{
		if (!isdigit(*it))
		{
			return false;
		}
		else if (*it == '0')
		{
			return false;
		}
		else 
		{
			mKey[column_count] = (*it) - '0';
			column_count++;
			if (column_count > MAX_NUM_RAILS)
			{
				printf("Invalid key! Exiting...\n");
				return false;
			}
		}
	}
	
	return true;
}

string RowTransposition::encrypt(const string& plaintext)
{
	string pt_temp = plaintext;
	string ciphertext = "";
	int padding = pt_temp.size() % column_count;
	if (padding != 0)
	{
		pt_temp.append(padding, '?');
	}

	int row_count = pt_temp.size() / column_count;

	for(int key_idx=0; key_idx<column_count; key_idx++)
	{
		for(int row=0; row<row_count; row++)
		{
			ciphertext += pt_temp[(mKey[key_idx]-1)+(row*column_count)];
		}
	}

	return ciphertext;
}

string RowTransposition::decrypt(const string& ciphertext)
{
	string ct_temp = ciphertext;
	string plaintext = "";
	int	row_count = ct_temp.size()/column_count;
	
	for (int row=0; row<row_count; row++)
	{ //walk through each row
	
	 	for (int col=1; col<(column_count+1); col++)
	 	{ 
			// walk through all the logical columns
		
		 	// find the offset into the key where the column starts
		 	int col_offset = -1;
		 	for (int i=0; i < column_count; i++)
		 		if (mKey[i] == col) col_offset = i;
		
		 	//calculate the index
		 	int idx = col_offset*row_count+row;
		 	plaintext+= ciphertext[idx]; 
		}
	}
	return plaintext;
}
