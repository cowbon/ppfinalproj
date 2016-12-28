#include "functions.h"
using namespace std;

node* generateSearchTree(int depth, int branch_factor, node *root){
	node *ind;
	if(root == NULL){
		ind = new node;
		//ind->sibling = NULL;
		ind->parent = NULL;
		ind->min_or_max = true;
		root = ind;
	}
	if(depth == 1){
		root->children = NULL;
		root->value = rand()%1024;
		root->num_of_children = 0;
		return root;
	}
	ind = new node[branch_factor];
	root->children = ind;
	root->num_of_children = branch_factor;
	for(int i = 0; i < branch_factor; i++){
		(ind+i)->parent = root;
		(ind+i)->min_or_max = !(root->min_or_max);
		generateSearchTree(depth - 1, branch_factor, (ind+i));
		/*if(i < branch_factor){
			ind->sibling = new node;
			ind = ind->sibling;
		}
		else ind->sibling = NULL;*/
	}
	return root;
}

int AlphaBeta(node* currentNode, int alpha, int beta, bool maximisingPlayer){
		int bestValue, childValue;
		if(currentNode->children == 0){
				bestValue = currentNode->value;
		}
		else if(maximisingPlayer){
			bestValue = alpha;

			// Recurse for all children of node.
			node *index = currentNode->children;
			for(int i = 0; i < currentNode->num_of_children; i++){
				childValue = AlphaBeta(index+i, bestValue, beta, false);
				bestValue = std::max(bestValue, childValue);
				if(beta <= bestValue){
					break;
				}
				//index = index->sibling;
			}
		}
		else{
			bestValue = beta;
			node *index = currentNode->children;
			// Recurse for all children of node.
			
			for(int i = 0; i < currentNode->num_of_children; i++){
				childValue = AlphaBeta(index+i, alpha, bestValue, true);
				bestValue = std::min(bestValue, childValue);
				if(bestValue <= alpha){
					break;
				}
				//index = index->sibling;
			}
		}
		return bestValue;
}

int MinMax(node* currentNode, int alpha, int beta){
		int bestValue, childValue;
		if(currentNode->children == 0){
			return currentNode->min_or_max ? currentNode->value : currentNode->value * (-1);
		}
		bestValue = alpha;
				
		// Recurse for all children of node.
		node *index = currentNode->children;
		for(int i = 0; i < currentNode->num_of_children; i++){
			childValue = MinMax(index+i, -beta, -bestValue) * (-1);
			bestValue = std::max(bestValue, childValue);
			if(bestValue >= beta) return bestValue;
			//index = index->sibling;
		}
		return bestValue;
}

int MinMax(node* currentNode){
	int bestValue, childValue;
	if (currentNode->children == 0)
		return currentNode->min_or_max ? currentNode->value : currentNode->value * (-1);

	bestValue = INT_MIN;

	// Recurse for all children of node.
	node *index = currentNode->children;
	for(int i = 0; i < currentNode->num_of_children; i++){
		childValue = MinMax(index)*(-1);
		bestValue = std::max(bestValue, childValue);
		//if(bestValue >= beta) return bestValue;
	}
	return bestValue;
}

void* parallel_(void* tmp){
	alphabeta* target = (alphabeta*)tmp;

	for (int  i = target->idx; i < target->branch_factor; i+=target->num_of_thread){
		//cout <<target->idx<<' '<<i<<' ' <<target->branch_factor<<' '<<target->num_of_thread<<endl; 
		int childValue = AlphaBeta(target->temp+i, INT_MIN, INT_MAX, false);
		//cout<<target->idx<<":"<<childValue<<endl;
		//target->result[target->idx] = std::max(target->result[target->idx], childValue);
		if (childValue > *(target->result+target->idx*sizeof(int))) *(target->result+target->idx*sizeof(int)) = childValue;
		//cout<<*(target->result+target->idx*sizeof(int))<<endl;
	}

	return NULL;
}

int newparaAlphaBeta(node* currentNode, int branch_factor, int num_of_thread, int* result, pthread_t* thr, alphabeta** t){

	int bestValue = INT_MIN;
	for(int i = 0; i < num_of_thread; i++){
		pthread_create(&thr[i], NULL, &parallel_, (void*)t[i]);
	}
	for(int i = 0; i < num_of_thread; i++)
		pthread_join(thr[i], NULL);

	for(int i = 0; i < num_of_thread; i++){
		//cout<<*(result+i*sizeof(int))<<' ';
		bestValue = std::max(bestValue, *(result+i*sizeof(int)));
	}
	//cout<<endl;
	//pthread_barrier_destroy(&barr);
	return bestValue;
}
