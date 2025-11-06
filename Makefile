C = clang

SRC = main.c lexer.c ast.c parser.c
OUT = basc

all: $(OUT)

$(OUT): $(SRC)
	$(C) $(SRC) -o $(OUT)

clean: 
	rm -f $(OUT)