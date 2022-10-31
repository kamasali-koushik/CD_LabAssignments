%{ 
#include<stdio.h>
#include<string.h>
void helper(char *str)
{
	if(strcmp(str,"if")==0)
	{
		printf("%s -->If Keyword\n",yytext);
	}
	else if(strcmp(str,"else")==0)
	{
		printf("%s -->Else Keyword\n",yytext);
	}
	else if(strcmp(str,"then")==0)
	{
		printf("%s -->Then\n",yytext);
	}
	else
	{
		printf("%s -->id\n",yytext);
	}
}
%}
%% 
[0-9]	{printf("%s --> digit\n",yytext);}
[0-9]+	{printf("%s --> digits\n",yytext);}
[0-9]+([.][0-9]+)?([E][+-]?[0-9]+)?	{printf("%s --> number\n",yytext);}
[a-zA-z]	{printf("%s --> letter\n",yytext);}
[a-zA-z]([a-zA-z]|[0-9])*	{helper(yytext);}
([<]|[>]|[<][=]|[>][=]|[<][>]|[=]|[=][=])	{printf("%s -->relop\n",yytext);}
["\n"" "] {}
%% 
int yywrap(){}  
int main() 
{ 
	yyin=fopen("input.txt","r");
	yylex(); 
	return 0; 
} 
