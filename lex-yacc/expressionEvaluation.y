%{
	#include<stdio.h>	
	int flag = 0;
	int yylex();
	void yyerror();
%}
%token NUM
%left '+''-'
%left '*''/''%'
%left '('')'

%%
stmt: E{
	printf("Evaluated expression = %d", $$);
	return 0;
}
E:E'+'E 	{$$ = $1 + $3;}
| E'-'E 	{$$ = $1 - $3;}
| E'*'E 	{$$ = $1 * $3;}
| E'/'E 	{$$ = $1 / $3;}
| E'%'E 	{$$ = $1 % $3;}
| '('E')' 	{$$ = $2;}
| NUM		{$$ = $1;}
;
%%

void main(){
	printf("Enter an arithmetic expression\n");
	yyparse();
	if(flag == 0)
		printf("\nEntered Arithmetic expression is Valid\n");
}

void yyerror(){
	printf("\nEntered arithmetic expression is Invalid\n");
	flag = 1;
}
