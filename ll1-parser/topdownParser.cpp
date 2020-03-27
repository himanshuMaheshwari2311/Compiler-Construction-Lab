#include<iostream>
#include<string.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include "firstfollow.cpp"

using namespace std;

#define maxCol 10

void parsingTableInit(string [][maxCol]);
void displayParsingTable(string [][maxCol]);
void generateParsingTable(string [][maxCol], string, string, int);
int getY(string [][maxCol], char);



/*Initialize parsing Table*/
void parsingTableInit(string parsingTable[][maxCol]){
    int i, j;
    for(i = 0; i <= nonTerminals.length(); i++){
        for(j = 0; j <= terminals.length(); j++){
            if(i == 0 && j == 0)
                parsingTable[i][j] == "-";
            else if(j == 0)
                parsingTable[i][j] = string(1,nonTerminals[i - 1]);
            else if(i == 0)
                parsingTable[i][j] = string(1,terminals[j - 1]);
            else parsingTable[i][j] = "";
        }
    }

}

/*Display Parsing Table*/
void displayParsingTable(string parsingTable[][maxCol]){
    int i, j;
    for(i = 0; i <= nonTerminals.length(); i++){
        for(j = 0; j <= terminals.length(); j++)
            if(parsingTable[i][j][0] == '#')
                cout<<"$"<<"\t\t";
            else
                cout<<parsingTable[i][j]<<"\t\t";
        cout<<endl;
    }
}

/*Get Y coordinate of terminal in parsing table*/
int getY(string parsingTable[][maxCol], char c){
    int i;
    for(i = 1; i <= terminals.length(); i++)
        if(parsingTable[0][i][0] == c)
            return i;
}

/*Generate Parsing Table*/
void generateParsingTable(string parsingTable[][maxCol], string parsingFirst[], string follows[], int row){
    int i, j, k, x, y;
    for(i = 0; i < row; i++){
        x = index(grammar[i][0], nonTerminals) + 1;
        for(j = 0; parsingFirst[i][j]; j++){
            if(parsingFirst[i][j] == '#'){
                for(k = 0; follows[x - 1][k]; k++){
                    if(follows[x - 1][k] == '$')
                        y = getY(parsingTable, '#');
                    else {
                        y = getY(parsingTable, follows[x - 1][k]);
                    }
                    parsingTable[x][y] += grammar[i];
                }
            }
            else{
                y = getY(parsingTable, parsingFirst[i][j]);
                parsingTable[x][y] += grammar[i];
            }
        }
    }
}

/*Main function*/
int main(){
    int i = 0, j = 0,k = 0, row, col, recCount = 0, maxJ = 0;
    char c;
    /*Get Grammar*/
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
    displayGrammar(grammar, row);

    /*Get terminals, non-terminals, null production terminals*/
    getTerminals(row);
    cout<<"Terminals: "<<terminals<<endl;
    getNonTerminals(row);
    cout<<"NonTerminals: "<<nonTerminals<<endl;
    cout<<"Null Terminals: "<<epsilonNonTerminals<<endl;
    string firsts[nonTerminals.length()];
    string follows[nonTerminals.length()];
    countOccurence[nonTerminals.length()] = {0};

    /*Count production of each non-terminal*/
    for(i = 0; i < row; i++)
        for(j = 0; j < nonTerminals.length(); j++)
            if(grammar[i][0] == nonTerminals[j])
                countOccurence[j]++;

    /*Compute first of every non-terminal*/
    cout<<"First of Grammar"<<endl;
    for(i = 0; i < nonTerminals.length(); i++){
        for(j = 0; j < countOccurence[i]; j++){
            firsts[i] += firstOfNonTerminal(grammar[position(nonTerminals[i], row,j)], nonTerminals[i], row, 3);
        }
        removeDuplicate(firsts[i]);
        cout<<nonTerminals[i]<<" : "<<firsts[i]<<endl;
    }
    cout<<endl;

    /*Compute follow of every non-terminal*/
    cout<<"Follow of Grammar"<<endl;
    for(i = 0; i < nonTerminals.length(); i++){
        follows[i] += followOfNonTerminal(nonTerminals[i], row, follows, recCount);
        removeDuplicate(follows[i]);
        removeEpsilon(follows[i]);
        cout<<nonTerminals[i]<<" : "<<follows[i]<<endl;
    }

    /*Generate parsing table*/
    string parsingTable[10][maxCol];
    parsingTableInit(parsingTable);
    cout<<endl;
    string parsingFirst[row];
    for(i = 0; i < row; i++){
        parsingFirst[i] += firstOfNonTerminal(grammar[i], grammar[i][0], row, 3);
        removeDuplicate(parsingFirst[i]);
        cout<<grammar[i][0]<<" : "<<parsingFirst[i]<<endl;
    }
    generateParsingTable(parsingTable, parsingFirst, follows, row);
    displayParsingTable(parsingTable);
    cout<<endl;
    return 0;
}
