#ifndef FUNC
#define FUNC
#include "node.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <climits>

struct param{
	int depth;
	int branch_factor;
	node* root;
	bool para;
	param(int depth, int branch_factor, node* root, bool para):depth(depth), branch_factor(branch_factor), root(root), para(para){} 
};

node* generateSearchTree(int depth, int branch_factor, node* root);
int AlphaBeta(node* currentNode, int alpha, int beta, bool maximisingPlayer);
//int MinMax(node* currentNode, bool maximisingPlayer);
int MinMax(node*, int, int);
void* paragenerateSearchTree(void*);

#endif
