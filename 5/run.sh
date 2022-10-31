clear;
flex 2grammar.l
g++ lex.yy.c 
./a.out < strings2.txt > out.txt
g++ main.cpp -o b.out
./b.out input4.txt out.txt
tmux copy-mode