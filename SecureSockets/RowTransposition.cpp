#include "RowTransposition.h"

string rowTranspositionKey;

bool RowTransposition::setKey(const string& key){
	rowTranspositionKey = key;
	return true;
}

string RowTransposition::encrypt(const string& plaintext){
    string cipherText = "";
    int totalRows = ceil((double)plaintext.length() / rowTranspositionKey.length());
    int totalCols = (int)rowTranspositionKey.length();
    string** rowTranspositionMatrix = new string*[totalRows];
    for (int i = 0; i < totalRows; i++) {
        rowTranspositionMatrix[i] = new string[totalCols];
    }
    
    int counter = 0;
    for (int row = 0; row < totalRows; row++) {
        for (int col = 0; col < totalCols; col++) {
            if(counter >= plaintext.length()){
                rowTranspositionMatrix[row][col] = "X";
            }else{
                rowTranspositionMatrix[row][col] = plaintext[counter];
            }
            counter++;
        }
    }
    
    for(int i = 0; i < rowTranspositionKey.length(); i++){
        char index = rowTranspositionKey.at(i);
        int colIndex = atoi(&index);
        colIndex--;
        for(int row = 0; row < totalRows; row++){
            cipherText += rowTranspositionMatrix[row][colIndex];
        }
    }
    
    for (int i = 0; i < totalRows; i++) {
        delete [] rowTranspositionMatrix[i];
    }
    delete [] rowTranspositionMatrix;
    
	return cipherText;
}

string RowTransposition::decrypt(const string& cipherText) {
    string plainText = "";
    int totalRows = ceil((double)cipherText.length() / rowTranspositionKey.length());
    int totalCols = (int)rowTranspositionKey.length();
    string** rowTranspositionMatrix = new string*[totalRows];
    for (int i = 0; i < totalRows; i++) {
        rowTranspositionMatrix[i] = new string[totalCols];
    }
    
    int counter = 0;
    for(int i = 0; i < rowTranspositionKey.length(); i++){
        char index = rowTranspositionKey.at(i);
        int colIndex = atoi(&index);
        colIndex--;
        for(int row = 0; row < totalRows; row++){
            rowTranspositionMatrix[row][colIndex] = cipherText[counter];
            counter++;
        }
    }
    
    for (int row = 0; row < totalRows; row++) {
        for (int col = 0; col < totalCols; col++) {
            plainText += rowTranspositionMatrix[row][col];
        }
    }
    
    for (int i = 0; i < totalRows; i++) {
        delete [] rowTranspositionMatrix[i];
    }
    delete [] rowTranspositionMatrix;
    
	return plainText;
}