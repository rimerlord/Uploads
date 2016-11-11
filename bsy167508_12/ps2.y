%{
	#include <stdio.h>
	int tcred=0;
	int yylex(void);
	void yyerror(char *);
%}
%union {
    int intValue;
    float flo;
    char *str;
}
%start input
%token TOKDROP TOKAUDIT TOKGRAD
%type<flo> line ecred TOKDROP TOKAUDIT
%type<str> TOKGRAD
%%
input:		/* empty string */
		| line line
		;
line:		'\n'					{printf("\n"); $$=0.00;}
		| ecred ecred ecred ecred		{$$=($1+$2+$3+$4)/tcred; printf(" %.2f\n", $$);}
		;
		
ecred:		TOKDROP					{$$=0.00; } 
		| TOKAUDIT				{$$=0.00; }
		| TOKGRAD				{$$=($1[7]-'0')*($1[9]-'0'); tcred+=$1[7]-'0';}
		;
%%
void yyerror(char *s) {
fprintf(stderr, "%s\n", s);
}
int main() {
yyparse();
return 0;
}
