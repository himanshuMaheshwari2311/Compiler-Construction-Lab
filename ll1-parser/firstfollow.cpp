#include<iostream>
#include<string.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

#define maxCol 10
char grammar[20][maxCol];
string terminals, nonTerminals, epsilonNonTerminals;
int countOccurence[10];
char next;

void displayGrammar(int);
void getTerminals(int);
void getNonTerminals(int);
bool isPresent(char , string);
string firstOfNonTerminal(string, char, int, int);
string followOfNonTerminal(char, int);
int index(char , string);
int position(char, int, int);
void removeDuplicate(string&);
void removeEpsilon(string&);

/*Display Grammar*/
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

/*For selection of production*/
int position(char c, int row, int j){
    int i, itr = 0;
    for(i = 0; i < row; i++){
        if(grammar[i][0] == c)
            if(j == itr)
                return i;
            else itr++;
    }
}

/*Presence*/
bool isPresent(char c, string str){
    int i;
    for(i = 0; i < str.length(); i++)
        if(str[i] == c)
            return true;
    return false;
}

/*For follow presence function*/
bool isPresentRHS(char c, string str){
    int i;
    for(i = 3; i < str.length(); i++)
        if(str[i] == c)
            return true;
    return false;
}

/*Get index*/
int index(char c, string str){
    int i;
    for(i = 0; i < str.length(); i++)
        if(str[i] == c)
            return i;
    return -1;
}

/*Extract terminals*/
void getTerminals(int row){
    int i,j;
    for(i = 0; i < row; i++)
        for(j = 3; grammar[i][j];j++)
            if(!(grammar[i][j] > 64 && grammar[i][j] < 91))
                if(!isPresent(grammar[i][j], terminals))
                    terminals += grammar[i][j];
}

/*Extract non-terminals*/
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

/*To create set*/
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

/*Follow utility function*/
void removeEpsilon(string &str){
    int i, j, k, len = str.length();
    for(i = 0; i < len; i++)
        if(str[i] == '#'){
            for(k = i; k < len; k++)
                str[k] = str[k + 1];
            len--;
        }
        else j++;
    str[len] = 0;
}

/*Function to compute first of non-Terminal*/
string firstOfNonTerminal(string production, char current, int row, int in){
    int i,j;
    string first;
    if(isPresent(current, terminals)){
        return string(1, production[j]);
    }
    for(j = in; production[j]; j++){
        if(isPresent(production[j], terminals)){
            first += production[j];
            break;
        }
        if(production[j] == '#'){
            first += '#';
        }
        if(isPresent(production[j], nonTerminals)){
            if(isPresent(production[j], epsilonNonTerminals)){
                for(i = 0; i < countOccurence[index(production[j], nonTerminals)]; i++)
                    first += firstOfNonTerminal(grammar[position(production[j],row, i)], production[j], row, 3);
            }
            else if(!isPresent(production[j], epsilonNonTerminals)){
                for(i = 0; i < countOccurence[index(production[j], nonTerminals)]; i++)
                    first += firstOfNonTerminal(grammar[position(production[j],row,i)], production[j], row, 3);
                break;
            }
        }
    }
    return first;
}

/*Function to compute follow of non-terminal*/
string followOfNonTerminal(char current, int row, string follows[], int &recCount){
    string follow;
    int itr;
    if(current == 'S'){
        follow += '$';
    }
    if(recCount > 100){
        follows[index(current, nonTerminals)] += follows[index(next, nonTerminals)];
        return follow;
    }
    int i, j, in;
    for(i = 0; i < row; i++){
        if(isPresentRHS(current, grammar[i])){
            in = index(current, grammar[i]);
            if(grammar[i][in + 1] == '\0'){
                recCount++;
                next = grammar[i][in];
                follow += followOfNonTerminal(grammar[i][0], row, follows, recCount);
            }
            else if(isPresent(grammar[i][in + 1], epsilonNonTerminals) && grammar[i][in + 1] != 0){
                follow += firstOfNonTerminal(grammar[i], grammar[i][in + 1], row, in + 1);
                if(grammar[i][in + 2] == 0 || grammar[i][in + 1] == 0) {
                        recCount++;
                        next = grammar[i][in];
                        follow += followOfNonTerminal(grammar[i][0], row, follows, recCount);
                }
                for(itr = in + 1; grammar[i][itr]; itr++)
                    if(!isPresent(grammar[i][itr],epsilonNonTerminals))
                        break;
                if(itr == strlen(grammar[i])){
                    recCount++;
                    next = grammar[i][in];
                    follow += followOfNonTerminal(grammar[i][0], row, follows, recCount);
                }
            }
            else if(isPresent(grammar[i][in + 1], nonTerminals) && grammar[i][in + 1] != 0){
                follow += firstOfNonTerminal(grammar[i], grammar[i][in + 1], row, in + 1);
            }
            else if(isPresent(grammar[i][in + 1], terminals) && grammar[i][in + 1] != 0){
                follow += grammar[i][in + 1];
            }
        }
    }
    return follow;
}

