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
        cout<<"Transition Symbol: "<<state[i].input<<endl;
        cout<<"From State: "<<state[i].from<<" Transition Symbol: "<<state[i].transitionSymbol<<endl<<endl;
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
    current->from = -1;
    current->transitionSymbol = 0;
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
    int choice, recCount;
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
            recCount++;
            if(recCount > 100){
                cout<<"String cannot be parsed"<<endl;
                break;
            }
        }while((symbol.compare("S") && S.compare(acceptState)));
        cout<<"Press 1 to parse again and 2 to quit: ";
        cin>>choice;
    }while(choice != 2);
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
    initDfa();
    generateParsingTable();
    parseString();
    return 0;
}
