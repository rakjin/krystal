CC     = g++
CFLAGS = -Wall -static-libgcc -static-libstdc++ -I ./lib/
BIN    = krystal

$(BIN) : parser.o scanner.o
	$(CC) $(CFLAGS) Main.cpp KstFile.cpp Context.cpp scanner.o parser.o -o $(BIN)

parser.o : parser.yy
	bison parser.yy
	$(CC) $(CFLAGS) -c -o parser.o parser.tab.cc

scanner.o : parser.yy scanner.l Scanner.h
	flex scanner.l
	$(CC) $(CFLAGS) -c -o scanner.o lex.yy.cc


.PHONY : all clean test

clean :
# remove bison generated files
	rm -rf parser.tab.cc parser.tab.hh location.hh position.hh stack.hh
# remove flex generated files
	rm -rf lex.yy.cc
# remove compiled files
	rm -rf $(BIN) $(BIN).exe *.o

test : $(BIN) test.kst
	./$(BIN) test.kst

