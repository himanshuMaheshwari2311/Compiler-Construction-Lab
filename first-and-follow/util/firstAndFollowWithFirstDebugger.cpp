#include<iostream>
#include<string>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

#define maxCol 10
char grammar[20][maxCol];
string terminals, nonTerminals, epsilonNonTerminals;
int countOccurence[10];

void displayGrammar(int);
void getTerminals(int);
void getNonTerminals(int);
bool isPresent(char , string);
string firstOfNonTerminal(string, char, int);
int index(char , string);
int position(char, int, int);
void removeDuplicate(string&);

void displayGrammar(char grammar[][maxCol], int row){
    int i,j;
    cout<<"Grammar: "<<endl;
    for(i = 0; i < row; i++){
        for(j = 0; grammar[i][j]; j++)
            cout<<grammar[i][j];
        cout<<endl;
    }
    cout<<endl;
}

int position(char c, int row, int j){
    int i, itr = 0;
    for(i = 0; i < row; i++){
        if(grammar[i][0] == c)
            if(j == itr)
                return i;
            else itr++;
    }
}

bool isPresent(char c, string str){
    int i;
    for(i = 0; i < str.length(); i++)
        if(str[i] == c)
            return true;
    return false;
}

int index(char c, string str){
    int i;
    for(i = 0; i < str.length(); i++)
        if(str[i] == c)
            return i;
    return 0;
}

void getTerminals(int row){
    int i,j;
    for(i = 0; i < row; i++)
        for(j = 3; grammar[i][j];j++)
            if(grammar[i][j] > 96 && grammar[i][j] < 123)
                if(!isPresent(grammar[i][j], terminals))
                    terminals += grammar[i][j];
}

void getNonTerminals(int row){
    int i,j;
    for(i = 0; i < row; i++)
        for(j = 0; grammar[i][j]; j++)
            if(grammar[i][j] > 64 && grammar[i][j] < 91){
                if(!isPresent(grammar[i][j], nonTerminals))
                    nonTerminals += grammar[i][j];
                if(isPresent(grammar[i][0], nonTerminals) && grammar[i][3] == '#')
                    epsilonNonTerminals += grammar[i][j];
            }
}

void removeDuplicate(string &str){
    int i, j, k, len = str.length();
    for(i = 0; i < len; i++)
        for(j = i + 1; j < len;)
            if(str[j] == str[i]){
                for(k = j; k < len; k++)
                    str[k] = str[k + 1];
                len--;
            }
            else j++;
    str[len] = 0;
}

string firstOfNonTerminal(string production, char current, int row){
    int i,j;
    string first;
    if(isPresent(current, terminals)){
        return string(1, production[j]);
    }
    for(j = 3; production[j]; j++){
        //cout<<"Production: "<<production[j]<<" : "<<j<<" current: "<<current<<endl;
        if(isPresent(production[j], terminals)){
            first += production[j];
            break;
        }
        if(production[j] == '#'){
            first += '#';
        }
        if(isPresent(production[j], nonTerminals)){
            if(isPresent(production[j], epsilonNonTerminals)){
                /*cout<<"Index: "<<index(production[j], nonTerminals)<<" its count "<<countOccurence[index(production[j], nonTerminals)]<<endl;
                cout<<"Satisfied next production: "<<position(production[j],row,countOccurence[index(production[j], nonTerminals) - 1])<<endl;
                cout<<grammar[position(production[j],row,countOccurence[index(production[j], nonTerminals)])]<<endl;*/
                for(i = 0; i < countOccurence[index(production[j], nonTerminals)]; i++)
                    first += firstOfNonTerminal(grammar[position(production[j],row, i)], production[j], row);
            }
            else if(!isPresent(production[j], epsilonNonTerminals)){
                for(i = 0; i < countOccurence[index(production[j], nonTerminals)]; i++)
                    first += firstOfNonTerminal(grammar[position(production[j],row,i)], production[j], row);
                break;
            }
        }
    }
    return first;
}


int main(){
    int i = 0, j = 0, row, col;
    char c;
    int maxJ = 0;
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
    getTerminals(row);
    cout<<"Terminals: "<<terminals<<endl;
    getNonTerminals(row);
    cout<<"NonTerminals: "<<nonTerminals<<endl;
    cout<<"Null Terminals: "<<epsilonNonTerminals<<endl<<endl;
    string firsts[nonTerminals.length()];
    countOccurence[nonTerminals.length()] = {0};
    for(i = 0; i < row; i++)
        for(j = 0; j < nonTerminals.length(); j++)
            if(grammar[i][0] == nonTerminals[j])
                countOccurence[j]++;
    /*
    for(i = 0; i < nonTerminals.length(); i++)
        cout<<nonTerminals[i]<<" : "<<countOccurence[i]<<endl;
    */
    for(i = 0; i < nonTerminals.length(); i++){
        for(j = 0; j < countOccurence[i]; j++){
            //cout<<j<<" : "<<nonTerminals[i]<<" : position "<<position(nonTerminals[i],row,j)<<endl;
            firsts[i] += firstOfNonTerminal(grammar[position(nonTerminals[i], row,j)], nonTerminals[i], row);
        }
        removeDuplicate(firsts[i]);
        cout<<nonTerminals[i]<<" : "<<firsts[i]<<endl;
    }
    return 0;
}
