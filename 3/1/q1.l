%{
#include<stdio.h>
%}

%%

(0*(10*1)*0*)* {printf("Accepted\n %s", yytext);}
.* {printf("Not Accepted\n");}

%%

int yywrap(void) {return 1;}

int main() {
	yyin=fopen("input1.txt","r");
	yylex();
}
