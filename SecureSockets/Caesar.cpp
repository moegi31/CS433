#include "Caesar.h"

int caesarKey;
string caesarMatrix[26];

void PopulateCaesarMatrix(int offset);

bool Caesar::setKey(const string& key){
	caesarKey = atoi(key.c_str());
	return true;
}

string Caesar::encrypt(const string& plaintext){
    string cipherText = "";
    PopulateCaesarMatrix(caesarKey);
    string currentLetter = "";
    for (int i = 0; i < plaintext.length(); i++) {
        currentLetter = plaintext[i];
        cipherText += caesarMatrix[LetterToInt(currentLetter)];
    }
	return cipherText;
}

string Caesar::decrypt(const string& cipherText) {
    string plainText = "";
    PopulateCaesarMatrix(caesarKey);
    string currentLetter = "";
    for (int i = 0; i < cipherText.length(); i++) {
        currentLetter = cipherText[i];
        int letterIndex = LetterToInt(currentLetter) - caesarKey;
        if(letterIndex < 0){
            letterIndex = 26 + letterIndex;
        }
        plainText += IntToLetter(letterIndex);
    }
	return plainText;
}

void PopulateCaesarMatrix(int offset)
{
    int currentLetter = offset;
    for (int col = 0; col < 26; col++) {
        if(currentLetter == 26){
            currentLetter = 0;
        }
        caesarMatrix[col] = IntToLetter(currentLetter);
        currentLetter++;
    }
}