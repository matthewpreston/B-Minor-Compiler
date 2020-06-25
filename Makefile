CC = gcc
CFLAGS = -Wall -g
LDFLAGS=`pkg-config libgvc --libs`
OBJECTS = hash_table.o stack.o ast_type.o symbol.o scope.o ast.o visualize_tree.o parser.o scanner.o
TARGET = bminor
SOURCE_FILES = Makefile test_cases.sh type.h scanner.flex parser.bison ast_type.h ast_type.c ast.h ast.c visualize_tree.h visualize_tree.c hash_table.h hash_table.c stack.h stack.c symbol.h symbol.c scope.h scope.c

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

# Simple objects
hash_table.o: hash_table.h
stack.o: stack.h

# Dependencies for parser
ast_type.o: ast_type.h scope.h stack.h symbol.h
symbol.o: symbol.h ast_type.h
scope.o: scope.h hash_table.h stack.h symbol.h
ast.o: ast.h ast_type.h scope.h stack.h symbol.h

# A debugging tool for visualizing the abstract syntax tree
visualize_tree.o: visualize_tree.c visualize_tree.h ast.h
	$(CC) $(CFLAGS) `pkg-config libgvc --cflags` -c -o $@ visualize_tree.c

# The actual parser and scanner
parser.o: ast.h token.h
parser.c: parser.bison ast.h
	bison --defines=token.h --output=$@ $<
scanner.o: token.h
scanner.c: scanner.flex token.h
	flex -o $@ $<
	
.PHONY: clean test wc
clean:
	$(RM) $(TARGET) $(OBJECTS) parser.c token.h scanner.c Tests/*.svg
	
# For testing the compiler against a series of test cases
test: $(TARGET)
	bash test_cases.sh
	
# For seeing how many lines of hand-written code I've done
wc:
	@echo "Total line counts (with blank lines):\nLines Words Chars"; \
	wc $(SOURCE_FILES);
	@echo;
	@echo "Total line counts (without blank lines):\n  Lines   Words   Chars"; \
	sed "/^\s*$$/d" $(SOURCE_FILES) | wc
