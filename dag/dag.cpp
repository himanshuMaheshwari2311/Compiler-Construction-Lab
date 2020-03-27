#include<iostream>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<fstream>
#include<algorithm>

using namespace std;

/**
Function Prototype
*/
struct treenode* getNode(char, int);
string fileInput();
string getExpression(string);
void printTree(struct treenode*);
short isPresent(char, string);
void removeDuplicate(string &);
struct treenode* searchNode(struct treenode*, char);
void rootInit();
string returnExpression(char);
int allVisited();
void insertTree(struct treenode*);
void makeTree(string);


/**
Global Variables
*/
struct treenode{
    string expression;
    char name;
    char op;
    int num;
    treenode *right;
    treenode *left;
};
string varVisited = "", isRHS = "", exp[7];
struct treenode *root = NULL;
struct treenode * mainNode = NULL;
int row, nodeNum = 0, flag = 0;

/**
Create a new Node
*/
struct treenode* getNode(char name, int num){
    struct treenode *newNode = new(treenode);
    newNode->num = num;
    newNode->name = name;
    newNode->expression = "";
    newNode->op = ' ';
    newNode->right = NULL;
    newNode->left = NULL;
    return newNode;
}

/**
Read 3 address code from file
*/
string fileInput(){
    int array_size = 1024;
	char * content = new char[array_size];
	string fileContent, temp;
	int position = 0, i = 0;
	ifstream fin("input-1.txt");
    if(fin.is_open()) {
    cout << "File Opened.. Reading Content..." << endl;
    	while(!fin.eof() && position < array_size){
			fin.get(content[position]);
			position++;
		}
		content[position-1] = '\0';
		for(int i = 0; content[i] != '\0'; i++)
			fileContent += content[i];
        fin.close();
	}
	else {
		cout << "File could not be opened." << endl;
	}
	row = 0;
	while(fileContent[i] != '\0'){
        if(fileContent[i] == '\n')
            row++;
        i++;
	}
	row++;
	return fileContent;
}

/**
get single expression
*/
string getExpression(string fileContent, int row){
    int i;
    if(row == 0)
        i = 0;
    else i = row * 5 + row;
    string exp;
    while(i < row * 5 + 5 + row){
        exp += fileContent[i];
        i++;
    }
    return exp;
}

/**
Print tree
*/
void printTree(struct treenode *root){
    if(root){
        printTree(root->left);
        if(root->left && root->right)
            cout<<root->num<<"\t"<<root->op<<"\t"<<root->name<<"\t"<<root->expression<<"\t"<<root->left->name<<":"<<root->left->num<<"\t"<<root->right->name<<":"<<root->right->num<<endl;
        else
            cout<<root->num<<"\t"<<root->op<<"\t"<<root->name<<"\t"<<root->expression<<endl;
        printTree(root->right);
    }
}

/**
To check if variable has already been used
*/
short isPresent(char var, string str){
    int i;
    for(i = 0; str[i]; i++){
        if(str[i] == var)
            return 1;
    }
    return 0;
}

/**
remove duplicate from varVisited
*/
void removeDuplicate(string &str){
    sort(str.begin(), str.end());
    str.erase(unique(str.begin(), str.end()), str.end());
}

/**
search function for makeTree
*/
struct treenode* searchNode(struct treenode *root, char name){
    if(root != NULL){
        if(root->name == name){
            return root;
        }
        else{
            struct treenode *temp = searchNode(root->left, name);
            if(temp == NULL)
                temp = searchNode(root->right, name);
            return temp;
        }
    }
    else return NULL;
}

/**
Initialize the root
*/
void rootInit(int row) {
    if(root == NULL){
        root = getNode(exp[row - 1][0], nodeNum++);
        root->expression = exp[row - 1];
        root->op = exp[row - 1][3];
        varVisited += root->name;
    }
}

/**
Return expression for variable
*/
string returnExpression(char name){
    int i;
    for(i = 0; i < row; i++)
        if(exp[i][0] == name)
            return exp[i];
}

/**
Insert Node function for makeTree
*/
void insertTree(struct treenode* root){
    if(root == NULL)
        return;
    root->left = getNode(root->expression[2], nodeNum++);
    if(isPresent(root->left->name, isRHS)){
        root->left->expression = returnExpression(root->left->name);
        root->left->op = root->left->expression[3];
        varVisited += root->left->name;
        removeDuplicate(varVisited);
        insertTree(root->left);
    }
    else {
        if(isPresent(root->left->name, varVisited)){
            struct treenode *temp = searchNode(mainNode, root->left->name);
            nodeNum--;
            root->left = NULL;
            root->left = temp;
        }
        varVisited += root->left->name;
        removeDuplicate(varVisited);
    }
    root->right = getNode(root->expression[4], nodeNum++);
    if(isPresent(root->right->name, isRHS)){
        root->right->expression = returnExpression(root->right->name);
        root->right->op = root->right->expression[3];
        varVisited += root->right->name;
        removeDuplicate(varVisited);
        insertTree(root->right);
    }
    else {
        if(isPresent(root->right->name, varVisited)){
            struct treenode *temp;
            temp = searchNode(mainNode, root->right->name);
            nodeNum--;
            root->right = NULL;
            root->right = temp;
            return;
        }
        varVisited += root->right->name;
        removeDuplicate(varVisited);
    }
}

/**
Are all nodes inserted
*/
int allVisited(){
    int i, j;
    int len = isRHS.length();
    int countRHS = 0;
    for(i = 0; isRHS[i]; i++)
        for(j = 0; varVisited[j]; j++)
            if(isRHS[i] == varVisited[j])
                countRHS++;
    if(countRHS == len)
        return 1;
    return 0;
}

/**
Generate Dag
*/
void makeTree(string fileContent){
    int i, nodeNum = 0, j;
    for(i = 0; i < row; i++){
        exp[i] = getExpression(fileContent, i);
        isRHS += exp[i][0];
    }
    rootInit(row);
    mainNode = root;
    insertTree(root);
    cout<<endl<<"Inserted Variables: "<<varVisited<<endl;
    cout<<endl<<"num"<<"\t"<<"op"<<"\t"<<"name"<<"\t"<<"exp"<<"\t"<<"left"<<"\t"<<"right"<<endl;
    printTree(root);
}

/**
main function
*/
int main(){
    string fileContent;
    fileContent = fileInput();
    cout<<fileContent<<endl;
    makeTree(fileContent);
    return 0;
}
