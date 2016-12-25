#ifndef FUNC
#define FUNC
#include "node.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <climits>

node* generateSearchTree(int depth, int branch_factor, node* root);
int AlphaBeta(node* currentNode, int alpha, int beta, bool maximisingPlayer);
//int MinMax(node* currentNode, bool maximisingPlayer);
int MinMax(node*, int, int);

#endif
