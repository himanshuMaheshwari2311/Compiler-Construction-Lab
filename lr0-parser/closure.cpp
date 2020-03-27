


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

