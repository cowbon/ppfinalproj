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

struct alphabeta{
	node* temp;
	int* result;
	int branch_factor;
	int num_of_thread;
	int idx;
	alphabeta(node* temp, int* result, int branch_factor, int num_of_thread, int idx):temp(temp), result(result), branch_factor(branch_factor), num_of_thread(num_of_thread), idx(idx){}
};

node* generateSearchTree(int depth, int branch_factor, node* root);
int AlphaBeta(node* currentNode, int alpha, int beta, bool maximisingPlayer);
//int MinMax(node* currentNode, bool maximisingPlayer);
int MinMax(node*, int, int);
int MinMax(node*);
void* paragenerateSearchTree(void*);
int newparaAlphaBeta(node* currentNode, int branch_factor, int num_of_thread, int* result, pthread_t* thr, alphabeta** t);
#endif
