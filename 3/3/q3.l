%{
#include<stdio.h>
#include<string.h>

void helper(char *str)
{
	if(strcmp(str,"main")==0)
	{
		printf("(%s,Function)\n",yytext);
	}
	if(strcmp(str,"if")==0)
	{
		printf("(%s,)\n",yytext);
	}
	else if(strcmp(str,"else")==0)
	{
		printf("(%s,)\n",yytext);
	}
	else if(strcmp(str,"then")==0)
	{
		printf("(%s,)\n",yytext);
	}
	else if(strcmp(str,"for")==0)
	{
		printf("(%s,loop)\n",yytext);
	}
	else if(strcmp(str,"while")==0)
	{
		printf("(%s,loop)\n",yytext);
	}
	else if(strcmp(str,"print")==0)
	{
		printf("(%s,output construct)\n",yytext);
	}
	else if(strcmp(str,"INT")==0 || strcmp(str,"int")==0)
	{
		printf("(%s,data type)\n",yytext);
	}
	else if(strcmp(str,"FLOAT")==0 || strcmp(str,"float")==0)
	{
		printf("(%s,data type)\n",yytext);
	}
	else if(strcmp(str,"read")==0)
	{
		printf("(%s,input construct)\n",yytext);
	}
	else
	{
		printf("(%s,id)\n",yytext);
	}
}
	
%}

%%
[0-9]	{printf("(%s,digit)\n",yytext);}
[0-9]+	{printf("(%s,digits)\n",yytext);}
[0-9]+([.][0-9]+)?([E][+-]?[0-9]+)?	{printf("(%s,number)\n",yytext);}
[a-zA-z]	{printf("(%s,letter)\n",yytext);}
[a-zA-z]([a-zA-z]|[0-9])*	{helper(yytext);}
[=]	{printf("(%s,Assignment Operator)\n",yytext);}
([+]|[-]|[*]|[/])	{printf("(%s,Arithmetic Operator)\n",yytext);}
([<]|[>]|[<][=]|[=][=]|[>][=])	{printf("(%s,Relational Operator)\n",yytext);}
[" ""("")""()""{}""\n""{""};]	{}
.	{}
%%


int yywrap(void) {return 1;}

int main()
{
	yyin = fopen("input.txt", "r"); 
	yylex();
}