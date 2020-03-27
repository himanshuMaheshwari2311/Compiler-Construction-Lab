#include<iostream>
#include<string.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include "utilityFunction.cpp"
#include "io.cpp"
#include "goto.cpp"
#include "closure.cpp"
#include "parsingTable.cpp"
#include "parseString.cpp"

using namespace std;

/*Variables*/
#define maxCol 10
typedef struct{
    vector<string> production;
    int type;
    string input;
    int ipPointer;
    int to[10];
    int from;
    char transitionSymbol;
}states;
int row;
char grammar[20][maxCol];
char augmentedGrammar[20][maxCol];
vector<states> state;
string dfa[20][maxCol];
string parsingTable[20][maxCol];
string nonTerminals;
string terminals;


/*Function Prototype*/
void getGrammar(char [][maxCol]);
void displayGrammar(char [][maxCol], int, int);
void getTerminals();
void getNonTerminals(int);
bool isPresent(char, string);
string toString(int);
void generateAugmentedGrammar(int);
int getDotIndex(string);
void closure(int);
void removeDuplicateProduction(states&);
void removeDuplicateTransitionSymbol(string&);
void printStatesProduction();
void generateInitialState();
void generateStates();
void gotoState(int, char);
string selectAndTransformProduction(int, char);
void removeDot(string&);
int removeIfRepeatedState(states &,int i);
void initDfa();
void printDfa();
void generateParsingTable();
void displayParsingTable();
string frontAppend(string, string);
string frontRemove(string);
void parseString();
string getAction(int, char);


/**
Get the Set of Non-Terminals
*/
void getNonTerminals(int row){
    int i,j;
    for(i = 0; i < row; i++)
        for(j = 0; grammar[i][j]; j++)
            if(grammar[i][j] > 64 && grammar[i][j] < 91){
                if(!isPresent(grammar[i][j], nonTerminals))
                    nonTerminals += grammar[i][j];
            }
    cout<<"Non-Terminals: "<<nonTerminals<<endl;
}


/**
Check Presence of character in string
*/
bool isPresent(char c, string str){
    int i;
    for(i = 0; i < str.length(); i++)
        if(str[i] == c)
            return true;
    return false;
}


/**
Converts number to String
*/
string toString(int num){
    string temp, str;
    int i;
    while(num){
        temp += num % 10 + '0';
        num = num/10;
    }
    for(i = 0; temp[i]; i++)
        str += temp[temp.length() - i - 1];
    return str;
}


/**
Generate Augmented Grammar
*/
void generateAugmentedGrammar(int row){
    int i, j, k = 0;
    augmentedGrammar[0][0] = 'Z';
    augmentedGrammar[0][1] = '-';
    augmentedGrammar[0][2] = '>';
    augmentedGrammar[0][3] = '.';
    augmentedGrammar[0][4] = 'S';

    for(i = 0; i < row; i++){
        for(j = 0; grammar[i][j]; j++){
            if(j == 2){
                augmentedGrammar[i + 1][k++] = grammar[i][j];
                augmentedGrammar[i + 1][k++] = '.';
            }
            else augmentedGrammar[i + 1][k++] = grammar[i][j];
        }
        k = 0;
    }
    displayGrammar(augmentedGrammar, row + 1, 2);
}


/**
Get Dot Index
*/
int getDotIndex(string str){
    int i;
    for(i = 0; str[i]; i++)
        if(str[i] == '.')
            return i;
}

/**
Get Terminals
*/
void getTerminals(int row){
    int i,j;
    for(i = 0; i < row; i++)
        for(j = 3; grammar[i][j];j++)
            if(!(grammar[i][j] > 64 && grammar[i][j] < 91))
                if(!isPresent(grammar[i][j], terminals))
                    terminals += grammar[i][j];
    cout<<"Terminals: "<<terminals<<endl<<endl;
}


/**
Remove dot from string
*/
void removeDot(string &str){
    int i, j, k, len = str.length();
    for(i = 0; i < len; i++)
        if(str[i] == '.'){
            for(k = i; k < len; k++)
                str[k] = str[k + 1];
            len--;
        }
    str[i] = 0;
}

/**
Print DFA
*/
void printDfa(){
    int i, j;
    for(i = 0; i <= state.size(); i++){
        if(i == 0)
            cout<<"\t";
        else cout<<i - 1<<"\t";
        for(j = 0; j < terminals.length() + nonTerminals.length(); j++)
            cout<<dfa[i][j]<<"\t";
        cout<<endl;
    }
}


/**
Display Parsing Table
*/
void displayParsingTable(){
    int i, j;
    for(i = 0; i <= state.size(); i++){
        if(i == 0)
            cout<<"State\t";
        else cout<<i - 1<<"\t";
        for(j = 0; j <= terminals.length() + nonTerminals.length(); j++)
            cout<<parsingTable[i][j]<<"\t";
        cout<<endl;
    }
}
