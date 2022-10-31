%{
#include<stdio.h>
%}

%%

(0|1)*0 {printf("Accepted\n");}
.* {printf("Not Accepted\n");}

%%

int yywrap(void) {return 1;}

int main() {
	yyin=fopen("input3.txt","r");
	yylex();
}
