#include<iostream>
#include<conio.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

#define maxCol 10
/*epsilon = #*/
bool isPresent(string str, char c){
    int i;
    for(i = 0; i < str.length(); i++)
        if(str[i] == c)
            return true;
    return false;
}

string getNonTerminals(string grammar[][maxCol],int row, int col){
    int i,j,k = 0;
    string nonTerminals;
    for(i = 0; i < row; i++)
        for(j = 0; j < col; j++)
            if(grammar[i][j][k] > 64 && grammar[i][j][k] < 91)
                if(!isPresent(nonTerminals, grammar[i][j][k]))
                    nonTerminals += grammar[i][j][k];
    return nonTerminals;
}

string getTerminals(string grammar[][maxCol],int row, int col){
    int i,j,k = 0;
    string terminals;
    for(i = 0; i < row; i++)
        for(j = 0; j < col; j++)
            if(grammar[i][j][k] > 96 && grammar[i][j][k] < 123)
                if(!isPresent(terminals, grammar[i][j][k]))
                    terminals += grammar[i][j][k];
    return terminals;
}

void displayGrammar(string grammar[][maxCol], int row, int col){
    int i,j;
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++)
            cout<<grammar[i][j];
        cout<<endl;
    }
}

string getEpsilonNonTerminals(string grammar[][maxCol], string nonTerminals, int row, int col){
    int i,j;
    string epsilonNonTerminals;
    for(i = 0; i < row; i++)
        if(isPresent(nonTerminals,grammar[i][0][0]) && grammar[i][3][0] == '#'){
            epsilonNonTerminals.append(grammar[i][0]);
            cout<<i<<" : "<<grammar[i][0]<<endl;
        }
    return epsilonNonTerminals;
}
string firstOfGrammar(string grammar[][maxCol], string production[], int row, int col, string terminals, string nonTerminals, string epsilonNonTerminals, char current){
    int i, j;
    string first;
    if(isPresent(terminals, current)){
        cout<<"Here"<<endl;
        return string(1,current);
    }
    for(j = 3; production[j][0]; j++){
        if(isPresent(terminals, production[j][0])){
            first += production[3][0];
            break;
        }
        /*else if(isPresent(nonTerminals, production[j][0])) {
            if(!isPresent(epsilonNonTerminals, production[j][0]))
                first += firstOfGrammar(grammar, production, row, col, terminals, nonTerminals, epsilonNonTerminals, production[j][0]);
        }*/
    }
    cout<<first<<endl<<"Returning...."<<endl;
    return first;
}



int main(){
    string grammar[10][maxCol];
    string terminals,nonTerminals,epsilonNonTerminals;
    int i = 0, j = 0, row, col;
    char c;
    int maxJ = 0;
    while((c = getchar()) != '$'){
        while(c != '\n'){
            grammar[i][j] = string(1, c);
            j++;
            maxJ = j > maxJ ? j : maxJ;
            c = getchar();
        }
        j = 0;
        i++;
    }
    row = i;
    col = maxJ;
    displayGrammar(grammar, row, col);
    terminals = getTerminals(grammar, row, col);
    nonTerminals = getNonTerminals(grammar, row, col);
    string first[nonTerminals.length()];
    epsilonNonTerminals = getEpsilonNonTerminals(grammar, nonTerminals, row, col);
    cout<<terminals<<endl;
    cout<<nonTerminals<<endl;
    cout<<epsilonNonTerminals<<endl;
    for(i = 0; i < row; i++){
        cout<<i<<endl;
        first[i] += firstOfGrammar(grammar, grammar[i],row, col, terminals, nonTerminals, epsilonNonTerminals, nonTerminals[i]);
        cout<<nonTerminals[i]<<" : "<<first[i]<<endl;
    }
}
