#include <assert.h>
#include <string.h>
#include "../lib/include/parser.h"
#include "../lib/include/match.h"

void run_test(const char* description, char* regex, char *match_string) {
    printf("\n------------------------------\n");
    printf("TEST: %s\n", description);
    printf("Regex: %s\n", regex);
    printf("------------------------------\n");

    Graph *parser = parse_regex((char *)regex);
    if (!parser) {
        printf("parse_regex returned NULL\n");
    } 
    else {
        printf("parse_regex completed successfully.\n");
    }

    int is_matched = match(parser, match_string);
    if (is_matched) {
        printf("The regex matches: %s\n", match_string);
    }
    else {
        printf("The regex does not match: %s\n", match_string);
    }
    
    deconstruct(parser); 
    printf("Deconstruction complete.\n");
}

int main() {
    printf("----- REGEX PARSER TESTS -----\n");

    // Test 1: Simple literal sequence
    run_test("Simple literal test - Correct", "hello", "hello");
    run_test("Simple literal test - Incorrect", "hello", "bello");

    // Test 2: Character class / bracket range
    run_test("Bracket test - Correct", "[a-z]", "a");
    run_test("Bracket test - Correct", "[a-z]", "g");

    // Test 3: Grouping test
    run_test("Group test - Correct", "(abc)", "abc");
    run_test("Group test - Incorrect", "(abc)", "cba");

    // Test 4: Alternation test
    run_test("OR operator test - Correct", "a|b", "a");
    run_test("OR operator test - Correct", "a|b", "b");
    run_test("OR operator test - Incorrect", "a|b", "c");

    // Test 5: Quantifier and repeat test
    run_test("Repeat specified test - Correct", "a{2,4}", "aa");
    run_test("Repeat specified test - Correct", "a{2,4}", "aaa");
    run_test("Repeat specified test - Incorrect", "a{2,4}", "a");

    
    // Complex test cases
    run_test("Complex Email test - Correct", "[a-zA-Z0-9_]+@[a-zA-Z0-9]+[a-zA-Z]{2,}", "valid_@example.com");
    run_test("Complex Email test - Incorrect", "[a-zA-Z0-9_]+@[a-zA-Z0-9]+[a-zA-Z]{2,}", "invalidemail.@@gmail.com");
    run_test("Complex Email test - Correct", "[a-zA-Z0-9_]+@[a-zA-Z0-9]+[a-zA-Z]{2,}", "Edwei395@gmail.com");

    printf("\nAll regex parser tests complete.\n");
    return 0;
}