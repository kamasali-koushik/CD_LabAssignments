%{
#define ERROR -1
#define BLACK "\033[0;30m"
#define RED  "\033[0;31m"
#define GRE "\033[0;32m"
#define YEL  "\033[0;33m"
#define BLUE  "\033[0;34m"
#define WHITE  "\033[0;37m"
#define RESET  "\033[0m"
#define CYAN "\033[0;36m"
%}

whitespace [ \t\n]
digit [0-9]
digits {digit}+
letter [a-zA-Z]
l_or_d ({letter}|{digit})
identifier ({letter}{l_or_d}*)
operator ([+*])
separator [()]

%%
{digits}  {printf("id\n");}
{identifier} {printf("id\n");}
{operator} {printf("%s\n",yytext);}
{separator} {printf("%s\n",yytext);}
{whitespace} {}
. {return ERROR;}
%%

int yywrap(){}
int main(){
	yylex();
	return 0;
}
