#include<iostream>
#include<conio.h>
#include<string.h>
#include<stack>

using namespace std;

/*E' = G  T' = S epsilon = # Blank = _*/

string frontAppend(string str, string prod){
    string temp;
    temp = str;
    str="";
    str += prod + temp;
    return str;
}

void displayString(string str){
    int i;
    for(i = 0; i < str.length(); i++)
        cout<<str[i];
    cout<<'\t'<<'\t'<<'\t';
}

string frontRemove(string str){
    string temp = str;
    int i;
    str = "";
    for(i = 1; i < temp.length(); i++)
        str += temp[i];
    return str;
}

string production(string parsingTable[][7], string x, string y){
    int xCo, yCo,i ,j;
    for(i = 0; i < 6; i++)
        if(parsingTable[i][0] == x){
            xCo = i;
            break;
        }
    for(j = 0; i < 7; j++)
        if(parsingTable[0][j] == y){
            yCo = j;
            break;
        }
    return parsingTable[xCo][yCo];
}

char terminal[7] = {'i','+','*','(',')','$'};

bool isValidChar(char c){
    int i;
    for(i = 0; i < 7; i++)
        if(c == terminal[i])
            return true;
    return false;
}

void isAccepted(string parsingTable[][7], string str) {
    string prod, slicedProd, rePush, temp;
    int strItr = 0, i, j = 0;
    string S;
    S = frontAppend(S,"$");
    S = frontAppend(S,"E");
    cout<<"Stack Content\t\tAction"<<endl;
    while(!((S[0] == '$' && str[strItr] == '$')||(S[0] == ')' && str[strItr] == ')'))) {
        string x = string(1,S[0]);
        string y = string(1,str[strItr]);
        prod = "";
        slicedProd = "";
        if(isValidChar(str[strItr]))
            prod = production(parsingTable, string(x), string(y));
        else break;
        if(prod == "_")
            break;
        displayString(S);
        displayString(prod);
        cout<<endl;
        for(i = 0; i < prod.length(); i++){
            if(i > 2)
                slicedProd += prod[i];
        }
        S = frontRemove(S);
        S = frontAppend(S, slicedProd);
        if(S[0] == str[strItr]){
            S = frontRemove(S);
            strItr++;
        }
        if(S[0] == '#')
            S = frontRemove(S);
    }
    if((S[0] == '$' && str[strItr] == '$')||(S[0] == ')' && str[strItr] == ')'))
            cout<<endl<<"String Accepted";
    else cout<<endl<<"String is not Accepted";
    return;
}

int main() {
    string parsingTable[6][7] = {{"_","i","+","*","(",")","$"},{"E","E->TG","_","_","E->TG","_","_"},
                                 {"G","_","E->+TG","_","_","G->#","G->#"},{"T","T->FS","_","_","T->FS","_","_"},
                                 {"S","_","T->#","T->*FS","_","S->#","S->#"},{"F","F->i","_","_","F->(E)","_","_"}};
    string str;
    cout<<"Enter a string: ";
    cin>>str;
    str += '$';
    isAccepted(parsingTable,str);
    return 0;
}
