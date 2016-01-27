#!/bin/bash
if [ "$#" -eq 1 ]; then
	S=$1
else
	echo "Enter the Source file"
	read S
fi

cd src
bison --yacc --defines miniLang.y
flex miniLang.l
cd ..
gcc -std=c99 ./src/lex.yy.c ./src/y.tab.c ./src/y.tab.h ./src/main.c -lfl -o run
clear
cat $S | ./run

rm run
cd src

rm lex.yy.c
rm y.tab.c
rm y.tab.h
