#include "include/match.h"

int check_character(Node *node, uint8_t character) {
    uint8_t index = character % node->length;
    if (index == 0) {
        if (*node->character == character) {
            return 1;
        }
        return 0;
    }

    for (uint8_t counter = 0; counter < node->length; ++counter) {
        if (node->character[index] == character) {
            return 1;
        }
        index = (index + 1) % node->length;
    }
    return 0;
}



int match(Graph *graph, char *input_string) {
    Ptrlist *node_list = NULL;
    append(&node_list, graph->nodes);
    uint8_t list_length = 1;
    uint8_t pos = 0;


    while (input_string[pos] != '\0') {
        // uint8_t match_char = input_string[pos];
        char match_char = input_string[pos];

        // for(int curr_list_pos = 0; curr_list_pos < list_length; ++curr_list_pos) {
        uint8_t curr_list_pos = 0;
        while (curr_list_pos < list_length) {
            Node *curr_node = dequeue(&node_list);
            Node *prev_node = curr_node;
            --list_length;
            
            if (*curr_node->character == EPSILON) {
                if (curr_node->right != NULL) {
                    append(&node_list, curr_node->right);
                    ++list_length;
                }
                curr_node = curr_node->left;
            }

            if(curr_node->right != NULL) {
                append(&node_list, curr_node->right);
                ++list_length;
            }

            if (*curr_node->character == GOAL) {
                ptrlist_deconstructor(&node_list);
                free(node_list);
                return 1;
            }
            else if (*curr_node->character == EPSILON) {
                append(&node_list, curr_node);
                ++list_length;
                continue;
            }

            if (check_character(curr_node, match_char)) {
                Node *left_node = curr_node->left;
                // if (*curr_node->left->character == GOAL) {
                if (*left_node->character == GOAL || *left_node->left->character == GOAL) {
                    ptrlist_deconstructor(&node_list);
                    free(node_list);
                    return 1;
                }
                else if (*left_node->left->character == EPSILON) {
                    if (*left_node->left->left->character == GOAL) {
                        ptrlist_deconstructor(&node_list);
                        free(node_list);
                        return 1;
                    }
                }
                append(&node_list, left_node);
                ++list_length;
            }
            else {
                append(&node_list, prev_node);
                ++list_length;
            }
            ++curr_list_pos;
        }
        ++pos;
    }

    return 0;
}