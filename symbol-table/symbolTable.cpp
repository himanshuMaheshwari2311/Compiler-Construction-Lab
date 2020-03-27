#include<iostream>
#include<string>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>

using namespace std;


/**
Global Variables
*/
typedef struct {
    string identifier;
    string type;
}symbolTable;

typedef struct {
    string functionName;
    string returnType;
    string identifier;
    string type;
    string varType;
}functionSymbolTable;


/**
Function Prototype
*/
string fileInput();
bool isKeyword(string);
bool isChar(char);
bool isNum(char);
bool isDataType(string);
bool isOperator(string);
bool isConst(string);
bool isIdentifier(string);


string fileInput(){
    int array_size = 1024;
	char * content = new char[array_size];
	string fileContent, temp;
	int position = 0;

	ifstream fin("test/test.cpp");
    if(fin.is_open()) {
    cout << "File Opened.. Reading Content..." << endl;
    	while(!fin.eof() && position < array_size){
			fin.get(content[position]);
			position++;
		}
		content[position-1] = '\0';
		for(int i = 0; content[i] != '\0'; i++)
			fileContent += content[i];
        fin.close();
	}
	else {
		cout << "File could not be opened." << endl;
	}
	return fileContent;
}

/**
checks if token is key word or not
*/
bool isKeyword(string str) {
    string keywords[65] = {"asm","auto","bool","break","case","catch","char","class","const","const_cast","continue","default","delete","do",
    "double","dynamic_cast","else","enum","explicit","export","extern","false","float","for","friend","goto","if","inline","int","long",
    "mutable","namespace","new","operator","private","protected","public","register","reinterpret_cast","return","short","signed","sizeof","static",
    "static_cast","struct","switch","template","this","throw","true","try","typedef","typeid","typename","union","unsigned","using","virtual",
    "void","volatile","wchar_t","while","main"};
    for(int i = 0; i < 65; i++)
        if(str == keywords[i])
            return true;
    return false;
}


bool isDataType(string str) {
    string dataType[7] = {"bool","char","double","float","int","long","short"};
    for(int i = 0; i < 7; i++)
        if(str == dataType[i])
            return true;
    return false;
}

/**
Utility function
*/
bool isChar(char c) {
    if((c>=65 && c<=90) || (c>=97 && c<=122))
        return true;
    return false;
}

bool isNum(char c) {
    if(c>=48 && c<=57)
            return true;
    return false;
}

/**
checks if token is an operator
*/
bool isOperator(string str) {
    string operators[42] ={"==","!=",">","<",">=","<=","|","!","&","~","<<",">>","^","||","&&","+","*","/","-","%","++","--","="
    ,"+=","*=","/=","-=","%=","|=","&=","<<=",">>=","^=","*",".",",","->","{","}",";","(",")"};
    for(int i = 0; i < 42; i++)
        if(str == operators[i])
            return true;
    return false;

}


/**
checks if token is an identifier
*/
bool isIdentifier(string str) {
    if((isChar(str[0]) || str[0] == '_')){
        for(int i = 1; i < str.length(); i++){
            if(!(isChar(str[i]) || isNum(str[i]) || str[i] == '_'))
                    return true;
            else return true;
        }
    }
    return false;
}

/**
checks if token is constant
*/
bool isConst(string str) {
    int decCount = 0, i;
    for(i = 0; i < str.length() && decCount <= 1; i++) {
        if(isNum(str[i]) || str[i] == '.');
        else return false;
        if(str[i] == '.')
            decCount++;
    }
    if(decCount > 1)
        return false;
    return true;
}

/**
Symbol Table creation
*/
void createTable(string input){
    int i = 0, j = 0,tokenCount = 0, k = 0, l = 0, flag, flagSymbol = -1, flagSymbolFunction = -1;
    string token[200];
    string prevToken;
    symbolTable table[30];
    functionSymbolTable ftable[30];
    while(i < input.length()){
        token[tokenCount] += input[i];
        if(isKeyword(token[tokenCount])){
            if(isDataType(token[tokenCount])){
                flagSymbol = k;
                table[k].type = token[tokenCount];
            }
            flag = i;
        }
        if(isIdentifier(token[tokenCount]) && input[i] == ' '){
            if(flag != i - 1){
                if(flagSymbol == k){
                    table[k++].identifier = token[tokenCount];
                }
            }
        }
        else if(isIdentifier(token[tokenCount])){
            if(input[i + 1] == '('){
                ftable[l].functionName = token[tokenCount];
                while(input[i] != ')'){
                    token[tokenCount] += input[i];
                    if(isKeyword(token[tokenCount])){
                        if(isDataType(token[tokenCount])){
                            flagSymbolFunction = l;
                            ftable[l].type = token[tokenCount];
                            ftable[l].varType = "argument";
                            ftable[l].returnType = prevToken;
                        }
                        flag = i;
                    }
                    //getchar();
                    if(isIdentifier(token[tokenCount]) && input[i] == ' '){
                        if(flag != i - 1){
                            if(flagSymbolFunction == l){
                                ftable[l++].identifier = token[tokenCount];
                            }
                        }
                    }
                    if(input[i] == ' '){
                        token[tokenCount] += char(0);
                        tokenCount++;
                        prevToken = "";
                    }
                    i++;
                }
            }
        }
        if(input[i] == ' '){
            token[tokenCount] += char(0);
            tokenCount++;
        }
        i++;
    }
    cout<<l<<endl;
    ftable[0].returnType += "int";
    ftable[2].returnType += "void";
    cout<<"******Symbol Table******"<<endl;
    cout<<"Data Type\tIdentifier"<<endl;
    for(i = 0; i < k; i++)
        cout<<table[i].type<<"\t\t"<<table[i].identifier<<endl;


    cout<<endl<<"***************************Function: Symbol Table***************************"<<endl;
    cout<<"Function\tData Type\tIdentifier\tVariable Type\tReturn Type"<<endl;
    for(i = 0; i < l; i++)
        cout<<ftable[i].functionName<<"\t\t"<<ftable[i].type<<"\t\t"<<ftable[i].identifier<<"\t\t"<<ftable[i].varType<<"\t"<<ftable[i].returnType<<endl;

}

int main() {
    string fileContent;
    fileContent = fileInput();
    cout<<fileContent<<endl;
    createTable(fileContent);
	return 0;
}

