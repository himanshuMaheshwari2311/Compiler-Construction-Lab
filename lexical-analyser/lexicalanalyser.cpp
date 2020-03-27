#include<iostream>
#include<conio.h>
#include<string.h>

using namespace std;

bool isKeyword(string);
bool isChar(char);
bool isNum(char);
bool isOperator(string);
bool isConst(string);
bool isIdentifier(string);
void displayString(string);

bool isKeyword(string str) {
    string keywords[65] = {"asm","auto","bool","break","case","catch","char","class","const","const_cast","continue","default","delete","do",
    "double","dynamic_cast","else","enum","explicit","export","extern","false","float","for","friend","goto","if","inline","int","long",
    "mutable","namespace","new","operator","private","protected","public","register","reinterpret_cast","return","short","signed","sizeof","static",
    "static_cast","struct","switch","template","this","throw","true","try","typedef","typeid","typename","union","unsigned","using","virtual",
    "void","volatile","wchar_t","while","main()"};
    for(int i = 0; i < 65; i++)
        if(str == keywords[i])
            return true;
    return false;
}

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

bool isOperator(string str) {
    string operators[42] ={"==","!=",">","<",">=","<=","|","!","&","~","<<",">>","^","||","&&","+","*","/","-","%","++","--","="
    ,"+=","*=","/=","-=","%=","|=","&=","<<=",">>=","^=","*",".",",","->","{","}",";","(",")"};
    for(int i = 0; i < 42; i++)
        if(str == operators[i])
            return true;
    return false;

}

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

void displayString(string str) {
    int i = 0;
    while(str[i]) {
        cout<<str[i];
        i++;
    }
    cout<<endl;
}

int main() {
    int i = 0, tokenCount = 0, k = 0, flag, lexemeCount = 0;
    char input[100];
    string token[20];
    cout<<"Enter the expression for lexical analysis: ";
    cin.getline(input, 100);
    while(i < strlen(input)){
        token[tokenCount] += input[i];
        if(isKeyword(token[tokenCount])){
            cout<<"Keyword: ";
            displayString(token[tokenCount]);
            flag = i;
            lexemeCount++;
        }
        if(isIdentifier(token[tokenCount]) && input[i] == ' '){
            if(flag != i - 1){
                cout<<"Identifier: ";
                displayString(token[tokenCount]);
                lexemeCount++;
            }
        }
        if(isOperator(token[tokenCount]) && input[i + 1] == ' '){
            cout<<"Operator: ";
            displayString(token[tokenCount]);
            lexemeCount++;
        }
        if(isConst(token[tokenCount]) && input[i + 1] == ' '){
            cout<<"Constant: ";
            displayString(token[tokenCount]);
            lexemeCount++;
        }
        if(input[i] == ' '){
            token[tokenCount] += char(0);
            tokenCount++;
        }
        i++;
    }
    cout<<endl<<tokenCount<<endl;
    return 0;
}
