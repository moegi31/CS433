#include <string>
#include <fstream>
#include <cerrno>
#include "CipherInterface.h"
#include "Playfair.h"
#include "DES.h"

using namespace std;

// source from: http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
std::string get_file_contents(const char *filename, long int offset, int blocksize)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
	int size=blocksize;
    std::string contents;
    in.seekg(0, std::ios::end);
	
	if (in.tellg()-offset < blocksize)
		size = in.tellg()-offset;
	
    contents.resize(size);
	in.seekg(offset, std::ios::beg);
    in.read(&contents[0], contents.size());
	if (contents[contents.size()-1] == '\n')
	{
		contents.erase(contents.size()-1, 1);
	}
    in.close();
    return(contents);
  }
  throw(errno);
}

void put_file_contents(const char *filename, string output, long int offset)
{
	ofstream output_file;
	if (offset == 0)
	{
		output_file.open(filename);
	}
	else
	{
		output_file.open(filename, ios::app);
	}
	output_file << output;
	output_file.close();
}

int main(int argc, char** argv)
{
	if (argc != 6)
	{
		printf("arg count: %u\n", argc);
		printf("USAGE: %s <CIPHER NAME> <KEY> <ENC/DEC> <INPUTFILE> <OUTPUT FILE>\n",
				argv[0]);
		return 0;
	}
	
	string plf_string("PLF");
	string des_string("DES");
	string enc_string("ENC");
	string dec_string("DEC");

	CipherInterface *cipher;
	// set cipher type
	if ( plf_string.compare(argv[1]) == 0)
	{
		cipher = new Playfair();
	}
	else if ( des_string.compare(argv[1]) == 0)
	{
		cipher = new DES();
	}
	else
	{
		printf("INVALID CIPHER NAME, EXITING...\n");
		return 0;
	}
	
	/* Error checks */
	if(!cipher)
	{
		fprintf(stderr, "ERROR [%s %s %d]: could not allocate memory\n",	
		__FILE__, __FUNCTION__, __LINE__);
		exit(-1);
	}

	// set key
	if( cipher->setKey(argv[2]) )
	{
		int offset = 0;

		while(true)
		{
			string input = get_file_contents(argv[4], offset, 8);
			if(input.length() <=0) return 0;
				
			if (enc_string.compare(argv[3]) == 0)
			{
				// read input from file
				string ciphertext = cipher->encrypt(input);
				// output encrypted text to file
				put_file_contents(argv[5], ciphertext, offset);
			}
			else if(dec_string.compare(argv[3]) == 0)
			{
				//read input from file
				string plaintext = cipher->decrypt(input);
				// output plaintext to file
				put_file_contents(argv[5], plaintext, offset);
			}
			else
			{
				printf("PARAMETER 3 <%s> INVALID, TRY AGAIN...\n", argv[3]);
				return 0;
			}
			offset += input.length();
		}
	}
	else
	{
		printf("key is invalid\n");
		return 0;
	}

	return 0;
}
