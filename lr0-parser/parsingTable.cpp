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
Generate DFA
*/
void initDfa(){
    int i, j, k;
    for(i = 0; i < terminals.length(); i++)
        dfa[0][i] = string(1, terminals[i]);
    for(j = 0; j < nonTerminals.length(); j++)
        dfa[0][i + j] = string(1, nonTerminals[j]);
    for(i = 0; i < state.size(); i++){
        for(k = 0; k < nonTerminals.length() + terminals.length(); k++)
            for(j = 0; j < state[i].input.length(); j++)
                if(dfa[0][k][0] == state[i].input[j])
                    dfa[i + 1][k] += toString(state[i].to[j]);
            }
}





/**
Generate LR Parsing Table
*/
void generateParsingTable(){
    int i, j, k, l;
    string temp, str;
    string aug;
    aug += augmentedGrammar[0];
    removeDot(aug);
    cout<<endl;
    for(i = 0; i < terminals.length(); i++)
        parsingTable[0][i] = string(1, terminals[i]);
    for(j = 0; j < nonTerminals.length(); j++)
        parsingTable[0][i + j] = string(1, nonTerminals[j]);
    parsingTable[0][i + j] = '$';
    for(i = 0; i < state.size(); i++){
        for(j = 0; j < nonTerminals.length() + terminals.length(); j++)
            if(state[i].input.length() != 0 && dfa[i + 1][j][0] != 0){
                if(dfa[0][j][0] >= 65 && dfa[0][j][0] <= 91)
                    parsingTable[i + 1][j] +=  dfa[i + 1][j];
                else parsingTable[i + 1][j] += "S" + dfa[i + 1][j];
            }
            else if(state[i].input.length() == 0){
                str = "";
                for(l = 0; l < state[i].production[0][l]; l++)
                        str += state[i].production[0][l];
                removeDot(str);
                for(k = 0; k <= row; k++){
                    temp = "";
                    for(l = 0; l < augmentedGrammar[k][l]; l++)
                        temp += augmentedGrammar[k][l];
                    temp[l] = 0;
                    removeDot(temp);
                    if(!temp.compare(str) && !(dfa[0][j][0] >= 65 && dfa[0][j][0] <= 91)){
                        if(!str.compare(aug))
                            parsingTable[i + 1][nonTerminals.length() + terminals.length()] = "accept";
                        else{
                            parsingTable[i + 1][j] += "R" + string(1, k  + '0');
                            parsingTable[i + 1][nonTerminals.length() + terminals.length()] = "R" + string(1, k + '0');
                        }
                    }
                }
            }
    }
    displayParsingTable();
}
