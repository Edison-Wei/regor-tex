#if !defined(UTILS)
#define UTILS
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_LENGTH_CHARACTER_ARRAY 64
#define EPSILON 0
#define GOAL 1

typedef struct Node Node;
typedef struct Ptrlist Ptrlist;
typedef struct Graph Graph;
typedef struct Stack Stack;

struct Node{
    uint8_t *character; // Will be a hashmap of stored characters [20] -> "T" == 00010100
    Node *left; // output
    Node *right; //output2
    uint8_t length;
    // int lastlist;
};

struct Ptrlist {
    Ptrlist *next;
    Node *node;
};

Ptrlist* ptrlist();
void ptrlist_deconstructor(Ptrlist **self);
void append(Ptrlist **self, Node *node);
Node* dequeue(Ptrlist **self);

struct Graph{
    Node *nodes;
    int length_of_nodes;
};

struct Stack {
    Node *next;
    uint8_t length;
};
Stack* stack();
void push(Stack *self, uint8_t character);
uint8_t pop(Stack *self);


Node* node(Node *out, Node* out1);
Node* node_single(uint8_t character, Node *out, Node* out1);
Node* node_copy(Node* to_copy);

Graph *graph();
void free_nodes(Graph *self);

#endif // UTILS
