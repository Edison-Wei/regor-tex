#include "include/parser.h"

void deconstruct(Graph *parser) {
    free_nodes(parser);

    return;
}


/// @brief Given a regular expression. Translate the expression into a readable form. (ex. [a-zA-Z]@gmail.com = heLLo@gmail.com)
/// @param parser a Graph pointer to be filled with nodes from the regular expression
/// @param regex an array of char (a string) containing the regular expression to be decoded
Graph* parse_regex(char *regex) {
    Graph *parser = graph();

    char character;
    uint8_t pos = 0;
    Node *node_handler = NULL;

    do {
        character = regex[pos];
        Node *new_nodes = NULL;
        if (character == '(') {
            // Capture group (abc)
            new_nodes = parse_group(regex, &pos);
        }
        else if (character == '[') {
            // Capture group
            new_nodes = parse_bracket(regex, &pos);
        }
        else if (character == '|') {
            // split to new selection
            new_nodes = parse_or(regex, &pos);
            Node *prev_node = parser->nodes;
            Node *hold_node = node_handler;
            while (parser->nodes != hold_node && *hold_node->character != EPSILON) {
                while (prev_node->left != hold_node) {
                    prev_node = prev_node->left;
                }
                hold_node = prev_node;
                prev_node = parser->nodes;
            }

            Node *new_epsilon = node_single(EPSILON, NULL, NULL);
            node_handler->left = new_epsilon;
            hold_node->right = new_nodes;
            prev_node = new_nodes;
            while(prev_node->left != NULL) {
                prev_node = prev_node->left;
            }
            prev_node->left = new_epsilon;

            node_handler = new_epsilon;
            
            ++pos;
            continue;
        }
        else if (character == '*' || character == '?' || character == '+' ) {
            // node_handler will be repeated, therefore not needed to parse anything
            Node* start_node = parse_repeat(regex, &pos, node_handler);
            Node* prev_node = parser->nodes;
            while (prev_node->left != node_handler) {
                prev_node = prev_node->left;
            }
            prev_node->left = start_node;
            node_handler = node_handler->left;

            ++pos;
            continue;
        }
        else if (character == '{') {
            // repeat the previous character that many times
            new_nodes = parse_repeat_specified(regex, &pos, node_handler);
        }
        else {
            // Just add the character
            new_nodes = node_single(character, NULL, NULL);
        }

        if (parser->nodes == NULL) {
            // Then add to parser nodes
            parser->nodes = new_nodes;

            node_handler = new_nodes;
            while(node_handler->left != NULL) {
                node_handler = node_handler->left;
            }
        }
        else {
            if (node_handler->character == EPSILON) {
                node_handler->left = new_nodes->left;
                node_handler->right = new_nodes->right;
                free(new_nodes);
            }
            else {
                node_handler->left = new_nodes;
            }

            while (node_handler->left != NULL) {
                node_handler = node_handler->left;
            }
        }

        ++pos;
    } while (regex[pos] != '\0');

    node_handler->left = node_single(GOAL, NULL, NULL);
    
    return parser;
}

Node* parse_group(char *regex, uint8_t *pos) {
    ++*pos;
    Node *begin_node = node_single(EPSILON, NULL, NULL);
    Node *curr_node = begin_node;
    char character;
    

    while (regex[*pos] != ')') {
        character = regex[*pos];
        Node *new_node = node_single(character, NULL, NULL);

        curr_node->left = new_node;

        curr_node = new_node;
        ++*pos;
    }

    return begin_node;
}

