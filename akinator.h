#ifndef AKINATOR_H_
#define AKINATOR_H_

#include "tree.h"
#include "stack/stack.h"
const char nil[3] = { 'n', 'i', 'l' };

void Game();
void ReadingNodes( Tree* tree, Node* node, int fileSize, char* buffer, int* i, int* bracket_counter );

#endif // #define AKINATOR_H_