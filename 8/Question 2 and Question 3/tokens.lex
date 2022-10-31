%{
#define YYSTYPE double
extern int yyerror(char *s);
#include "y.tab.h"
%}

digit [0-9]
digits {digit}+
number {digits}\.?|{digit}*\.{digits}

%%

{number}  { yylval=atof(yytext);return NUMBER; }
"("       return START_PAR;
")"       return END_PAR;
"+"       return ADD;
"-"       return SUBTRACT;
"*"       return MULTIPLY;
"/"       return DIVIDE;
"%"	  return MOD;
"^"	  return POW;
sqrt 	  return SQRT;
";"       return SEMICOLON;
[ \t]   ;
.      { yyerror("Invalid operator");exit(0);}

%%

int yywrap() {
  return 1;
}
