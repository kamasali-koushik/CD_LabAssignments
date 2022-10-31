lex tokens.lex
yacc -d gram.y
gcc y.tab.c -lm
./a.out
rm a.out lex.yy.c y.tab.c y.tab.h