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
int countRHSOccurence[10];

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

string followOfNonTerminal(char current, int row, string follows[]){
    string follow;
    int itr;
    if(current == 'S'){
        follow += '$';
    }
    int i, j, in;
    for(i = 0; i < row; i++){
        //cout<<i<<endl;
        if(isPresentRHS(current, grammar[i])){
            in = index(current, grammar[i]);
            //cout<<grammar[i][in]<<" Terminal at index: "<<in<<endl;
            if(grammar[i][in + 1] == '\0'){
                //cout<<grammar[i][in]<<" and its follow "<<grammar[i][in + 1]<<" as"<<endl;
                follow += followOfNonTerminal(grammar[i][0], row, follows);
                //cout<<grammar[i][in]<<"'s follow"<<follow<<endl;
            }
            else if(isPresent(grammar[i][in + 1], epsilonNonTerminals) && grammar[i][in + 1] != 0){
                //cout<<grammar[i][in + 1]<<" Next is epsilonNonTerminal"<<endl;
                follow += firstOfNonTerminal(grammar[i], grammar[i][in + 1], row, in + 1);
                if(grammar[i][in + 2] == 0 || grammar[i][in + 1] == 0) {
                        //cout<<"End Null production"<<endl;
                        follow += followOfNonTerminal(grammar[i][0], row, follows);
                }
                for(itr = in + 1; grammar[i][itr]; itr++)
                    if(!isPresent(grammar[i][itr],epsilonNonTerminals))
                        break;
                if(itr == strlen(grammar[i]))
                    follow += followOfNonTerminal(grammar[i][0], row, follows);
                //cout<<grammar[i][in]<<"'s Follow: "<<follow<<endl;
            }
            else if(isPresent(grammar[i][in + 1], nonTerminals) && grammar[i][in + 1] != 0){
                //cout<<grammar[i][in + 1]<<" Next is Nonterminal"<<endl;
                follow += firstOfNonTerminal(grammar[i], grammar[i][in + 1], row, in + 1);
                //cout<<grammar[i][in]<<"'s Follow: "<<follow<<endl;
            }
            else if(isPresent(grammar[i][in + 1], terminals) && grammar[i][in + 1] != 0){
                follow += grammar[i][in + 1];
                //cout<<grammar[i][in + 1]<<"Next is Terminal"<<endl;
            }
        }
    }
    return follow;
}



int main(){
    int i = 0, j = 0,k = 0, row, col;
    char c;
    int maxJ = 0;
    cout<<"Enter the grammar: "<<endl;
    /*Get Grammar*/
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
    countRHSOccurence[nonTerminals.length()] = {0};

    /*Count production of each non-terminal*/
    for(i = 0; i < row; i++)
        for(j = 0; j < nonTerminals.length(); j++)
            if(grammar[i][0] == nonTerminals[j])
                countOccurence[j]++;

    /*Compute first of every non-terminal*/
    for(i = 0; i < nonTerminals.length(); i++){
        for(j = 0; j < countOccurence[i]; j++){
            firsts[i] += firstOfNonTerminal(grammar[position(nonTerminals[i], row,j)], nonTerminals[i], row, 3);
        }
        removeDuplicate(firsts[i]);
        cout<<nonTerminals[i]<<" : "<<firsts[i]<<endl;
    }
    cout<<endl;

    /*Compute occurrence of non-terminal*/
    for(i = 0; i < row; i++)
        for(j = 3; grammar[i][j]; j++)
            for(k = 0; k < nonTerminals.length(); k++)
                if(grammar[i][j] == nonTerminals[k])
                    countRHSOccurence[k]++;

    /*Compute follow of every non-terminal*/
    for(i = 0; i < nonTerminals.length(); i++){
        follows[i] += followOfNonTerminal(nonTerminals[i], row, follows);
        removeDuplicate(follows[i]);
        removeEpsilon(follows[i]);
        //cout<<nonTerminals[i]<<" : "<<follows[i]<<endl;
        cout<<"Follow "<<nonTerminals[i]<<" : "<<follows[i]<<endl;
    }
    return 0;
}
