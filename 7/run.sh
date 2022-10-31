
lex Lab7.l
yacc Lab7.y -d
gcc y.tab.c
./a.out
rm a.out lex.yy.c y.tab.c y.tab.h
