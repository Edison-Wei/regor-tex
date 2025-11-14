# Regex engine

An experimental Regular Expression (regex) engine built from scratch, using the following process:
- Parse input string and store each valid regex type inside a parser struct
- Build a non-Deterministic Finite automaton, that has each character acts as a node
- Construct state transitions based on node type (or epsilon for trivial transitions)

## Usage
#### Build with Dockerfile
Use the `Dockerfile` to build and compile all required files:
To build, make sure you have `docker` along with `alpine:latest` on your system.

Then:
```cmd
    docker build -t regex-project . 
```

Then to execute either programs:
```cmd
    docker run -it --rm regex-project #To run the main regex engine
    docker run --entrypoint /app/regex_test --rm regex-project #To run the test program
```

#### Build with Makefile
Use the `makefile` to easily compile all files required into two executables:
```cmd
    make all # Creates two files 'regex' and 'regex_test'
```
Then use the following command to run the files:
```cmd
	'./regex' to run regex engine with user input
    './regex_test' to run test cases on the regex engine
```

## Video example
<video src="https://github.com/user-attachments/assets/5b488d33-358b-4566-99c0-5cfc781cf16f" width="100%" controls></video>

### Requirements
- gcc

The `makefile` will be compiled with `gcc`.
