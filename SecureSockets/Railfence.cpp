#include "Railfence.h"

int railfenceKey;

bool Railfence::setKey(const string& key){
	railfenceKey = atoi(key.c_str());
	return true;
}

string Railfence::encrypt(const string& plaintext){
	string cipherText = "";
    int totalRows = railfenceKey;
    int totalCols = ceil(plaintext.length() / totalRows);
    string** railfenceMatrix = new string*[totalRows];
    for (int i = 0; i < totalRows; i++) {
        railfenceMatrix[i] = new string[totalCols];
    }
    
    int counter = 0;
    for (int col = 0; col < totalCols; col++) {
        for (int row = 0; row < totalRows; row++) {
            railfenceMatrix[row][col] = plaintext[counter];
            counter++;
        }
    }
    counter = 0;
    for (int row = 0; row < totalRows; row++) {
        for (int col = 0; col < totalCols; col++) {
            if(counter < plaintext.length()){
                cipherText += railfenceMatrix[row][col];
            }
            counter++;
        }
    }
    
    
    for (int i = 0; i < totalRows; i++) {
        delete [] railfenceMatrix[i];
    }
    delete [] railfenceMatrix;
    
	return cipherText;
}

string Railfence::decrypt(const string& cipherText) {
	string plainText = "";
    int totalRows = railfenceKey;
    int totalCols = ceil(cipherText.length() / (double)totalRows);
    string** railfenceMatrix = new string*[totalRows];
    for (int i = 0; i < totalRows; i++) {
        railfenceMatrix[i] = new string[totalCols];
    }
    string currentLetter = "";
    int rowsWithExtraLetter = cipherText.length() % railfenceKey;
    int counter = 0;
    for (int row = 0; row < totalRows; row++) {
        for (int col = 0; col < totalCols; col++) {
            if((col == totalCols - 1 && (row < rowsWithExtraLetter || rowsWithExtraLetter == 0)) ||
               col < totalCols - 1){
                railfenceMatrix[row][col] = cipherText[counter];
                currentLetter = cipherText[counter];
                counter++;
            }
        }
    }
    counter = 0;
    for (int col = 0; col < totalCols; col++) {
        for (int row = 0; row < totalRows; row++) {
            if(counter < cipherText.length()){
                plainText += railfenceMatrix[row][col];
            }
            counter++;
        }
    }
    
    
    for (int i = 0; i < totalRows; i++) {
        delete [] railfenceMatrix[i];
    }
    delete [] railfenceMatrix;
    
	return plainText;
}

