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
Input Grammar
*/
void getGrammar(char grammar[][maxCol]){
    int i = 0, j = 0,k = 0, recCount = 0, maxJ = 0;
    char c;
    cout<<"Enter the grammar: "<<endl;
    while((c = getchar()) != '$'){
        while(c != '\n'){
            grammar[i][j] = char(c);
            j++;
            maxJ = j > maxJ ? j : maxJ;
            c = getchar();
        }
        grammar[i][j] = '\0';
        j = 0;
        i++;
    }
    row = i;
}


/**
Display Grammar
*/
void displayGrammar(char grammar[][maxCol], int row, int type){
    int i,j;
    if(type == 1)
        cout<<"Grammar: "<<endl;
    else cout<<"Augmented Grammar: "<<endl;
    for(i = 0; i < row; i++){
        for(j = 0; grammar[i][j]; j++)
            cout<<grammar[i][j];
        cout<<endl;
    }
    cout<<endl;
}


/**
Print states Production
*/
void printStatesProduction(){
    int stateNo = state.size(), productionNo, i, j;
    for(i = 0; i < stateNo; i++){
        cout<<"State: "<<i<<endl;
        productionNo = state[i].production.size();
        for(j = 0; j < productionNo; j++){
            cout<<state[i].production[j]<<endl;
        }
        cout<<"Transition Symbol: "<<state[i].input<<endl;
        cout<<"From State: "<<state[i].from<<" Transition Symbol: "<<state[i].transitionSymbol<<endl<<endl;
    }
}
