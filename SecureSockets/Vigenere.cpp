#include "Vigenere.h"

string vigenereKey;
string vigenereMatrix[26][26];

void PopulateVigenereMatrix();

bool Vigenere::setKey(const string& key){
	vigenereKey = key;
	return true;
}

string Vigenere::encrypt(const string& plaintext){
	PopulateVigenereMatrix();
    string cipherText = "";
    string plainText = plaintext;
    int keyIndex = 0;
    int plainTextIndex = 0;
    for (int i = 0; i < plainText.length(); i++){
        if(keyIndex == vigenereKey.length()){
            keyIndex = 0;
        }
        string encryptRow = "";
        string encryptCol = "";
        encryptRow += vigenereKey[keyIndex];
        encryptCol += plainText[plainTextIndex];
        cipherText += vigenereMatrix[LetterToInt(encryptRow)][LetterToInt(encryptCol)];
        plainTextIndex++;
        keyIndex++;
    }
    
	return cipherText;
}

string Vigenere::decrypt(const string& cipherText) {
	PopulateVigenereMatrix();
    string plainText = "";
    string ciphertext = cipherText;
    int keyIndex = 0;
    int cipherTextIndex = 0;
    
    for (int i = 0; i < ciphertext.length(); i++){
        if(keyIndex == vigenereKey.length()){
            keyIndex = 0;
        }
        string decryptRow = "";
        int colIndex = 0;
        decryptRow += vigenereKey[keyIndex];
        for (int j = 0; j < 26; j++) {
            string checkLetter = "";
            checkLetter += ciphertext[cipherTextIndex];
            if(vigenereMatrix[LetterToInt(decryptRow)][j] == checkLetter){
                colIndex = j;
                break;
            }
        }
        plainText += vigenereMatrix[0][colIndex];
        cipherTextIndex++;
        keyIndex++;
    }
    
	return plainText;
}

void PopulateVigenereMatrix(){
    int startingLetter = 0;
    for (int row = 0; row < 26; row++) {
        int rowLetter = startingLetter;
        for (int col = 0; col < 26; col++) {
            if(rowLetter == 26){
                rowLetter = 0;
            }
            vigenereMatrix[row][col] = IntToLetter(rowLetter);
            rowLetter++;
        }
        startingLetter++;
    }
}

