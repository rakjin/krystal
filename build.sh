#!/bin/sh

flex krystal.l
bison -d krystal.y
gcc lex.yy.c krystal.tab.c -o krystal
rm lex.yy.c krystal.tab.c krystal.tab.h

