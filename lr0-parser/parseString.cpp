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
Push in stack
*/
string frontAppend(string str, string prod){
    string temp;
    temp = str;
    str="";
    str += prod + temp;
    return str;
}


/**
Pop from stack
*/
string frontRemove(string str){
    string temp = str;
    int i;
    str = "";
    for(i = 1; i < temp.length(); i++)
        str += temp[i];
    return str;
}


/**
get the action from parsing table
*/
string getAction(int index, char transitionSymbol){
    int i, j;
    for(i = 0; i < state.size(); i++){
        if(i == index)
            for(j = 0; j <= terminals.length() + nonTerminals.length(); j++)
                if(transitionSymbol == parsingTable[0][j][0])
                    return parsingTable[i + 1][j];
    }
}


/**
Take string input and parse it for the given grammar
*/
void parseString(){
    int choice;
    do{
        string input, S = "0", symbol = "S", action, acceptState, production, go;
        int strItr = 0;
        int i, j, k;
        int grammarIndex, len = 0;
        char actionElement;
        int recCount = 0;
        cout<<endl<<"Enter the string you want to parse: ";
        cin>>input;
        cout<<"Stack\t\tSymbol\t\tInput\t\tAction\n";
        strItr = 0;
        for(i = 0; i <= state.size(); i++)
            if(!parsingTable[i + 1][terminals.length() + nonTerminals.length()].compare("accept")){
                acceptState = toString(i);
                break;
            }
        do{
            action = getAction(S[0] - '0', input[strItr]);
            if(action[0] == 'S'){
                if(strItr == 0)
                    symbol = frontRemove(symbol);
                S = frontAppend(S, string(1, action[1]));
                symbol = frontAppend(symbol, string(1, input[strItr]));
                strItr++;
                cout<<S<<"\t\t"<<symbol<<"\t\t"<<input<<"\t"<<action<<endl;
            }
            else if(action[0] == 'R'){
                len = 0;
                grammarIndex = action[1] - '0';
                production = augmentedGrammar[grammarIndex];
                removeDot(production);
                for(i = 3; production[i]; i++)
                    len++;
                for(i = 0; i < len; i++){
                    S = frontRemove(S);
                    symbol = frontRemove(symbol);
                    actionElement = S[0];
                }
                go = getAction(actionElement - '0', production[0]);
                S = frontAppend(S, go);
                symbol = frontAppend(symbol, string(1, production[0]));
                cout<<S<<"\t\t"<<symbol<<"\t\t"<<input<<"\t"<<action<<" "<<production<<endl;
            }
            if(!action.compare(" "))
                break;
        }while((symbol.compare("S") && S.compare(acceptState)));
        cout<<"Press 1 to parse again and 2 to quit: ";
        cin>>choice;
    }while(choice != 2);
}
