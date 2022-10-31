%{
#include <stdio.h>
#include <math.h>
#include "lex.yy.c"
int yyerror(char *s);
%}

%token NUMBER SEMICOLON ADD SUBTRACT MULTIPLY DIVIDE MOD START_PAR END_PAR SQRT POW

%left ADD SUBTRACT
%left MULTIPLY DIVIDE MOD
%right POW
%left START_PAR END_PAR

%start S

%%

S: S Input 
   | ;

Input:  E { printf("\nValid Expression\n");
	     printf("Result of Expression: %lf\n", $1); }
	| SEMICOLON {exit(0);} ;

E: NUMBER           { $$ = $1; }
      | START_PAR E END_PAR { $$ = $2; }	
      | E ADD E { $$ = $1 + $3; }
      | E SUBTRACT E { $$ = $1 - $3; }
      | E MULTIPLY E { $$ = $1 * $3; }
      | E DIVIDE E { $$ = $1 / $3; }
      | E MOD E {$$ = (int)$1 % (int)$3;} 
      | SQRT START_PAR E END_PAR {$$ = sqrt($3); }
      | E POW E {$$ = pow($1,$3); } ;

%%
extern int yylex();

int yyerror(char *s) {
  fprintf(stderr, "%s\n", s);
  return 0;
}

int main() {
  printf("\nEnter an arithmetic operation ending with semicolon:\n");
  yyparse();
  return 0;
}
