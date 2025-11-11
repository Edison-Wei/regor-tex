#if !defined(PARSER)
#define PARSER

#include "../util/include/util.h"

void deconstruct(Graph *parser);

Graph* parse_regex(char *regex);
Node* parse_group(char *regex, uint8_t *pos);
Node* parse_bracket(char *regex, uint8_t *pos);
Node* parse_or(char *regex, uint8_t *pos);
Node* parse_repeat(char *regex, uint8_t *pos, Node *target_node);
Node* parse_repeat_specified(char *regex, uint8_t *pos, Node *target_node);

#endif // PARSER
