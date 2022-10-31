%{
extern int yyerror(char *s);
%}

digit [0-9]
digits {digit}+
number {digits}\.?|{digit}*\.{digits}

%%

{number}  { yylval.val=atof(yytext);return NUMBER; }
"+"       return ADD;
"-"       return SUBTRACT;
"*"       return MULTIPLY;
"/"       return DIVIDE;
"="       return EQUALS;
[ \t]   ;
.      { yyerror("Invalid operator");exit(0);}

%%

int yywrap() {
  return 1;
}