Node* parse_bracket(char *regex, uint8_t *pos) {
    ++*pos;
    Stack *literal_stack = stack();
    char character;
    
    while (regex[*pos] != ']') {
        character = regex[*pos];

        if (character == '-') {
            char next_character = regex[*pos+1];
            push(literal_stack, next_character);
            ++*pos;
        }
        else {
            push(literal_stack, character);
        }
        ++*pos;
    }

    Node *begin_node = node_single(EPSILON, NULL, NULL);
    Node *new_node = node(NULL, NULL);
    begin_node->left = new_node;
    
    while (literal_stack->length != 0) {
        uint8_t begin_char = pop(literal_stack);
        uint8_t end_char = pop(literal_stack);
        if (end_char == 0) {
            uint8_t index = begin_char % MAX_LENGTH_CHARACTER_ARRAY;
            while (new_node->character[index] > 0) {
                ++index;
            }
            new_node->character[index] = begin_char;
        }
        else {
            for (; begin_char <= end_char; ++begin_char) {
                uint8_t index = begin_char % MAX_LENGTH_CHARACTER_ARRAY;
                while (new_node->character[index] > 0) {
                    index = (index + 1) % MAX_LENGTH_CHARACTER_ARRAY;
                }
                new_node->character[index] = begin_char;
            }
        }
    }

    for (int i = 0; i < new_node->length; ++i) {
    }
    free(literal_stack);
    literal_stack = NULL;
    return begin_node;
}

Node* parse_or(char *regex, uint8_t *pos) {
    ++*pos;

    Node *right_side = NULL;
    Node *curr_node = NULL;
    char character;

    while (regex[*pos] != '\0' && regex[*pos] != ')') {
        character = regex[*pos];

        Node *new_node = node_single(character, NULL, NULL);

        if (right_side == NULL) {
            curr_node = new_node;
            right_side = new_node;
        }
        else {
            curr_node->left = new_node;
            curr_node = curr_node->left;

        }
        ++*pos;
    }

    return curr_node;
}

Node* parse_repeat(char *regex, uint8_t *pos, Node* target_node) {
    char repeat_char = regex[*pos];
    Node *begin_node = node_single(EPSILON, NULL, NULL);
    Node *exit_node = node_single(EPSILON, NULL, NULL);

    if (repeat_char == '*') {
        begin_node->left = target_node;
        target_node->left = exit_node;
        begin_node->right = exit_node;
        exit_node->right = target_node;
    }
    else if (repeat_char == '+') {
        begin_node->left = target_node;
        target_node->left = exit_node;
        exit_node->right = target_node;
    }
    else if (repeat_char == '?') {
        begin_node->left = target_node;
        begin_node->right = exit_node;
        target_node->left = exit_node;
    }

    return begin_node;
}

Node* parse_repeat_specified(char *regex, uint8_t *pos, Node *target_node) {
    ++*pos;

    uint8_t min = 0, max = 0;
    uint8_t found_comma = 0;

    while (regex[*pos] != '}') {
        char character = regex[*pos];

        if(character == ',') {
            found_comma = 1;
            ++*pos;
            continue;
        }

        if (!found_comma) {
            min = min * 10 + (character - '0');
        }
        else {
            max = max * 10 + (character - '0');
        }
        
        ++*pos;
    }

    if (!found_comma) {
        max = min;
    }
    if (found_comma && max == 0) {
        max = min;
    }

    Node *begin_node = node_single(EPSILON, NULL, NULL);
    Node *exit_node = node_single(EPSILON, NULL, NULL);
    Node *curr_node = begin_node;
    if (min == 0) {
        free(exit_node);
        return begin_node;
    }

    for (int i = 2; i < min; ++i) {
        Node *new_node = node_copy(target_node);

        curr_node->left = new_node;
        curr_node = new_node;
    }

    if (min == max) {
        exit_node->right = curr_node;
    }

    for (int i = min; i < max; ++i) {
        Node *optional_node = node_copy(target_node);

        curr_node->left = optional_node;
        optional_node->right = exit_node;
        curr_node = curr_node->left;
    }

    if (begin_node->left != NULL) {
        Node *hold_node = begin_node;
        begin_node = begin_node->left;
        free(hold_node->character);
        hold_node->character = NULL;
        free(hold_node);
    }
    
    curr_node->left = exit_node;

    return begin_node;
}

