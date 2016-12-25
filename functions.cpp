#include "functions.h"
using namespace std;

node* generateSearchTree(int depth, int branch_factor, node *root){
	node *ind;
	if(root == NULL){
		ind = new node;
		ind->sibling = NULL;
		ind->parent = NULL;
		ind->min_or_max = true;
		root = ind;
	}
	if(depth == 1){
		root->children = NULL;
		root->value = rand();
		return root;
	}
	ind = new node;
	root->children = ind;
	for(int i = 1; i <= branch_factor; i++){
		ind->parent = root;
		ind->min_or_max = !(root->min_or_max);
		generateSearchTree(depth - 1, branch_factor, ind);
		if(i < branch_factor){
			ind->sibling = new node;
			ind = ind->sibling;
		}
		else ind->sibling = NULL;
	}
	return root;
}

int AlphaBeta(node* currentNode, int alpha, int beta, bool maximisingPlayer){
		int bestValue, i , childValue;
		if(currentNode->children == 0){
				bestValue = currentNode->value;
		}
		else if(maximisingPlayer){
				bestValue = alpha;

				// Recurse for all children of node.
				node *index = currentNode->children;
				while(index != NULL){
						childValue = AlphaBeta(index, bestValue, beta, false);
						bestValue = std::max(bestValue, childValue);
						if(beta <= bestValue){
								break;
						}
						index = index->sibling;
				}
		}
		else{
				bestValue = beta;
				node *index = currentNode->children;
				// Recurse for all children of node.
				
				while(index != NULL){
						childValue = AlphaBeta(index, alpha, bestValue, true);
						bestValue = std::min(bestValue, childValue);
						if(bestValue <= alpha){
								break;
						}
						index = index->sibling;
				}
		}
		return bestValue;
}

int MinMax(node* currentNode, int alpha, int beta){
		int bestValue, i, childValue;
		if(currentNode->children == 0){
			return currentNode->min_or_max ? currentNode->value : currentNode->value * (-1);
		}
		bestValue = alpha;
				
		// Recurse for all children of node.
		node *index = currentNode->children;
		while(index != NULL){
			childValue = MinMax(index, -beta, -bestValue) * (-1);
			bestValue = std::max(bestValue, childValue);
			if(bestValue >= beta) return bestValue;
			index = index->sibling;
		}
		return bestValue;
}
