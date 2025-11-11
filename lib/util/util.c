#include "include/util.h"

// Node constructor
Node* node(Node *out, Node* out1) {
    Node *new_node = malloc(sizeof(Node)); 
    new_node->character = calloc(MAX_LENGTH_CHARACTER_ARRAY, sizeof(uint8_t));
    new_node->left = out;
    new_node->right = out1;
    new_node->length = MAX_LENGTH_CHARACTER_ARRAY;
    
    return new_node;
}

Node* node_single(uint8_t character, Node *out, Node* out1) {
    Node *new_node = malloc(sizeof(Node));
    new_node->character = malloc(sizeof(uint8_t));
    *new_node->character = character;
    new_node->left = out;
    new_node->right = out1;
    new_node->length = 1;
    return new_node;
}

Node* node_copy(Node* to_copy) {
    Node* from_copy = malloc(sizeof(Node));
    if (to_copy->length == 1) {
        from_copy->length = 1;
        from_copy->character = malloc(sizeof(uint8_t));
        *from_copy->character = *to_copy->character;
    }
    else {
        from_copy->character = calloc(MAX_LENGTH_CHARACTER_ARRAY, sizeof(uint8_t));
        from_copy->length = MAX_LENGTH_CHARACTER_ARRAY;
        for (int i = 0; i < from_copy->length; ++i) {
            from_copy->character[i] = to_copy->character[i];
        }
    }

    return from_copy;
}

Graph *graph() {
    Graph *graph = malloc(sizeof(Graph));
    graph->nodes = NULL;
    graph->length_of_nodes = 0;

    return graph;
}

Ptrlist* ptrlist() {
    Ptrlist *ptrlist = malloc(sizeof(Ptrlist));
    ptrlist->next = NULL;
    ptrlist->node = NULL;

    return ptrlist;
}
void ptrlist_deconstructor(Ptrlist **self) {
    if (self == NULL || *self == NULL) {
        return;
    }
    Ptrlist *curr_list = *self;
    while (curr_list != NULL) {
        Ptrlist *to_free = curr_list;
        curr_list = curr_list->next;

        free(to_free);
    }
    *self = NULL;
}
void append(Ptrlist **self, Node *node) {
    if (!node) {
        return;
    }
    Ptrlist *new_ptrlist = ptrlist();
    new_ptrlist->node = node;
    if (self == NULL || *self == NULL) {
        *self = new_ptrlist;
        return;
    }
    Ptrlist *curr_ptrlist = *self;
    while (curr_ptrlist->next != NULL) {
        if (curr_ptrlist->node == node) {
            free(new_ptrlist);
            return;
        }
        curr_ptrlist = curr_ptrlist->next;
    }

    if (curr_ptrlist->node == node) {
        free(new_ptrlist);
        return;
    }
    curr_ptrlist->next = new_ptrlist;
}

Node* dequeue(Ptrlist **self) {
    if (self == NULL || *self == NULL) {
        return NULL;
    }
    Ptrlist *next_list = *self;
    Node *node = next_list->node;
    *self = next_list->next;

    next_list->next = NULL;
    next_list->node = NULL;
    free(next_list);
    return node;
}


Stack* stack() {
    Stack *stack = malloc(sizeof(Stack));
    stack->length = 0;
    return stack;
}

void push(Stack *self, uint8_t character) {
    Node* new_node = node_single(character, NULL, NULL);
    if (self->next == NULL) {
        self->next = new_node;
        self->length = 1;
        return;
    }
    Node *curr_node = self->next;
    while (curr_node->left != NULL) {
        curr_node = curr_node->left;
    }
    curr_node->left = new_node;
    ++self->length;
}
uint8_t pop(Stack *self) {
    if (self->length == 0) 
        return 0;
    Node *node_holder = self->next;
    Node *next_node = node_holder->left;
    uint8_t character = *node_holder->character;

    node_holder->left = NULL;
    free(node_holder);
    self->next = next_node;
    --self->length;
    return character;
}


void free_nodes(Graph *self) {
    if (self == NULL || self->nodes == NULL) {
        return;
    }

    Ptrlist *list = NULL;
    Node *curr_node = self->nodes;
    append(&list, curr_node);
    while (curr_node != NULL) {
        Node *left_node = curr_node->left;
        Node *right_node = curr_node->right;

        if (left_node != NULL) {
            append(&list, left_node);
        }
        while (right_node != NULL && right_node != curr_node) {
            append(&list, right_node);
            right_node = right_node->left;
        }
        curr_node = left_node;
    }

    while (list != NULL) {
        Node *to_free = dequeue(&list);
        if (to_free == NULL) {
            break;
        }
        to_free->left = NULL;
        to_free->right = NULL;
        free(to_free->character);
        to_free->character = NULL;
        free(to_free);
    }

    ptrlist_deconstructor(&list);
    self->nodes = NULL;
    return;
}
