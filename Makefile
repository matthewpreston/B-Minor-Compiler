CC = gcc
FLAGS = -Wall
OBJECTS = decl.o parser.o scanner.o
TARGET = bminor
SOURCE_FILES = Makefile test_cases.sh type.h scanner.flex parser.bison decl.h decl.c

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(FLAGS)

decl.o: decl.h

parser.o:
parser.c: parser.bison decl.h
	bison -v --defines=token.h --output=$@ $<

scanner.o:
scanner.c: scanner.flex token.h
	flex -o $@ $^
	
.PHONY: clean test wc
clean:
	-rm -f $(TARGET) $(OBJECTS) parser.c token.h scanner.c
test:
	bash test_cases.sh
wc:
	@echo "Total line counts (with blank lines):\nLines Words Chars"; \
	wc $(SOURCE_FILES);
	@echo;
	@echo "Total line counts (without blank lines):\n  Lines   Words   Chars"; \
	sed "/^\s*$$/d" $(SOURCE_FILES) | wc
