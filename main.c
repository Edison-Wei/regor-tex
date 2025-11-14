#include <string.h>
#include "lib/include/parser.h"
#include "lib/include/match.h"

#define MAX_INPUT_LENGTH 256

void check_graph(Graph *regex);

int main() {
    char regex[MAX_INPUT_LENGTH];
    char string[MAX_INPUT_LENGTH];
    
    printf("Enter a Regex formula: ");
    fgets(regex, MAX_INPUT_LENGTH, stdin);
    regex[strlen(regex)-1] = '\0';

    Graph *parsed_regex = NULL;
    parsed_regex = parse_regex(regex);

    do {
        printf("\nEnter a String to match.\nCurrent regex - %s: ", regex);
        fgets(string, MAX_INPUT_LENGTH, stdin);
        string[strlen(string)-1] = '\0';        

        if (strcmp(string, "change") == 0) {
            printf("Enter a new Regex: ");
            fgets(regex, MAX_INPUT_LENGTH, stdin);
            regex[strlen(regex)-1] = '\0';

            deconstruct(parsed_regex);
            free(parsed_regex);

            // Parse with new regex
            parsed_regex = parse_regex(regex);
            continue;
        }
        else if (strcmp(string, "end") == 0) {
            break;
        }
        else {
            int matched = match(parsed_regex, string);

            if (matched == 1)
                printf("The string (%s) matches\n", string);
            else
                printf("The string (%s) does not match\n", string);
        }

        
    } while(strcmp(string, "end") != 0);

    deconstruct(parsed_regex);
    free(parsed_regex);

    return 0;
}

void print_special(uint8_t c) {
    switch (c) {
    case GOAL:
        printf("Goal node!!!!");
        break;
    case EPSILON:
        printf("Epsilon node----");
        break;
    default:
        printf("%c ", c);
        break;
    }
}

void check_graph(Graph *regex) {
    Node *curr_node = regex->nodes;
    int counter = 0;

    printf("Checking Graph------------\n");

    while (curr_node != NULL) {
        printf("Printing curr_node character: ");
        for (int i = 0; i < curr_node->length; ++i) {
            print_special(curr_node->character[i]);
            // printf("%c ", curr_node->character[i]);
        }
        printf("\n");
        Node *left_node = curr_node->left;
        Node *right_node = curr_node->right;
        while (right_node != NULL && right_node != curr_node) {
            printf("--Printing right_node character: ");
            for (int i = 0; i < right_node->length; ++i) {
                print_special(right_node->character[i]);
                // printf("%c ", right_node->character[i]);
            }
            printf("\n");
            right_node = right_node->left; 
            ++counter;
        }
        curr_node = left_node;
        ++counter;
        
    }
    printf("Number of counted nodes: %d", counter);
}