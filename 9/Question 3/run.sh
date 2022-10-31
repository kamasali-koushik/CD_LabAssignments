lex tokens.lex
yacc -d gram.y
g++ y.tab.c -lm
./a.out

rm a.out y.tab.c y.tab.h edges.dot lex.yy.c 