#include "Playfair.h"

string playfairKey;
string playfairMatrix[5][5];

void PopulatePlayfairMatrix();
bool LetterInKey(string letter);
void PopulateMatrixWithKey();
void PrintMatrix();
string PadInput(string input);
void LetterCoords(string letter, int* xCoord, int *yCoord);

bool Playfair::setKey(const string& key){
	playfairKey = key;
    
    string strippedKey = "";
    for(int i = 0; i < playfairKey.length(); i++){
        bool duplicate = false;
        for(int j = 0; j < strippedKey.length(); j++){
            if(strippedKey[j] == playfairKey[i]){
                duplicate = true;
            }
        }
        if(!duplicate){
            strippedKey += playfairKey[i];
        }
    }
    
    playfairKey = strippedKey;
    
	return true;
}

string Playfair::encrypt(const string& plaintext){
	PopulatePlayfairMatrix();
	string paddedInput = PadInput(plaintext);
    int numPairs = (int)paddedInput.length() / 2;
    string encryptedText = "";
    
    for (int i = 0; i < numPairs; i++) {
        int pairIndex = (i * 2);
        string letter0 = "";
        string letter1 = "";
        letter0 = paddedInput[pairIndex];
        letter1 = paddedInput[pairIndex + 1];
        int letter0Col = -1;
        int letter0Row = -1;
        int letter1Col = -1;
        int letter1Row = -1;
        LetterCoords(letter0, &letter0Col, &letter0Row);
        LetterCoords(letter1, &letter1Col, &letter1Row);
        
        if(letter0Row == letter1Row){
            letter0Col++;
            letter1Col++;
            if(letter0Col == 5){ letter0Col = 0; }
            if(letter1Col == 5){ letter1Col = 0; }
            encryptedText += playfairMatrix[letter0Row][letter0Col] + playfairMatrix[letter1Row][letter1Col];
        }else if(letter0Col == letter1Col){
            letter0Row++;
            letter1Row++;
            if(letter0Row == 5){ letter0Row = 0; }
            if(letter1Row == 5){ letter1Row = 0; }
            encryptedText += playfairMatrix[letter0Row][letter0Col] + playfairMatrix[letter1Row][letter1Col];
        }else{
            encryptedText += playfairMatrix[letter0Row][letter1Col] + playfairMatrix[letter1Row][letter0Col];
        }
    }
    
	return encryptedText;
}

string Playfair::decrypt(const string& cipherText) { 
	PopulatePlayfairMatrix();
    int numPairs = (int)cipherText.length() / 2;
    string decryptedText = "";
    
    for (int i = 0; i < numPairs; i++) {
        int pairIndex = (i * 2);
        string letter0 = "";
        string letter1 = "";
        letter0 = cipherText[pairIndex];
        letter1 = cipherText[pairIndex + 1];
        int letter0Col = -1;
        int letter0Row = -1;
        int letter1Col = -1;
        int letter1Row = -1;
        LetterCoords(letter0, &letter0Col, &letter0Row);
        LetterCoords(letter1, &letter1Col, &letter1Row);
        
        if(letter0Row == letter1Row){
            letter0Col--;
            letter1Col--;
            if(letter0Col == -1){ letter0Col = 4; }
            if(letter1Col == -1){ letter1Col = 4; }
            decryptedText += playfairMatrix[letter0Row][letter0Col] + playfairMatrix[letter1Row][letter1Col];
        }else if(letter0Col == letter1Col){
            letter0Row--;
            letter1Row--;
            if(letter0Row == -1){ letter0Row = 4; }
            if(letter1Row == -1){ letter1Row = 4; }
            decryptedText += playfairMatrix[letter0Row][letter0Col] + playfairMatrix[letter1Row][letter1Col];
        }else{
            decryptedText += playfairMatrix[letter0Row][letter1Col] + playfairMatrix[letter1Row][letter0Col];
        }
    }
    
	return decryptedText;
}

void LetterCoords(string letter, int* colPos, int *rowPos){
    
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            if(playfairMatrix[row][col] == letter){
                *colPos = col;
                *rowPos = row;
            }
        }
    }
}

string PadInput(string input){
    int length = (int)input.length();
    string paddedInput = "";
    for (int i = 0; i < length; i++) {
        paddedInput += input[i];
        if (input[i] == input[i + 1]) {
            paddedInput += "X";
        }
    }
    if(paddedInput.length() % 2 == 1){
        paddedInput += "X";
    }
    return paddedInput;
}

void PopulatePlayfairMatrix(){
    cout << "Playfair Matrix:" << endl;
    int currentLetter = 0;
    int startingRow = floor(playfairKey.length() / 5);
    int startingCol = playfairKey.length() % 5;
    
    PopulateMatrixWithKey();
    for (int row = startingRow; row < 5; row++) {
        startingRow = 0;
        for (int col = startingCol; col < 5; col++) {
            startingCol = 0;
            string letter = IntToLetter(currentLetter);
            if(!LetterInKey(letter) && letter != "J"){
                playfairMatrix[row][col] = IntToLetter(currentLetter);
            }else{
                col--;
            }
            currentLetter++;
        }
    }
    
    PrintMatrix();
}

void PrintMatrix(){
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            cout << playfairMatrix[row][col] << " ";
        }
        cout << endl;
    }
}

void PopulateMatrixWithKey(){
    int rowCounter = 0;
    int colCounter = 0;
    
    for(int i = 0; i < playfairKey.length(); i++){
        if(colCounter == 5){
            rowCounter++;
            colCounter = 0;
        }
        playfairMatrix[rowCounter][colCounter] = playfairKey[i];
        colCounter++;
    }
}

bool LetterInKey(string letter){
    for(int i = 0; i < playfairKey.length(); i++){
        if(playfairKey[i] == letter[0]){
            return true;
        }
    }
    return false;
}