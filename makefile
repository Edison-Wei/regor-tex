main := main.c
match := lib/match.c
parser := lib/parser.c
util := lib/util/util.c
test := test/test.c

all: regex_engine regex_test
	@echo "Compiling regex_engine and tests"
	@echo "	'./regex' to run regex engine with user input"
	@echo "	'./regex_test' to run test cases on the regex engine"

regex_engine: 
	gcc -Wall -o regex ${main} ${match} ${parser} ${util}

regex_test:
	gcc -Wall -o regex_test ${test} ${match} ${parser} ${util}

clean:
	rm -v regex