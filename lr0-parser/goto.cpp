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
Goto function for LR0 Parser
*/
void gotoState(int index, char transitioSymbol){
    int i, j, occurrence = 0, dotIndex;
    string pushElement;
    for(i = 0; i < state[index].production.size(); i++){
        dotIndex = getDotIndex(state[index].production[i]);
        if(transitioSymbol == state[index].production[i][dotIndex + 1])
            occurrence++;
    }
    states *current = new states;
    for(i = 0; i < occurrence; i++){
        pushElement = selectAndTransformProduction(index, transitioSymbol, i);
        current->production.push_back(pushElement);
    }
    current->ipPointer = 0;
    current->from = index;
    current->transitionSymbol = transitioSymbol;
    state.push_back(*current);
    closure(state.size() - 1);
    state[index].to[state[index].ipPointer] = removeIfRepeatedState(state[state.size() - 1], state.size() - 1);
}


/**
Select the production and Shift the dot
*/
string selectAndTransformProduction(int index, char transitionSymbol, int occurrence){
    int i, dotIndex;
    string str, temp;
    for(i = 0; i < state[index].production.size(); i++){
        dotIndex = getDotIndex(state[index].production[i]);
        if(state[index].production[i][dotIndex + 1] == transitionSymbol){
            if(occurrence == 0){
                str = state[index].production[i];
                break;
            }
            else occurrence--;
        }
    }
    removeDot(str);
    char dot = '.';
    for(i = 3; i < str.length(); i++)
        if(str[i] == transitionSymbol){
            str.insert(i+1, string(1,dot));
            break;
        }
    return str;
}


/**
Remove duplicate
*/
int removeIfRepeatedState(states &current, int index){
    int i, j = 0, k;
    int stateNo;
    bool equalProduction;
    for(i = 0; i < index; i++){
        if(i != index){
            if(current.production.size() != state[i].production.size()){
                continue;
            }
            for(j = 0; j < current.production.size(); j++){
                if(current.production[j].compare(state[i].production[j]))
                    break;
            }
            if(j == current.production.size()){
                stateNo = i;
                state.erase(state.begin() + index);
                return stateNo;
            }
        }
    }
    return i;
}


/**
Generate States using Goto and Closure
*/
void generateInitialState(){
    int m;
    string pushElement = "";
    for(m = 0; augmentedGrammar[0][m]; m++)
        pushElement += augmentedGrammar[0][m];
    states *current = new states;
    current->production.push_back(pushElement);
    current->ipPointer = 0;
    current->type = 1;
    current->from = -1;
    current->transitionSymbol = 0;
    state.push_back(*current);
    closure(0);
}

/**
Generate all states
*/
void generateStates(){
    int i, j;
    for(i = 0; i < state.size(); i++){
        for(j = 0; j < state[i].input.length(); j++){
            gotoState(i,state[i].input[state[i].ipPointer]);
            state[i].ipPointer++;
        }
    }
}
