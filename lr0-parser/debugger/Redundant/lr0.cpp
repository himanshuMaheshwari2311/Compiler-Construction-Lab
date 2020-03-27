#include<iostream>
#include<string.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>

using namespace std;

/*Variables*/
#define maxCol 10
typedef struct{
    vector<string> production;
    int type;
    string input;
    int ipPointer;
}states;
int row;
char grammar[20][maxCol];
char augmentedGrammar[20][maxCol];
vector<states> state;
char dfa[20][maxCol];
string nonTerminals;
string terminals;

/*Function Prototype*/
void getGrammar(char [][maxCol]);
void displayGrammar(char [][maxCol], int, int);
void getTerminals();
void getNonTerminals(int);
bool isPresent(char, string);
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
void removeIfRepeatedState(states &,int i);

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
Remove Duplicate Production
*/
void removeDuplicateProduction(states &state){
    int i, j;
    for(i = 1; i < state.production.size(); i++)
        for(j = i + 1; j < state.production.size(); j++)
            if(!state.production[i].compare(state.production[j])){
               state.production.erase(state.production.begin() + j);
               j--;
            }
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
Removes extra symbol from transition symbols
*/
void removeDuplicateTransitionSymbol(string &str){
    int i, j, k, len = str.length();
    for(i = 0; i < len; i++)
        for(j = i + 1; j < len; j++)
            if(str[i] == str[j]){
                str.erase( j, 1 );
                len--;
            }
    str[len] = 0;
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
        cout<<"Transition Symbol: "<<state[i].input<<endl<<endl;
    }
}

/**
Compute Closure of state
*/
void closure(int index){
    int i, j, k;
    char nextSymbol;
    string str;
    for(i = 0; i < state[index].production.size(); i++){
        nextSymbol = state[index].production[i][getDotIndex(state[index].production[i]) + 1];
        if(nextSymbol != 0)
            state[index].input += nextSymbol;
        if(isPresent(nextSymbol, nonTerminals))
            for(j = 0; j <= row; j++)
                if(nextSymbol == augmentedGrammar[j][0]){
                    str = "";
                    for(k = 0; augmentedGrammar[j][k]; k++)
                        str += augmentedGrammar[j][k];
                    state[index].production.push_back(str);
                    removeDuplicateProduction(state[index]);
                    removeDuplicateTransitionSymbol(state[index].input);
                }
    }

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
    state.push_back(*current);
    closure(0);
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
void removeIfRepeatedState(states &current, int index){
    int i, j = 0, k;
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
                state.erase(state.begin() + index);
            }
        }
    }
}

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
    state.push_back(*current);
    closure(state.size() - 1);
    removeIfRepeatedState(state[state.size() - 1], state.size() - 1);
}

/**
Generate all states
*/
void generateStates(){
    int i, j;
    for(i = 0; i < state.size(); i++)
        for(j = 0; j < state[i].input.length(); j++){
            gotoState(i,state[i].input[state[i].ipPointer]);
            state[i].ipPointer++;
        }
}


/**
Main function
*/
int main(){
    getGrammar(grammar);
    getNonTerminals(row);
    getTerminals(row);
    displayGrammar(grammar, row, 1);
    generateAugmentedGrammar(row);
    generateInitialState();
    generateStates();
    printStatesProduction();
    return 0;
}
